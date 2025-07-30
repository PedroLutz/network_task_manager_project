#ifndef TASKREQHANDLER_H
#define TASKREQHANDLER_H

void handleTaskReq(char *buffer, char **response); //caller needs to initialize char *response = NULL; and then free(response);

#endif