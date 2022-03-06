#include "CORE.h"
#include "authserver/auth-command-types.h"
#include "authserver/account.h"


CORE_OBJECT_INTERFACE(AccountRepository, 
    const char * url
);


static const AuthenticatePayload _mocked_creds[] = 
{
    {"delphifeel", "1234"},
    {"dhatz", "1234"},
    {"cherniki", "12345"},
};


void AccountRepository_Create(AccountRepository *instance_ptr){
    CORE_OBJECT_CREATE(instance_ptr, AccountRepository); 
}

void AccountRepository_Free(AccountRepository *instance_ptr){
    CORE_OBJECT_FREE(instance_ptr);
}

void AccountRepository_Setup(AccountRepository instance, const char *url){
    instance->url = url;
}

void AccountRepository_SetURL(AccountRepository instance, const char *url){
    instance->url = url;
}

CORE_Bool AccountRepository_Find(const AccountRepository instance, const char *login, const char *password) {
    FILE                           *fp; 
    char                           *_login, *_password;
    char                           buffer[MAX_LOGIN_SIZE + MAX_PASSWORD_SIZE]; 
    int                            position = 0;
    int                            chr; 

    if((fp = fopen(instance->url, "r")) == NULL){
        CORE_DebugInfo("Cannot open file: %s", instance->url);  
        return FALSE;
    }

    while ((chr = getc(fp)) != EOF) {
        if (chr != '\n'){
            buffer[position++] = chr;
            continue;  
        }

        buffer[position] = '\0'; 

        _login = strtok(buffer, ",");
        _password = strtok(NULL, ",");

        if(CORE_StringEqual(_login, login) && CORE_StringEqual(_password, password)){
            CORE_DebugInfo("User found: %s.", login); 
            fclose(fp);
            return TRUE;
        }

        position = 0;
    }

    fclose(fp);
    return FALSE; 
}


CORE_Bool AccountRepository_Add(const AccountRepository instance, const char *login, const char *password){
    FILE                            *fp;

    if(AccountRepository_Find(instance, login, password)){
        CORE_DebugInfo("Cannot create duplicate"); 
        return FALSE;
    }

    if((fp = fopen(instance->url, "w")) == NULL){
        CORE_DebugInfo("Cannot open file: %s", instance->url);  
        return FALSE;
    }

    fseek(fp, SEEK_CUR, SEEK_END); 

    for(int i = 0; i < CORE_StringLength(login); i++)
        putc(login[i], fp); 
    
    putc(',', fp); 
    // TODO (dhatz): Secure storing password
    for(int i = 0; i < CORE_StringLength(password); i++)
        putc(password[i], fp);
    
    putc('\n', fp); 

    CORE_DebugInfo("New account added."); 
    fclose(fp);
    return TRUE;
}

// CORE_Bool AccountRepository_Remove(const AccountRepository instance, const char *login); 


// CORE_Bool AccountRepository_Update(const AccountRepository instance, const char *login, )


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
