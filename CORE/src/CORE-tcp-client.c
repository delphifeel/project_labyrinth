#include <uv.h>
#include "CORE.h"

/*****************************************************************************************************************************/

#define CORE_TCPSERVER_DEFAULT_BACKLOG 		(128)

CORE_OBJECT_INTERFACE(CORE_TCPClient,
    void                    *context;

	/*
	 * 		libuv specific data types
	 */
	uv_loop_t  				*uv_loop;
	uv_tcp_t  				uv_tcp_client;
    uv_connect_t            uv_connection;

	/*
	 * 		socket data
	 */
	struct sockaddr_in  	server_addr;
	uint32  				server_port;

    /*
     *      callbacks
     */
    CORE_TCPClient_OnReadFunc               on_read;
    CORE_TCPClient_OnWriteFunc              on_write;
    CORE_TCPClient_OnConnectedFunc          on_connected;
    CORE_TCPClient_OnCloseConnectionFunc    on_close_connection;

    /* 		temp fields related to write process. 
     *		DO NOT RELY ON IT, CAUSE IT CAN BE CHANGED ANYTIME.
     */
    uv_write_t              temp_write_request_handle;
    uv_buf_t                temp_write_buffer;
);

/*****************************************************************************************************************************/

static void _UVHandleSetContext(uv_handle_t *handle, CORE_TCPClient instance)
{
    uv_handle_set_data(handle, instance);
}

static void _UVHandleGetContext(uv_handle_t *handle, CORE_TCPClient *instance_ptr)
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
    CORE_TCPClient     instance;


    _UVHandleGetContext(handle, &instance);

    CORE_DebugInfo("Close connection to server\n");
    if (instance->on_close_connection != NULL)
    {
        instance->on_close_connection(instance, instance->context);
    }
}

static void _OnWriteBuffer(uv_write_t* request, int status) 
{
    CORE_TCPClient instance;


    _UVHandleGetContext((uv_handle_t *) request, &instance);
    CORE_MemFree(instance->temp_write_buffer.base);
    CORE_MemZero(&instance->temp_write_buffer, sizeof(uv_buf_t));

    if (instance->on_write != NULL)
    {
        instance->on_write(instance, instance->context);
    }

    if (status < 0)
    {
        CORE_DebugError("Write buffer error\n");
        return;
    }
}

static void _OnReadBuffer(uv_stream_t *client, ssize_t nread, const uv_buf_t *buf) 
{
    CORE_TCPClient 	instance;


    _UVHandleGetContext((uv_handle_t *) client, &instance);

    if (nread > 0) 
    {
        CORE_DebugInfo("Read %lu bytes\n", nread);
        if (instance->on_read != NULL)
        {
            instance->on_read(instance, instance->context, (const uint8 *) buf->base, nread);
        }
    }
    else if (nread < 0) 
    {
        if (nread != UV_EOF)
        {
        	CORE_DebugError("Read error\n");
        }

        uv_close((uv_handle_t *) client, _OnHandleClose);
    }

    CORE_MemFree(buf->base);
}

static void _OnConnected(uv_connect_t *connection, int status)
{
    CORE_TCPClient instance;


	if (status < 0) 
	{
		CORE_DebugError("Connection to server failed\n");
        return;
    }

    _UVHandleGetContext((uv_handle_t *) connection->handle, &instance);
    CORE_DebugInfo("Connected to server (status %d)\n", status);
    if (instance->on_connected != NULL)
    {
        instance->on_connected(instance, instance->context);
    }

    CORE_DebugInfo("Start read...\n");
    uv_read_start(connection->handle, _AllocBuffer, _OnReadBuffer);
}

void CORE_TCPClient_Write(CORE_TCPClient instance, const uint8 data[], uint32 data_size)
{
    CORE_AssertPointer(data);

    char *data_alloced;


    data_alloced = CORE_MemAlloc(data_size);
    memcpy(data_alloced, data, data_size);

    instance->temp_write_buffer.base = data_alloced;
    instance->temp_write_buffer.len = data_size;

    _UVHandleSetContext((uv_handle_t *) &instance->temp_write_request_handle, instance);
    if (uv_write(&instance->temp_write_request_handle, 
                 (uv_stream_t *) &instance->uv_tcp_client, 
                 &instance->temp_write_buffer, 1, _OnWriteBuffer) != 0)
    {
        CORE_DebugError("Write error\n");
    }

    CORE_DebugInfo("Send %u bytes to server\n", data_size);
}

