#ifndef USERREQHANDLER_H
#define USERREQHANDLER_H

void handleUserReq(char *buffer, char **response); //caller needs to initialize char *response = NULL; and then free(response);

#endif