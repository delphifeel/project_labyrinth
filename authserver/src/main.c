#include "authserver/auth-io-system.h"


static AuthIOSystem Server; 

// void on_read(CORE_TCPServer instance, void *context, CORE_TCPServer_ClientConnection client_connection, const uint8 data[], uint32 data_size)
// {
//     CORE_TCPServer_Write(instance, client_connection, data, data_size);
// }

int main()
{
    AuthIOSystem_Create(&Server);
    AuthIOSystem_Setup(Server);
    AuthIOSystem_Start(Server); 
}