/*****************************************************************************************************************************/

void CORE_TCPClient_OnRead(CORE_TCPClient instance, CORE_TCPClient_OnReadFunc on_read)
{
    CORE_AssertPointer(on_read);

    instance->on_read = on_read;
}

void CORE_TCPClient_OnWrite(CORE_TCPClient instance, CORE_TCPClient_OnWriteFunc on_write)
{
    CORE_AssertPointer(on_write);

    instance->on_write = on_write;
}

void CORE_TCPClient_OnConnected(CORE_TCPClient instance, CORE_TCPClient_OnConnectedFunc on_connected)
{
    CORE_AssertPointer(on_connected);

    instance->on_connected = on_connected;
}

void CORE_TCPClient_OnCloseConnection(CORE_TCPClient instance, CORE_TCPClient_OnCloseConnectionFunc on_close_connection)
{
	CORE_AssertPointer(on_close_connection);

    instance->on_close_connection = on_close_connection;
}

/*****************************************************************************************************************************/

void CORE_TCPClient_SetContext(CORE_TCPClient instance, void *context)
{
    instance->context = context;
}

/*****************************************************************************************************************************/

CORE_Bool CORE_TCPClient_Connect(CORE_TCPClient instance, const char *dest_address, uint32 dest_port)
{
    instance->server_port = dest_port;
    instance->uv_loop = uv_default_loop();

    if (uv_tcp_init(instance->uv_loop, &instance->uv_tcp_client) != 0)
    {
        CORE_DebugError("TCP Init error\n");
        return FALSE;
    }
    _UVHandleSetContext((uv_handle_t *) &instance->uv_tcp_client, instance);

    if (uv_ip4_addr(dest_address, instance->server_port, &instance->server_addr) != 0)
    {
        CORE_DebugError("IP4 address create error\n");
        return FALSE;
    }
    if (uv_tcp_connect(&instance->uv_connection, 
                       &instance->uv_tcp_client, 
                       (const struct sockaddr *) &instance->server_addr, 
                       _OnConnected) != 0)
    {
        CORE_DebugError("TCP connect error\n");
        return FALSE;
    }

    CORE_DebugInfo("Connected to %s, port %u\n", dest_address, instance->server_port);

    uv_run(instance->uv_loop, UV_RUN_DEFAULT);

    return TRUE;
}

CORE_Bool CORE_TCPClient_Disconnect(CORE_TCPClient instance)
{
    uv_close((uv_handle_t *) instance->uv_connection.handle, _OnHandleClose);
    return TRUE;
}

/*****************************************************************************************************************************/

void CORE_TCPClient_Create(CORE_TCPClient *instance_ptr)
{
	CORE_OBJECT_CREATE(instance_ptr, CORE_TCPClient);
    
    CORE_TCPClient instance = *instance_ptr;

	instance->on_read = NULL;
    instance->on_write = NULL;
	instance->on_connected = NULL;
    instance->on_close_connection = NULL;
    instance->context = NULL;
    instance->server_port = 0;

    CORE_MemZero(&instance->server_addr, sizeof(instance->server_addr));
    CORE_MemZero(&instance->uv_connection, sizeof(instance->uv_connection));
    CORE_MemZero(&instance->temp_write_buffer, sizeof(instance->temp_write_buffer));
    CORE_MemZero(&instance->temp_write_request_handle, sizeof(instance->temp_write_request_handle));
}

void CORE_TCPClient_Free(CORE_TCPClient *instance_ptr)
{
    CORE_TCPClient instance = *instance_ptr;

    uv_loop_close(instance->uv_loop);

	CORE_OBJECT_FREE(instance_ptr);
}

/*****************************************************************************************************************************/
