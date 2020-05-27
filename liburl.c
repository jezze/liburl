#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include "liburl.h"

struct pos
{

    char *buffer;
    unsigned int buffersize;
    unsigned int bufferused;

};

static char *createstring(struct pos *pos, const char *s, const char *e, unsigned int zero)
{

    unsigned int len = e - s;
    unsigned int avail;

    if (!zero && !len)
        return 0;

    avail = pos->buffersize - pos->bufferused;

    if (len < avail)
    {

        char *out = pos->buffer + pos->bufferused;
        unsigned int i;

        for (i = 0; i < len; i++)
            out[i] = s[i];

        out[len] = '\0';
        pos->bufferused += len + 1;

        return out;

    }

    return 0;

}

static unsigned int copyout(char *buffer, unsigned int buffersize, char *s, unsigned int len, unsigned int offset)
{

    unsigned int avail = buffersize - offset;

    if (len < avail)
    {

        char *out = buffer + offset;
        unsigned int i;

        for (i = 0; i < len; i++)
            out[i] = s[i];

        out[len] = '\0';

        return len;

    }

    return 0;

}

unsigned int liburl_parse(struct liburl_url *url, void *buffer, unsigned int buffersize, const char *s, unsigned int state)
{

    const char *start = s;
    unsigned int count = strlen(s) + 1;
    struct pos pos;
    unsigned int i;

    pos.buffer = buffer;
    pos.buffersize = buffersize;
    pos.bufferused = 0;

    url->full = 0;
    url->authority = 0;
    url->scheme = 0;
    url->host = 0;
    url->port = 0;
    url->path = 0;
    url->query = 0;
    url->fragment = 0;
    url->userinfo = 0;

    for (i = 0; i < count; i++)
    {

        const char *end = s + i;
        const char *next = end + 1;
        char c = s[i];

        switch (state)
        {

        case LIBURL_SCHEME:
            url->full = 1;

            switch (c)
            {

            case '\0':
                break;

            case ':':
                url->scheme = createstring(&pos, start, end, 0);
                state = LIBURL_LIMBO1;
                start = next;

                break;

            case '+':
            case '-':
            case '.':
                break;

            default:
                if (!isalpha(c))
                    return 0;

                break;

            }

            break;

        case LIBURL_LIMBO1:
            switch (c)
            {

            case '/':
                state = LIBURL_LIMBO2;

                break;

            default:
                state = LIBURL_PATH;
                i--;

                break;

            }

            break;

        case LIBURL_LIMBO2:
            switch (c)
            {

            case '/':
                state = LIBURL_AUTHORITY;
                start = next;

                break;

            default:
                state = LIBURL_PATH;
                i--;

                break;

            }

            break;

        case LIBURL_AUTHORITY:
            url->authority = 1;

            switch (c)
            {

            case '@':
                state = LIBURL_USERINFO;
                i--;

                break;

            case '\0':
            case '/':
            case ':':
                state = LIBURL_HOST;
                i--;

                break;

            default:
                break;

            }

            break;

        case LIBURL_USERINFO:
            url->authority = 1;

            switch (c)
            {

            case '\0':
                url->userinfo = createstring(&pos, start, end, 0);

                break;

            case '@':
                url->userinfo = createstring(&pos, start, end, 0);
                state = LIBURL_HOST;
                start = next;

                break;

            default:
                break;

            }

            break;

        case LIBURL_HOST:
            url->authority = 1;

            switch (c)
            {

            case '\0':
                url->host = createstring(&pos, start, end, 0);

                break;

            case ':':
                url->host = createstring(&pos, start, end, 0);
                state = LIBURL_PORT;
                start = next;

                break;

            case '/':
                url->host = createstring(&pos, start, end, 0);
                state = LIBURL_PATH;
                start = next;

                break;

            default:
                break;

            }

            break;

        case LIBURL_PORT:
            url->authority = 1;

            switch (c)
            {

            case '\0':
                url->port = createstring(&pos, start, end, 0);

                break;

            case '/':
                url->port = createstring(&pos, start, end, 0);
                state = LIBURL_PATH;
                start = next;

                break;

            default:
                if (!isdigit(c))
                    return 0;

                break;

            }

            break;

        case LIBURL_PATH:
            switch (c)
            {

            case '\0':
                url->path = createstring(&pos, start, end, 1);

                break;

            case '?':
                url->path = createstring(&pos, start, end, 1);
                state = LIBURL_QUERY;
                start = next;

                break;

            case '#':
                url->path = createstring(&pos, start, end, 1);
                state = LIBURL_FRAGMENT;
                start = next;

                break;

            default:
                break;

            }

            break;

        case LIBURL_QUERY:
            switch (c)
            {

            case '\0':
                url->query = createstring(&pos, start, end, 1);

                break;

            case '#':
                url->query = createstring(&pos, start, end, 1);
                state = LIBURL_FRAGMENT;
                start = next;

                break;

            default:
                break;

            }

            break;

        case LIBURL_FRAGMENT:
            switch (c)
            {

            case '\0':
                url->fragment = createstring(&pos, start, end, 1);

                break;

            default:
                break;

            }

        }

    }

    if (url->full && !url->scheme)
        return 0;

    if (url->authority && !url->host)
        return 0;

    return 1;

}

unsigned int liburl_tostring(struct liburl_url *url, char *buffer, unsigned int buffersize)
{

    unsigned int offset = 0;

    if (url->scheme)
    {

        offset += copyout(buffer, buffersize, url->scheme, strlen(url->scheme), offset);
        offset += copyout(buffer, buffersize, ":", 1, offset);

    }

    if (url->authority)
    {

        offset += copyout(buffer, buffersize, "//", 2, offset);

        if (url->userinfo)
        {

            offset += copyout(buffer, buffersize, url->userinfo, strlen(url->userinfo), offset);
            offset += copyout(buffer, buffersize, "@", 1, offset);

        }

        if (url->host)
        {

            offset += copyout(buffer, buffersize, url->host, strlen(url->host), offset);

        }

        if (url->port)
        {

            offset += copyout(buffer, buffersize, ":", 1, offset);
            offset += copyout(buffer, buffersize, url->port, strlen(url->port), offset);

        }

    }

    if (url->path)
    {

        if (url->authority)
            offset += copyout(buffer, buffersize, "/", 1, offset);

        offset += copyout(buffer, buffersize, url->path, strlen(url->path), offset);

    }

    if (url->query)
    {

        offset += copyout(buffer, buffersize, "?", 1, offset);
        offset += copyout(buffer, buffersize, url->query, strlen(url->query), offset);

    }

    if (url->fragment)
    {

        offset += copyout(buffer, buffersize, "#", 1, offset);
        offset += copyout(buffer, buffersize, url->fragment, strlen(url->fragment), offset);

    }

    buffer[offset] = '\0';

    return offset;

}

