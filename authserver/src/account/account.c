#include "CORE.h"
#include "authserver/auth-command-types.h"
#include "authserver/account.h"


static const AuthenticatePayload _mocked_creds[] = 
{
    {"delphifeel", "1234"},
    {"dhatz", "1234"},
    {"cherniki", "12345"},
};


CORE_Bool Account_LogIn(const char *login, const char *password)
{
    uint32                          creds_array_size;
    CORE_Bool                       is_found;

    is_found = FALSE; 
    creds_array_size = sizeof(_mocked_creds) / sizeof(AuthenticatePayload); 

    for (uint32 i = 0; i < creds_array_size; i++)
    {
        if ((CORE_StringEqual(login, _mocked_creds[i].login) == TRUE) && 
            (CORE_StringEqual(password, _mocked_creds[i].password) == TRUE))
        {
            CORE_DebugInfo("Found user: %s - return auth creds\n", login);
            is_found = TRUE;
            break;
        }
    }

    if (is_found == FALSE)
    {
        CORE_DebugError("Can't login - user not found: %s\n", login);
        return FALSE;
    }

    return is_found;
}

