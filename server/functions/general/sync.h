#ifndef SYNC_H
#define SYNC_H

#include <pthread.h>

extern pthread_mutex_t mutexTasks;
extern pthread_rwlock_t rwlockUsers;
extern pthread_rwlock_t rwlockTasks;

#endif