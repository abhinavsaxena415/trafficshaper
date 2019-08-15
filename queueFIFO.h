/*
 * @author: Abhinav Saxena
 *
 * @version: v1.0
 *
 * @Created on: Aug 14, 2019
 */

#ifndef QUEUE_FIFO_H_
#define QUEUE_FIFO_H_

typedef struct queue_node {
  struct queue_node *next;
  double timeEnterQ;
  int packetCount;
} queueNode_t;

typedef struct queue {
  queueNode_t *front;
  queueNode_t *rear;
  int queueSize;
} queue_t;

enum {
  INFINITE = 1, FINITE = 0
};

/*function protypes*/
queue_t *create_queue();
int is_queue_empty(queue_t *queue);
int is_queue_full(queue_t *queue);
int enqueue(queue_t *queue, int FLAG_queueType);
int dequeue(queue_t *queue);
queueNode_t *front_of_queue(queue_t *queue);
queueNode_t *rear_of_queue(queue_t *queue);

#endif /*QUEUE_FIFO_H_*/
