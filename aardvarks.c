#include "/comp/111/a/a2/anthills.h" 
#include <semaphore.h>
#include <time.h>

#define TRUE 1
#define FALSE 0

int initialized=FALSE; // semaphores and mutexes are not initialized 
int cleanup=FALSE; // sempahores and mutexes are not cleaned up

int ants_remaining[ANTHILLS];
sem_t anthill[ANTHILLS];
pthread_mutex_t mut[ANTHILLS];
struct timespec time1, time2;
int i, hillno;

/* wrapper around the provided slurp() function that 
 * locks critical sections to minimize aardvarks' 
 * sulking behavior while maximizing speed */
static void my_slurp(char aname, int hillNo) {
    pthread_mutex_lock(&mut[0]);
    if (ants_remaining[hillNo]-- > 0) {
        pthread_mutex_unlock(&mut[0]);
        if (slurp(aname, hillNo) != 1) {
            pthread_mutex_lock(&mut[1]);
            ants_remaining[hillNo]++;
            pthread_mutex_unlock(&mut[1]);
        }
    } else {
        pthread_mutex_unlock(&mut[0]);
    }
}

/* a routine for pthread_create() that puts the running 
 * thread to sleep for a little over 1 second and then 
 * calls sem_post(), which will allow aardvarks to slurp 
 * while other aardvarks still in the slurp() function that
 * is a little over 2 seconds */
static void *a_second(void *sem_to_post_to) {
    if (nanosleep(&time1, &time2) != 0) perror("nanosleep error");
    if (sem_post((sem_t *) sem_to_post_to) != 0) perror ("sem_post error");
}

void *aardvark(void *input) { 
    char aname = *(char *)input; // name of aardvark

    /* INITIALIZATION BY FIRST THREAD */
    pthread_mutex_lock(&init_lock); 
    if (!initialized++) { 
        for (i = 0; i < ANTHILLS; i++) {
            if (sem_init(&anthill[i], 0, AARDVARKS_PER_HILL) != 0) perror("sem_init error");
            if (pthread_mutex_init(&mut[i], NULL) != 0) perror("pthread_mutex_init error");
            ants_remaining[i] = ANTS_PER_HILL;
        }
        time1.tv_sec = 1;
        time1.tv_nsec = 600000000;
        hillno = 0;
    } 
    pthread_mutex_unlock(&init_lock); 

    /* WHILE THERE IS AN ANT TO EAT */
    while (chow_time()) {
        pthread_mutex_lock(&mut[2]);
        if (sem_trywait(&anthill[(hillno = rand() % ANTHILLS)]) != -1) {
            pthread_t time_thread1;
            pthread_create(&time_thread1, NULL, a_second, &anthill[hillno]);
            pthread_mutex_unlock(&mut[2]);           
            my_slurp(aname, hillno);
            pthread_join(time_thread1, NULL);
        } else {
            pthread_mutex_unlock(&mut[2]);
        }
    } 

    /* CLEAN UP BY LAST THREAD */
    pthread_mutex_lock(&init_lock); 
    if (++cleanup == AARDVARKS) {
        for (i = 0; i < ANTHILLS; i++) {
            sem_destroy(&anthill[i]);
            pthread_mutex_destroy(&mut[i]);
        }
    }
    pthread_mutex_unlock(&init_lock); 
    return NULL; 
} 
