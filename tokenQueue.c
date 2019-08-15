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

void resourceToken_execution() {
	for (;;) {
		if (x > 0) {
			sleep(x);
		} else {
			sleep(0);
		}

		if (R <= L) // if R is less than Max capacity of resource token queue
				{
			pthread_mutex_lock(&mutex);
			if (enqueue(resourceToken, FINITE) == SUCCESS) {

				if (resourceToken->queueSize >= R) {
					for (int tokenCounter = 1; tokenCounter <= R;
							tokenCounter++) {
						if (tokenCounter == R) {
							for (int i = 1; i <= tokenCounter; i++) {
								dequeue(resourceToken);
							}

							FLAG_tokenDequeue = TRUE;

						}
					}
				}
			}
			pthread_mutex_unlock(&mutex);
		}
	}
	pthread_exit(NULL);
} /*Resource Token Execution Task Function*/

int resource_token_thread_create(void) {
	int status = FALSE;
	status = pthread_create(&g_resource_token_handle, NULL,
			&resourceToken_execution, NULL);
	if (SUCCESS != status) {
		error_logger("FAIL : start resourceToken, status = %d", status);
		status = FALSE;
	}
	return status;
} /*Thread Create Function*/
