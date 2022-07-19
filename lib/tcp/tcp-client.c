#include <uv.h>
#include "lib/tcp/tcp-client.h"

/*****************************************************************************************************************************/

#define TCPSERVER_DEFAULT_BACKLOG 		(128)

typedef struct TCPClient_s 
{
    void                    *context;

	/*
	 * 		libuv specific data types
	 */
    uv_loop_t               uv_loop_struct;
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
    TCPClient_OnReadFunc               on_read;
    TCPClient_OnWriteFunc              on_write;
    TCPClient_OnConnectedFunc          on_connected;
    TCPClient_OnCloseConnectionFunc    on_close_connection;

    /* 		temp fields related to write process. 
     *		DO NOT RELY ON IT, CAUSE IT CAN BE CHANGED ANYTIME.
     */
    uv_write_t              temp_write_request_handle;
    uv_buf_t                temp_write_buffer;
} TCPClient;

/*****************************************************************************************************************************/

static void _UVHandleSetContext(uv_handle_t *handle, TCPClient *instance)
{
    uv_handle_set_data(handle, instance);
}

static void _UVHandleGetContext(uv_handle_t *handle, TCPClient **instance_ptr)
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
    TCPClient *instance;


    _UVHandleGetContext(handle, &instance);

    // CORE_DebugInfo("Close connection to server\n");
    if (instance->on_close_connection != NULL)
    {
        instance->on_close_connection(instance, instance->context);
    }
}

static void _OnWriteBuffer(uv_write_t* request, int status) 
{
    TCPClient *instance;


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
    TCPClient *instance;


    _UVHandleGetContext((uv_handle_t *) client, &instance);

    if (nread > 0) 
    {
        // CORE_DebugInfo("Read %lu bytes\n", nread);
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
    TCPClient *instance;


	if (status < 0) 
	{
		CORE_DebugError("Connection to server failed\n");
        return;
    }

    _UVHandleGetContext((uv_handle_t *) connection->handle, &instance);
    // CORE_DebugInfo("Connected to server (status %d)\n", status);
    if (instance->on_connected != NULL)
    {
        instance->on_connected(instance, instance->context);
    }

    // CORE_DebugInfo("Start read...\n");
    uv_read_start(connection->handle, _AllocBuffer, _OnReadBuffer);
}

void TCPClient_Write(TCPClient *instance, const uint8 data[], uint32 data_size)
{
    CORE_AssertPointer(data);

    char *data_alloced;


    data_alloced = CORE_MemAlloc(sizeof(char), data_size);
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

    // CORE_DebugInfo("Send %u bytes to server\n", data_size);
}

/*****************************************************************************************************************************/

void TCPClient_OnRead(TCPClient *instance, TCPClient_OnReadFunc on_read)
{
    CORE_AssertPointer(on_read);

    instance->on_read = on_read;
}

void TCPClient_OnWrite(TCPClient *instance, TCPClient_OnWriteFunc on_write)
{
    CORE_AssertPointer(on_write);

    instance->on_write = on_write;
}

void TCPClient_OnConnected(TCPClient *instance, TCPClient_OnConnectedFunc on_connected)
{
    CORE_AssertPointer(on_connected);

    instance->on_connected = on_connected;
}

void TCPClient_OnCloseConnection(TCPClient *instance, TCPClient_OnCloseConnectionFunc on_close_connection)
{
	CORE_AssertPointer(on_close_connection);

    instance->on_close_connection = on_close_connection;
}

/*****************************************************************************************************************************/

void TCPClient_SetContext(TCPClient *instance, void *context)
{
    instance->context = context;
}

/*****************************************************************************************************************************/

bool TCPClient_Connect(TCPClient *instance, const char *dest_address, uint32 dest_port)
{
    instance->server_port = dest_port;
    uv_loop_init(&instance->uv_loop_struct);
    instance->uv_loop = &instance->uv_loop_struct;

    if (uv_tcp_init(instance->uv_loop, &instance->uv_tcp_client) != 0)
    {
        CORE_DebugError("TCP Init error\n");
        return false;
    }
    _UVHandleSetContext((uv_handle_t *) &instance->uv_tcp_client, instance);

    if (uv_ip4_addr(dest_address, instance->server_port, &instance->server_addr) != 0)
    {
        CORE_DebugError("IP4 address create error\n");
        return false;
    }
    if (uv_tcp_connect(&instance->uv_connection, 
                       &instance->uv_tcp_client, 
                       (const struct sockaddr *) &instance->server_addr, 
                       _OnConnected) != 0)
    {
        CORE_DebugError("TCP connect error\n");
        return false;
    }

    // CORE_DebugInfo("Connected to %s, port %u\n", dest_address, instance->server_port);

    uv_run(instance->uv_loop, UV_RUN_DEFAULT);

    return true;
}

bool TCPClient_Disconnect(TCPClient *instance)
{
    uv_close((uv_handle_t *) instance->uv_connection.handle, _OnHandleClose);
    return true;
}

/*****************************************************************************************************************************/

TCPClient *TCPClient_Create(void)
{
    TCPClient *instance = CORE_MemAlloc(sizeof(TCPClient), 1);

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

    return instance;
}

void TCPClient_Free(TCPClient *instance)
{
    uv_loop_close(instance->uv_loop);
    CORE_MemFree(instance);
}

/*****************************************************************************************************************************/
