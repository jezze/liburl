#define LIBURL_SCHEME                   1
#define LIBURL_LIMBO1                   2
#define LIBURL_LIMBO2                   3
#define LIBURL_AUTHORITY                4
#define LIBURL_USERINFO                 5
#define LIBURL_HOST                     6
#define LIBURL_PORT                     7
#define LIBURL_PATH                     8
#define LIBURL_QUERY                    9
#define LIBURL_FRAGMENT                 10

struct liburl_url
{

    unsigned int full;
    unsigned int authority;
    char *scheme;
    char *userinfo;
    char *host;
    char *port;
    char *path;
    char *query;
    char *fragment;

};

unsigned int liburl_parse(struct liburl_url *url, void *buffer, unsigned int buffersize, const char *s, unsigned int state);
unsigned int liburl_tostring(struct liburl_url *url, char *buffer, unsigned int buffersize);
