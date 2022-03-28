#include "authserver/auth-io-system.h"


static AuthIOSystem Server; 

int main(void)
{
    CORE_DebugStdOut("[AUTH SERVER] Started\n");
    AuthIOSystem_Create(&Server);
    AuthIOSystem_Setup(Server);
    AuthIOSystem_Start(Server); 

    return 0;
}
