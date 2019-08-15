/*
 * @author: Abhinav Saxena
 *
 * @version: v1.0
 *
 * @Created on: Aug 14, 2019
 */

#include <stdlib.h>
#include <stdio.h>
#include <semaphore.h>
#include <pthread.h>
#include <stddef.h>
#include "queueFIFO.h"
#include "header.h"

// Extern Variables
extern pthread_mutex_t mutex;
extern pthread_cond_t serverWakeup;
extern int FLAG_tokenDequeue;
extern int FLAG_Q1dequeue;
extern int R;
extern queue_t *Q1;
extern queue_t *resourceToken;
extern queue_t *Q2;

// Global Variables
double time_in_Q1 = 0.00;
int counter1 = 1;
int counter2 = 1;
int counter = 1;
queueNode_t *q1Rear;
queueNode_t *q1Front;

static void *Q1_execution(void) {
	for (;;) {
		if (r > 0) {
			sleep(r);
		} else {
			sleep(0);
		}

		if (R <= L) // if R is less than Max capacity of resource token queue
				{
			pthread_mutex_lock(&mutex);
			debug_logger("r%d arrives, need %d tokens", counter, R);
			enqueue(Q1, INFINITE);
			q1Rear = rear_of_queue(Q1);
			pthread_mutex_unlock(&mutex);
			g_server_in_time = time_tick();
			debug_logger("r%d enters Q1", counter);

			if (FLAG_tokenDequeue == TRUE) {
				q1Front = front_of_queue(Q1);
				time_in_Q1 = g_current_dequeue_time - q1Front->timeEnterQ;
				pthread_mutex_lock(&mutex);
				FLAG_tokenDequeue = FALSE;
				FLAG_Q1dequeue = TRUE;
				dequeue(Q1);
				pthread_mutex_unlock(&mutex);
				debug_logger(
						"r%d leaves Q1, time in Q1 = %0.03fms, remaining token = %d",
						counter1, time_in_Q1, resourceToken->queueSize);
				counter1++;
			}

			if (FLAG_Q1dequeue == TRUE) {
				pthread_mutex_lock(&mutex);
				FLAG_Q1dequeue = FALSE;
				enqueue(Q2, INFINITE);
				pthread_mutex_unlock(&mutex);
				debug_logger("r%d enters Q2", counter2);
				counter2++;
				g_Q2_in_timer = time_tick();
				pthread_cond_broadcast(&serverWakeup); /*Server going to Wakeup Mode*/
			}
			pthread_mutex_unlock(&mutex);
		}
		counter++;
	}
	pthread_exit(NULL);
} /*Generating Request Function*/

int Q1_thread_create(void) {
	int status = FALSE;
	status = pthread_create(&g_Q1_handle, NULL, &Q1_execution, NULL);
	if (SUCCESS != status) {
		error_logger("FAIL : start Q1, status = %d", status);
		status = FALSE;
	}
	return status;
} /*Thread Create Function*/

