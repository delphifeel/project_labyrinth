#include "authserver/auth-io-system.h"
#include "authserver/account.h"



static AuthIOSystem Server; 

int main(void)
{
    AccountRepository account_repository; 
    AccountRepository_Create(&account_repository); 
    AccountRepository_Setup(account_repository, "storage.csv"); 

    CORE_DebugPrint("[AUTH SERVER] Started\n");
    AuthIOSystem_Create(&Server);
    AuthIOSystem_Setup(Server);
    AuthIOSystem_Start(Server); 

    return 0;
}
