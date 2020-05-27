# About

This is a simple C library for parsing URLs. Just copy the liburl.h and
liburl.c file to your project to get started.

This library does not use any malloc. Each function takes a buffer.

## Examples

To parse a complete URL:

    char *input = "http://www.github.com/jezze/liburl";
    struct liburl_url url;
    char buffer[4096];

    liburl_parse(&url, buffer, 4096, input, LIBURL_SCHEME);

To parse only a part of a URL:

    char *input = "/jezze/liburl";
    struct liburl_url url;
    char buffer[4096];

    liburl_parse(&url, buffer, 4096, input, LIBURL_PATH);

After you have parsed the URL you can print it's parts:

    printf("Scheme: %s\n", url.scheme);
    printf("Host: %s\n", url.host);

You can also write the full url to a buffer:

    char buffer[4096];

    liburl_tostring(&url, buffer, 4096);
    printf("URL: %s\n", buffer);

## Missing features

* It ignores any ':' in the userinfo part of the URL. This is because of
  two reasons, it should not be used and secondly it made parsing easier
  because it did not conflict with finding the port number.

* Right now I don't really care about what characters are being used
  except for the scheme. This can easily be added if somebody just knows
  what characters are allowed for the different parts.

Have fun!
