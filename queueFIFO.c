/*
 * @author: Abhinav Saxena
 * 
 * @version: v1.0
 *
 * @Created on: Aug 14, 2019  
 */

#include <stdlib.h>
#include <stdio.h>
#include <stddef.h>
#include "queueFIFO.h"
#include "header.h"

queue_t *create_queue() {
    queue_t * newQ = (queue_t*) calloc(1, sizeof(queue_t));
    newQ->front = NULL;
    newQ->rear = NULL;
    newQ->queueSize = 0;
    return newQ;
} /*create a new queue*/

int is_queue_empty(queue_t *queue) {
    if (queue->rear == NULL) {
        return TRUE;
    }
    return FALSE;
} /*check queue is empty or not*/

int is_queue_full(queue_t *queue) {
    if (queue->queueSize < L) {
        return FALSE;
    }
    return TRUE;
} /*check queue is full or not*/

int enqueue(queue_t *queue, int FLAG_queueType) {

    if (is_queue_full(queue) == TRUE) {
        error_logger("Queue is full - %s", __func__);
        return FAILURE;
    }

    queueNode_t *newNodeEnqueue = (queueNode_t *) calloc(1,
            sizeof(queueNode_t));

    if (newNodeEnqueue == NULL) {
        error_logger("Unable to allocate Memory - %s", __func__);
        return FAILURE;
    }

    newNodeEnqueue->packetCount = queue->queueSize + 1;
    newNodeEnqueue->timeEnterQ = time_tick();
    newNodeEnqueue->next = NULL;

    if (is_queue_empty(queue) == FALSE) {
        queue->rear->next = newNodeEnqueue;
        queue->rear = newNodeEnqueue;
    } else {
        queue->front = newNodeEnqueue;
        queue->rear = newNodeEnqueue;
    }

    queue->queueSize++;

    return SUCCESS;
} /*insert tokens in queue*/

int dequeue(queue_t *queue) {
    if (is_queue_empty(queue) == TRUE) {
        queue->queueSize = 0;
        return FAILURE;
    }
    g_current_dequeue_count = 0;
    g_current_dequeue_time = 0.00;

    g_current_dequeue_count = queue->front->packetCount;
    g_current_dequeue_time = time_tick();

    queueNode_t *newNodeDequeue = queue->front;
    queue->front = queue->front->next;

    if (queue->front == NULL) {
        queue->rear = NULL;
    }

    queue->queueSize--;

    free(newNodeDequeue);
    newNodeDequeue = NULL;

    return SUCCESS;
} /*delete tokens from queue*/

queueNode_t *front_of_queue(queue_t *queue) {
    if (is_queue_empty(queue) == TRUE) {
        return NULL;
    } else {
        return queue->front;
    }
} /*Return front of queue*/

queueNode_t *rear_of_queue(queue_t *queue) {
    if (is_queue_empty(queue) == TRUE) {
        return NULL;
    } else {
        return queue->rear;
    }
} /*Return Rear of queue*/
