#ifndef MODEPARSER_H
#define MODEPARSER_H

typedef enum {
    MODE_UNKNOWN,
    MODE_CREATE,
    MODE_UPDATE,
    MODE_GET,
    MODE_DELETE
} MODE;

MODE parseMode(const char *str);

#endif