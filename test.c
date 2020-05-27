#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include "liburl.h"

#define VALID                           1
#define INVALID                         2

unsigned int total;
unsigned int passed;
unsigned int failed;
unsigned int num;

static void test(unsigned int state, char *input, unsigned int result)
{

    char buffer[4096];
    char output[4096];
    struct liburl_url url;
    unsigned int count = liburl_parse(&url, buffer, 4096, input, state);

    printf("TEST (%d/%d) - ", num, total);

    if (count)
    {

        if (result == VALID)
        {

            printf("PASS (valid)\n");

            passed++;

        }

        else
        {

            printf("FAIL (valid)\n");

            failed++;

        }

    }

    else
    {

        if (result == INVALID)
        {

            printf("PASS (invalid)\n");

            passed++;

        }

        else
        {

            printf("FAIL (invalid)\n");

            failed++;

        }

    }

    switch (state)
    {

    case LIBURL_SCHEME:
        printf("    Type: Scheme\n");

        break;

    case LIBURL_PATH:
        printf("    Type: Path\n");

        break;

    }

    printf("    Input: %s\n", input);

    if (url.scheme)
        printf("    Scheme: %s\n", url.scheme);

    if (url.userinfo)
        printf("    Userinfo: %s\n", url.userinfo);
        
    if (url.host)
        printf("    Host: %s\n", url.host);

    if (url.port)
        printf("    Port: %s\n", url.port);

    if (url.path)
        printf("    Path: %s\n", url.path);

    if (url.query)
        printf("    Query: %s\n", url.query);
        
    if (url.fragment)
        printf("    Fragment: %s\n", url.fragment);

    if (liburl_tostring(&url, output, 4096))
        printf("    String: %s\n", output);

    num++;

}

static void summerize(void)
{

    printf("SUMMARY\n");
    printf("  Passed: %d/%d\n", passed, total);
    printf("  Failed: %d/%d\n", failed, total);

}

int main(int argc, char **argv)
{

    total = 30;

    /* Schema */
    test(LIBURL_SCHEME, "", INVALID);
    test(LIBURL_SCHEME, ":", INVALID);
    test(LIBURL_SCHEME, ":h", INVALID);
    test(LIBURL_SCHEME, "http//", INVALID);
    test(LIBURL_SCHEME, "http:", VALID);

    /* Limbo */
    test(LIBURL_SCHEME, "http:/", VALID);

    /* Authority */
    test(LIBURL_SCHEME, "http://", INVALID);

    /* Host */
    test(LIBURL_SCHEME, "http://www.google.com", VALID);
    test(LIBURL_SCHEME, "http://www.google.com/", VALID);

    /* Userinfo */
    test(LIBURL_SCHEME, "http://jens@www.google.com/", VALID);
    test(LIBURL_SCHEME, "http://jens:nyberg@www.google.com/", INVALID);

    /* Port */
    test(LIBURL_SCHEME, "http://www.google.com:8080/", VALID);
    test(LIBURL_SCHEME, "http://www.google.com:", VALID);
    test(LIBURL_SCHEME, "http://www.google.com:/", VALID);
    test(LIBURL_SCHEME, "http://www.google.com:a/", INVALID);

    /* Path */
    test(LIBURL_SCHEME, "http://www.google.com/index.html?", VALID);
    test(LIBURL_SCHEME, "http://www.google.com/index.html?a=b", VALID);
    test(LIBURL_SCHEME, "http://www.google.com/index.html?a=b&", VALID);
    test(LIBURL_SCHEME, "http://www.google.com/index.html?a=b&c=ddd", VALID);
    test(LIBURL_SCHEME, "http://www.google.com/index.html?a=b&c=ddd#", VALID);
    test(LIBURL_SCHEME, "http://www.google.com/index.html?a=b&c=ddd#first", VALID);

    /* Path only */
    test(LIBURL_PATH, "", VALID);
    test(LIBURL_PATH, "/", VALID);
    test(LIBURL_PATH, "index.html", VALID);
    test(LIBURL_PATH, "/index.html", VALID);
    test(LIBURL_PATH, "/index.html?a=b&c=ddd#first", VALID);
    test(LIBURL_PATH, "/index.html/", VALID);
    test(LIBURL_PATH, "/index.html/?a=b&c=ddd#first", VALID);
    test(LIBURL_PATH, "#", VALID);
    test(LIBURL_PATH, "#first", VALID);
    summerize();

    return 0;

}

