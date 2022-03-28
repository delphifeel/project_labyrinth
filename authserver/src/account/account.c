#include "CCORE.h"
#include "authserver/auth-command-types.h"
#include "authserver/account.h"


static const AuthenticatePayload _mocked_creds[] = 
{
    {"delphifeel", "1234"},
    {"dhatz", "1234"},
    {"cherniki", "12345"},
};


bool Account_LogIn(const char *login, const char *password)
{
    uint32                          creds_array_size;
    bool                       is_found;

    is_found = false; 
    creds_array_size = sizeof(_mocked_creds) / sizeof(AuthenticatePayload); 

    for (uint32 i = 0; i < creds_array_size; i++)
    {
        if (CORE_StrEqual(login, _mocked_creds[i].login) && 
            CORE_StrEqual(password, _mocked_creds[i].password))
        {
            CORE_DebugInfo("Found user: %s - return auth creds\n", login);
            is_found = true;
            break;
        }
    }

    if (is_found == false)
    {
        CORE_DebugError("Can't login - user not found: %s\n", login);
        return false;
    }

    return is_found;
}

