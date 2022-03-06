# ifndef _ACCOUNT_H_
# define _ACCOUNT_H_

#include "CORE.h"


#define MAX_LOGIN_SIZE         (36)
#define MAX_PASSWORD_SIZE      (24)


CORE_OBJECT_DEFINE(AccountRepository);


typedef struct AuthenticatePayload
{
    // TODO(dhatz): UTF-8 support. 
    char                login[MAX_LOGIN_SIZE];
    char                password[MAX_PASSWORD_SIZE]; 
} AuthenticatePayload; 


CORE_Bool Account_LogIn(const char *login, const char *password);

void AccountRepository_Create(AccountRepository *instance_ptr); 
void AccountRepository_Setup(AccountRepository instance, const char *url);
void AccountRepository_Free(AccountRepository *instance_ptr); 
void AccountRepository_SetURL(AccountRepository instance, const char *url); 
CORE_Bool AccountRepository_Add(AccountRepository instance, const char *login, const char *password); 
CORE_Bool AccountRepository_Find(AccountRepository instance, const char *login, const char *password);


#endif