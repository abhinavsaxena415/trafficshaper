/*
 * @author: Abhinav Saxena
 *
 * @version: v1.0
 *
 * @Created on: Aug 14, 2019
 */

#include <stdlib.h>
#include <stdio.h>
#include <time.h>
#include <semaphore.h>
#include <pthread.h>
#include <stddef.h>
#include "queueFIFO.h"
#include "header.h"

extern pthread_mutex_t mutex;
extern pthread_cond_t serverWakeup;
extern int FLAG_tokenDequeue;
extern int FLAG_Q1dequeue;
extern queue_t *resourceToken;
extern queue_t *Q2;

queueNode_t *q2Rear;
queueNode_t *q2Front;

double t1 = 0.00;
double t2 = 0.00;
int FLAG_waitServing = FALSE;

void Q2_execution() {
    double time_in_Q2 = 0.00;
    int cntr = 1;
    for (;;) {
        if (is_queue_empty(Q2) == TRUE) {
            pthread_mutex_lock(&mutex);
            pthread_cond_wait(&serverWakeup, &mutex); /*Server going to Sleep Mode*/
            pthread_mutex_unlock(&mutex);
        }

        pthread_mutex_lock(&mutex);
        dequeue(Q2);
        pthread_mutex_unlock(&mutex);
        g_Q2_out_timer = time_tick();
        time_in_Q2 = g_Q2_out_timer - g_Q2_in_timer;
        debug_logger(
                "r%d leaves Q2, time in Q2 = %'.3fms, remaining token = %d",
                cntr, time_in_Q2, resourceToken->queueSize);

        t1 = time_tick();
        debug_logger("r%d begins service at S, requesting %dms of service",
                cntr, t * 1000);

        t2 = time_tick();
        debug_logger(
                "r%d departs from S, service time = %'.3fms, time in system = %'.3fms",
                cntr, (t2 - t1) + (t * 1000), (t2 - g_server_in_time)+ (t * 1000));
        cntr++;
        sleep(t);
    }

    pthread_exit(NULL);
} /*Serving Requests at Server Function*/

int Q2_thread_create(void) {
    int status = FALSE;
    status = pthread_create(&g_Q2_handle, NULL, &Q2_execution, NULL);
    if (SUCCESS != status) {
        error_logger("FAIL : start Q2, status = %d", status);
        status = FALSE;
    }
    return status;
} /*Thread Create Function*/
