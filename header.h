/*
 * @author: Abhinav Saxena
 * 
 * @version: v1.0
 *
 * @Created on: Aug 14, 2019  
 */

#ifndef HEADER_H_
#define HEADER_H_

#include <time.h>

#define TRUE		1
#define FALSE		0
#define SUCCESS		0
#define FAILURE		-1

#define REQ_ARRIVAL_RATE		"-r"
#define FINITE_BUFFER 			"-L"
#define TOKENS_TO_SERVE_REQ		"-R"
#define REQ_SERVING_TIME		"-t"
#define RESOURCE_TOKEN_RATE		"-x"
#define INVALID_VALUE			"0"

#define debug_logger(msg, ...) printf("[DEBUG] [%s:%d] " msg "\n",__FILE__, __LINE__, ##__VA_ARGS__)
#define error_logger(msg, ...) printf("[ERROR] [%s:%d] " msg "\n",__FILE__, __LINE__,##__VA_ARGS__)
#define info_logger(msg, ...) printf("[INFO] [%s:%d] " msg "\n",__FILE__, __LINE__, ##__VA_ARGS__)

pthread_t g_resource_token_handle;
pthread_t g_Q1_handle;
pthread_t g_Q2_handle;
pthread_t g_shutdown_handle;

double g_server_in_time;
double g_current_dequeue_time;
double g_Q2_in_timer;
double g_Q2_out_timer;
int g_current_dequeue_count;

int L;  /*Maximum Number of tokens in token queue*/
int R;  /*Deterministic Resource Requirement*/
int r;  /*Rate of Requests(sec)*/
int t;  /*Serving Time at Server(sec)*/
int x;  /*Rate of Token Creation(sec)*/

double time_tick(void);

#endif /*HEADER_H_*/
