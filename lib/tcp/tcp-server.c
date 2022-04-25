#include <uv.h>
#include "lib/tcp/tcp-server.h"

/*****************************************************************************************************************************/

#define TCPSERVER_DEFAULT_BACKLOG 		(128)

CORE_OBJECT_INTERFACE(TCPServer,
    void                    *context;

	/*
	 * 		libuv specific data types
	 */
	uv_loop_t  				*uv_loop;
	uv_tcp_t  				uv_tcp_server;

	/*
	 * 		socket data
	 */
	struct sockaddr_in  	addr;
	uint32  				port;

    /*
     *      callbacks
     */
    OnReadFunc              on_read;
    OnErrorFunc             on_error;
    OnNewConnectionFunc     on_new_connection;
    OnCloseConnectionFunc	on_close_connection;
);

typedef struct {
    uv_write_t  req;
    uv_buf_t    buf;
} WriteRecord;

/*****************************************************************************************************************************/

static inline TCPServer_ClientConnection _UVClientToClientConnection(uv_stream_t *client)
{
    return (TCPServer_ClientConnection) client;
}

static inline uv_stream_t* _ClientConnectionToUVClient(TCPServer_ClientConnection client_connection)
{
    return (uv_stream_t *) client_connection;
}

static void _UVHandleSetContext(uv_handle_t *handle, TCPServer instance)
{
    uv_handle_set_data(handle, instance);
}

static void _UVHandleGetContext(uv_handle_t *handle, TCPServer *instance_ptr)
{
    *instance_ptr = uv_handle_get_data(handle);
}

static void _AllocBuffer(uv_handle_t *handle, size_t suggested_size, uv_buf_t *buf) 
{
    buf->base = (char *) CORE_MemCalloc(suggested_size, 1);
    buf->len = suggested_size;
}

static void _OnHandleClose(uv_handle_t* handle) 
{
    CORE_MemFree(handle);
}

static void _OnWriteBuffer(uv_write_t *req, int status) 
{
    WriteRecord *write_record = (WriteRecord *) req;
    free(write_record->buf.base);
    free(write_record);

    if (status < 0)
    {
        CORE_DebugError("Write buffer error\n");
        return;
    }
}

static void _OnReadBuffer(uv_stream_t *client, ssize_t nread, const uv_buf_t *buf) 
{
    TCPServer 	                    instance;
    TCPServer_ClientConnection     client_connection;


    client_connection = _UVClientToClientConnection(client);
    _UVHandleGetContext((uv_handle_t *) client, &instance);

    if (nread > 0) {
        instance->on_read(instance, instance->context, client_connection, (const uint8 *) buf->base, nread);
    }
    else if (nread < 0) {
        if (nread != UV_EOF) {
        	if (instance->on_error != NULL) {
        		instance->on_error(instance, instance->context, "Read error");
        	}
        }

        if (instance->on_close_connection != NULL) {
    		instance->on_close_connection(instance, instance->context, client_connection);
    	}
        uv_close((uv_handle_t *) client, _OnHandleClose);
    }

    CORE_MemFree(buf->base);
}

static void _OnNewConnection(uv_stream_t *server, int status)
{
	TCPServer                      instance;
	uv_tcp_t                            *client;
    TCPServer_ClientConnection     client_connection;


	_UVHandleGetContext((uv_handle_t *) server, &instance);

	if (status < 0) 
	{
		if (instance->on_error != NULL)
		{
			instance->on_error(instance, instance->context, "New connection error");
		}
        return;
    }

    client = (uv_tcp_t *) CORE_MemAlloc(sizeof(uv_tcp_t), 1);

    uv_tcp_init(instance->uv_loop, client);
    _UVHandleSetContext((uv_handle_t *) client, instance);
    client_connection = _UVClientToClientConnection((uv_stream_t *) client);

    if (uv_accept(server, (uv_stream_t *) client) != 0)
    {
        if (instance->on_close_connection != NULL)
        {
            instance->on_close_connection(instance, instance->context, client_connection);
        }
        uv_close((uv_handle_t *) client, _OnHandleClose);
        return;
    }

    if (instance->on_new_connection != NULL)
    {
        instance->on_new_connection(instance, instance->context, client_connection);
    }

    uv_read_start((uv_stream_t *) client, _AllocBuffer, _OnReadBuffer);
}

