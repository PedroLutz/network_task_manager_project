#include "sync.h"

pthread_rwlock_t rwlockUsers = PTHREAD_RWLOCK_INITIALIZER;
pthread_rwlock_t rwlockTasks = PTHREAD_RWLOCK_INITIALIZER;
pthread_mutex_t mutexTasks = PTHREAD_MUTEX_INITIALIZER;