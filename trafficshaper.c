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
#include <signal.h>

sigset_t set;

extern pthread_mutex_t mutex = PTHREAD_MUTEX_INITIALIZER;
extern pthread_cond_t serverWakeup = PTHREAD_COND_INITIALIZER;
extern queue_t *Q1 = NULL;
extern queue_t *Q2 = NULL;
extern queue_t *resourceToken = NULL;
extern int FLAG_tokenDequeue = FALSE;
extern int FLAG_Q1dequeue = FALSE;

int Q1_thread_create(void);
int resource_token_thread_create(void);
int Q2_thread_create(void);

double time_tick() {
	struct timespec time_monotonic;
	clock_gettime(CLOCK_MONOTONIC_RAW, &time_monotonic);
	return ((double) time_monotonic.tv_nsec / 1000000);
} /*Timer Function*/

void *shutdown_traffic_shaper() {
	int sig = 0;
	sigwait(&set, &sig);
	info_logger("emulation end");
	pthread_mutex_lock(&mutex);
	pthread_cancel(g_Q1_handle);
	pthread_cancel(g_Q2_handle);
	pthread_cancel(g_resource_token_handle);
	pthread_mutex_unlock(&mutex);
	pthread_sigmask(SIG_UNBLOCK, &set, NULL);
	pthread_exit(NULL);
	exit(0);
	return;
} /*Gracefully Shutdown when ctrl+c pressed*/

int main(int argc, char *argv[]) {

	sigemptyset(&set);
	sigaddset(&set, SIGINT);
	sigprocmask(SIG_BLOCK, &set, 0);

	/*Default Values*/
	L = 50; /*Maximum Number of tokens in token queue*/
	R = 5; /*Deterministic Resource Requirement*/
	r = 2; /*Rate of Requests(sec)*/
	t = 5; /*Serving Time at Server(sec)*/
	x = 1; /*Rate of Token Creation(sec)*/

	/*Command line arguments*/
	for (int i = 0; i < argc; i++) {
		if (i % 2 != SUCCESS) {
			if (argv[i][0] == '-') {
				if (strcmp(argv[i], REQ_ARRIVAL_RATE) == SUCCESS) //r
						{
					if (argv[i + 1] == NULL) {
						error_logger(
								"r is NULL, Enter Request Arrival Rate > Zero(0)");
						exit(FAILURE);
					}
					r = atof(argv[i + 1]);
					if (r == SUCCESS) {
						if (strcmp(INVALID_VALUE, argv[i + 1]) == SUCCESS) {
							error_logger(
									"Request Arrival Rate cannot be Zero(0)");
							exit(FAILURE);
						} else {
							error_logger("Invalid Value entered for r");
							exit(FAILURE);
						}
					}
					if (r < SUCCESS) {
						error_logger("r cannot be less than Zero(0)");
						exit(FAILURE);
					}
				}

				else if (strcmp(argv[i], REQ_SERVING_TIME) == SUCCESS) //t
						{
					if (argv[i + 1] == NULL) {
						error_logger(
								"t is NULL, Enter Request Serving Time > Zero(0)");
						exit(FAILURE);
					}
					t = atof(argv[i + 1]);
					if (t == SUCCESS) {
						if (strcmp(INVALID_VALUE, argv[i + 1]) == SUCCESS) {
							error_logger(
									"Request Serving Time cannot be Zero(0)");
							exit(FAILURE);
						} else {
							error_logger("Invalid Value entered for t");
							exit(FAILURE);
						}
					}
					if (t < SUCCESS) {
						error_logger("t cannot be less than Zero(0)");
						exit(FAILURE);
					}
				}

				else if (strcmp(argv[i], RESOURCE_TOKEN_RATE) == SUCCESS) //x
						{
					if (argv[i + 1] == NULL) {
						error_logger(
								"x is NULL, Enter Resource Token Rate > Zero(0)");
						exit(FAILURE);
					}
					x = atof(argv[i + 1]);
					if (x == SUCCESS) {
						if (strcmp(INVALID_VALUE, argv[i + 1]) == SUCCESS) {
							error_logger(
									"Resource Token Rate cannot be Zero(0)");
							exit(FAILURE);
						} else {
							error_logger("Invalid Value entered for x");
							exit(FAILURE);
						}
					}
					if (x < SUCCESS) {
						error_logger("x cannot be less than Zero(0)");
						exit(FAILURE);
					}
				}

				else if (strcmp(argv[i], FINITE_BUFFER) == SUCCESS)  //L
						{
					if (argv[i + 1] == NULL) {
						error_logger(
								"L is NULL, Enter Token Queue Size > Zero(0)");
						exit(FAILURE);
					}
					L = atoi(argv[i + 1]);
					if (L == SUCCESS) {
						if (strcmp(INVALID_VALUE, argv[i + 1]) == SUCCESS) {
							error_logger("Token Queue size cannot be Zero(0)");
							exit(FAILURE);
						} else {
							error_logger("Invalid Value entered for L");
							exit(FAILURE);
						}
					}
					if (L < SUCCESS) {
						error_logger("L cannot be less than Zero(0)");
						exit(FAILURE);
					}
				}

				else if (strcmp(argv[i], TOKENS_TO_SERVE_REQ) == SUCCESS) //R
						{
					if (argv[i + 1] == NULL) {
						error_logger(
								"R is NULL, Enter Token to serve request > Zero(0)");
						exit(FAILURE);
					}
					R = atoi(argv[i + 1]);
					if (R == SUCCESS) {
						if (strcmp(INVALID_VALUE, argv[i + 1]) == SUCCESS) {
							error_logger(
									"Token to serve request cannot be ZERO(0)");
							exit(FAILURE);
						} else {
							error_logger("Invalid Value entered for R");
							exit(FAILURE);
						}
					}
					if (R < SUCCESS) {
						error_logger("R cannot be less than Zero(0)");
						exit(FAILURE);
					}
				} else {
					error_logger(
							"Command Line Argument is Invalid\n"
									"Only accept arguments in format and Valid values respectively:\n"
									"-r r, -R R, -L L, -x x, -t t \n"
									"Quit Execution\n");
					exit(FAILURE);
				}
			} else {
				error_logger(
						"Invalid Command line arguments, Use valid argument[-r r, -R R, -L L, -x x, -t t]");
				exit(FAILURE);
			}
		}
	}
	if (R > L) {
		error_logger("R Cannot be greater than Maximum Finite Token");
		exit(0);
	}
	info_logger("emulation begin\r");
	Q1 = create_queue(); /*Creating Request queue*/
	Q2 = create_queue(); /*Creating Serving queue */
	resourceToken = create_queue(); /*Creating Resource token queue*/

	/*Thread Creation*/
	Q1_thread_create();
	resource_token_thread_create();
	Q2_thread_create();
	pthread_create(&g_shutdown_handle, NULL, shutdown_traffic_shaper, NULL);

	/*Thread Join*/
	pthread_join(g_Q1_handle, NULL);
	pthread_join(g_resource_token_handle, NULL);
	pthread_join(g_Q2_handle, NULL);

	return 0;
} /*Main Function*/