void TCPServer_Write(TCPServer instance, TCPServer_ClientConnection client_connection,
                          const uint8 data[], uint32 data_size)
{
    CORE_AssertPointer(data);

    WriteRecord *write_record = CORE_MemAlloc(sizeof(WriteRecord), 1);
    char *data_alloced = CORE_MemAlloc(sizeof(char), data_size);
    CORE_MemCpy(data_alloced, data, data_size);

    write_record->buf.base  = data_alloced;
    write_record->buf.len   = data_size;
    if (uv_write((uv_write_t *) write_record, 
                 _ClientConnectionToUVClient(client_connection), 
                 &write_record->buf, 
                 1, 
                 _OnWriteBuffer) != 0)
    {
        if (instance->on_error != NULL)
        {
            instance->on_error(instance, instance->context, "Write error");
        }
    }
}

void TCPServer_CloseConnection(TCPServer instance, TCPServer_ClientConnection client_connection)
{
    CORE_AssertPointer(client_connection);

    uv_close(
        (uv_handle_t *) _ClientConnectionToUVClient(client_connection),
        _OnHandleClose
    );
}

/*****************************************************************************************************************************/

void TCPServer_OnRead(TCPServer instance, OnReadFunc on_read)
{
    CORE_AssertPointer(on_read);

    instance->on_read = on_read;
}

void TCPServer_OnError(TCPServer instance, OnErrorFunc on_error)
{
    CORE_AssertPointer(on_error);

    instance->on_error = on_error;
}

void TCPServer_OnNewConnection(TCPServer instance, OnNewConnectionFunc on_new_connection)
{
    CORE_AssertPointer(on_new_connection);

    instance->on_new_connection = on_new_connection;
}

void TCPServer_OnCloseConnection(TCPServer instance, OnCloseConnectionFunc on_close_connection)
{
	CORE_AssertPointer(on_close_connection);

    instance->on_close_connection = on_close_connection;
}

/*****************************************************************************************************************************/

void TCPServer_SetContext(TCPServer instance, void *context)
{
    instance->context = context;
}

/*****************************************************************************************************************************/

void TCPServer_Setup(TCPServer instance, uint32 port)
{
	CORE_AssertPointer(instance->on_read);


	instance->port = port;
	instance->uv_loop = uv_default_loop();

    uv_tcp_init(instance->uv_loop, &instance->uv_tcp_server);
    _UVHandleSetContext((uv_handle_t *) &instance->uv_tcp_server, instance);

    uv_ip4_addr("0.0.0.0", instance->port, &instance->addr);
    uv_tcp_bind(&instance->uv_tcp_server, (const struct sockaddr *) &instance->addr, 0);
}

void TCPServer_Start(TCPServer instance)
{
    int32 error;


    error = uv_listen((uv_stream_t *) &instance->uv_tcp_server, TCPSERVER_DEFAULT_BACKLOG, _OnNewConnection);
    if (error) 
    {
    	if (instance->on_error != NULL)
    	{
    		instance->on_error(instance, instance->context, "Socket listen error");
    	}
        return;
    }

    CORE_DebugInfo("Starting server on port %u\n", instance->port);
    uv_run(instance->uv_loop, UV_RUN_DEFAULT);
}

/*****************************************************************************************************************************/

void TCPServer_Create(TCPServer *instance_ptr)
{
	CORE_OBJECT_CREATE(instance_ptr, TCPServer);
    TCPServer instance = *instance_ptr;

	instance->on_read = NULL;
	instance->on_new_connection = NULL;
    instance->on_close_connection = NULL;
	instance->on_error = NULL;
    instance->context = NULL;
}

void TCPServer_Free(TCPServer *instance_ptr)
{
    TCPServer instance = *instance_ptr;
    uv_loop_close(instance->uv_loop);
	CORE_OBJECT_FREE(instance_ptr);
}

/*****************************************************************************************************************************/
