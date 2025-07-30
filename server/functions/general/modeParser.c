#include <string.h>
#include "modeParser.h"

MODE parseMode(const char *str) {
    if (strcmp(str, "create") == 0) {
        return MODE_CREATE;
    } else if (strcmp(str, "update") == 0) {
        return MODE_UPDATE;
    } else if (strcmp(str, "delete") == 0) {
        return MODE_DELETE;
    } else if (strcmp(str, "get") == 0) {
        return MODE_GET;
    } else {
        return MODE_UNKNOWN;
    }
}