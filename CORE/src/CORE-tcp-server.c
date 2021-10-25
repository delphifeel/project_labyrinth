#include <uv.h>
#include "CORE/CORE-tcp-server.h"

/*****************************************************************************************************************************/

#define CORE_TCPSERVER_DEFAULT_BACKLOG 		(128)

CORE_OBJECT_INTERFACE(CORE_TCPServer,
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

    /* 		temp fields related to write process. 
     *		DO NOT RELY ON IT, CAUSE IT CAN BE CHANGED ANYTIME.
     */
    uv_write_t  			temp_write_request_handle;
    uv_buf_t  				temp_write_buffer;
);

/*****************************************************************************************************************************/

static inline CORE_TCPServer_ClientConnection _UVClientToClientConnection(uv_stream_t *client)
{
    return (CORE_TCPServer_ClientConnection) client;
}

static inline uv_stream_t* _ClientConnectionToUVClient(CORE_TCPServer_ClientConnection client_connection)
{
    return (uv_stream_t *) client_connection;
}

static void _UVHandleSetContext(uv_handle_t *handle, CORE_TCPServer instance)
{
    uv_handle_set_data(handle, instance);
}

static void _UVHandleGetContext(uv_handle_t *handle, CORE_TCPServer *instance_ptr)
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

static void _OnWriteBuffer(uv_write_t* request, int status) 
{
    CORE_TCPServer instance;


    _UVHandleGetContext((uv_handle_t *) request, &instance);
    CORE_MemFree(instance->temp_write_buffer.base);
    CORE_MemZero(&instance->temp_write_buffer, sizeof(uv_buf_t));

    if (status < 0)
    {
        CORE_DebugError("Write buffer error\n");
        return;
    }
}

static void _OnReadBuffer(uv_stream_t *client, ssize_t nread, const uv_buf_t *buf) 
{
    CORE_TCPServer 	                    instance;
    CORE_TCPServer_ClientConnection     client_connection;


    client_connection = _UVClientToClientConnection(client);
    _UVHandleGetContext((uv_handle_t *) client, &instance);

    if (nread > 0) 
    {
        instance->on_read(instance, instance->context, client_connection, (const uint8 *) buf->base, nread);
    }
    else if (nread < 0) 
    {
        if (nread != UV_EOF)
        {
        	if (instance->on_error != NULL)
        	{
        		instance->on_error(instance, instance->context, "Read error");
        	}
        }

        if (instance->on_close_connection != NULL)
    	{
    		instance->on_close_connection(instance, instance->context, client_connection);
    	}
        uv_close((uv_handle_t *) client, _OnHandleClose);
    }

    CORE_MemFree(buf->base);
}

static void _OnNewConnection(uv_stream_t *server, int status)
{
	CORE_TCPServer                      instance;
	uv_tcp_t                            *client;
    CORE_TCPServer_ClientConnection     client_connection;


	_UVHandleGetContext((uv_handle_t *) server, &instance);

	if (status < 0) 
	{
		if (instance->on_error != NULL)
		{
			instance->on_error(instance, instance->context, "New connection error");
		}
        return;
    }

    client = (uv_tcp_t *) CORE_MemAlloc(sizeof(uv_tcp_t));

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

void CORE_TCPServer_Write(CORE_TCPServer instance, CORE_TCPServer_ClientConnection client_connection,
                          const uint8 data[], uint32 data_size)
{
    CORE_AssertPointer(data);

    char *data_alloced;


    data_alloced = CORE_MemAlloc(data_size);
    memcpy(data_alloced, data, data_size);

    instance->temp_write_buffer.base = data_alloced;
    instance->temp_write_buffer.len = data_size;

    _UVHandleSetContext((uv_handle_t *) &instance->temp_write_request_handle, instance);
    if (uv_write(&instance->temp_write_request_handle, _ClientConnectionToUVClient(client_connection), 
                 &instance->temp_write_buffer, 1, _OnWriteBuffer) != 0)
    {
        if (instance->on_error != NULL)
        {
            instance->on_error(instance, instance->context, "Write error");
        }
    }
}

/*****************************************************************************************************************************/

void CORE_TCPServer_OnRead(CORE_TCPServer instance, OnReadFunc on_read)
{
    CORE_AssertPointer(on_read);

    instance->on_read = on_read;
}

void CORE_TCPServer_OnError(CORE_TCPServer instance, OnErrorFunc on_error)
{
    CORE_AssertPointer(on_error);

    instance->on_error = on_error;
}

void CORE_TCPServer_OnNewConnection(CORE_TCPServer instance, OnNewConnectionFunc on_new_connection)
{
    CORE_AssertPointer(on_new_connection);

    instance->on_new_connection = on_new_connection;
}

void CORE_TCPServer_OnCloseConnection(CORE_TCPServer instance, OnCloseConnectionFunc on_close_connection)
{
	CORE_AssertPointer(on_close_connection);

    instance->on_close_connection = on_close_connection;
}

/*****************************************************************************************************************************/

void CORE_TCPServer_SetContext(CORE_TCPServer instance, void *context)
{
    instance->context = context;
}

/*****************************************************************************************************************************/

void CORE_TCPServer_Setup(CORE_TCPServer instance, uint32 port)
{
	CORE_AssertPointer(instance->on_read);


	instance->port = port;
	instance->uv_loop = uv_default_loop();

    uv_tcp_init(instance->uv_loop, &instance->uv_tcp_server);
    _UVHandleSetContext((uv_handle_t *) &instance->uv_tcp_server, instance);

    uv_ip4_addr("0.0.0.0", instance->port, &instance->addr);
    uv_tcp_bind(&instance->uv_tcp_server, (const struct sockaddr *) &instance->addr, 0);
}

void CORE_TCPServer_Start(CORE_TCPServer instance)
{
    int32 error;


    error = uv_listen((uv_stream_t *) &instance->uv_tcp_server, CORE_TCPSERVER_DEFAULT_BACKLOG, _OnNewConnection);
    if (error) 
    {
    	if (instance->on_error != NULL)
    	{
    		instance->on_error(instance, instance->context, "Socket listen error");
    	}
        return;
    }

    CORE_DebugInfo("Starting server on port %lu\n", instance->port);
    uv_run(instance->uv_loop, UV_RUN_DEFAULT);
}

/*****************************************************************************************************************************/

void CORE_TCPServer_Create(CORE_TCPServer *instance_ptr)
{
	CORE_OBJECT_CREATE(instance_ptr, CORE_TCPServer);

	(*instance_ptr)->on_read = NULL;
	(*instance_ptr)->on_new_connection = NULL;
    (*instance_ptr)->on_close_connection = NULL;
	(*instance_ptr)->on_error = NULL;
    (*instance_ptr)->context = NULL;

	CORE_MemZero(&(*instance_ptr)->temp_write_buffer, sizeof(uv_buf_t));
	CORE_MemZero(&(*instance_ptr)->temp_write_request_handle, sizeof(uv_write_t));
}

void CORE_TCPServer_Free(CORE_TCPServer *instance_ptr)
{
	CORE_OBJECT_FREE(instance_ptr);
}

/*****************************************************************************************************************************/