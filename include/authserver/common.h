#define MAX_LOGIN_SIZE         (36)
#define MAX_PASSWORD_SIZE      (24)


typedef struct AuthenticatePayload
{
    // TODO(dhatz): UTF-8 support. 
    char                login[MAX_LOGIN_SIZE];
    char                password[MAX_PASSWORD_SIZE]; 
} AuthenticatePayload; 