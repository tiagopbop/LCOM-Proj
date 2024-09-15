#include "queue.h"

Queue * initQueue(unsigned capacity) {
    Queue * queue = (Queue *) malloc(sizeof(Queue));
    queue->front = 0;
    queue->rear = -1;
    queue->size = 0;
    queue->capacity = capacity;
    
    queue->data = (uint8_t *) malloc(capacity * sizeof(uint8_t));

    return queue;
}

int getQueueSize(Queue * queue) {
    return queue->size;
}

int isQueueFull(Queue * queue) {
    return queue->size == (int) queue->capacity;
}

int isQueueEmpty(Queue * queue) {
    return queue->size == 0;
}

void push(Queue * queue, uint8_t item) {
    if (isQueueFull(queue)) {
        return;
    }

    queue->rear = (queue->rear + 1) % queue->capacity;
    queue->data[queue->rear] = item;
    queue->size++;
}

uint8_t pop(Queue * queue) {
    if (isQueueEmpty(queue)) {
        return 0;
    }

    uint8_t removedItem = queue->data[queue->front];
    queue->front = (queue->front + 1) % queue->capacity;
    queue->size--;

    return removedItem;
}

uint8_t getFront(Queue * queue) {
    if (isQueueEmpty(queue)) {
        return 0;
    }

    return queue->data[queue->front];
}

uint8_t getRear(Queue * queue) {
    if (isQueueEmpty(queue)) {
        return 0;
    }

    return queue->data[queue->rear];
}

void clearQueue(Queue * queue) {
    free(queue->data);
    free(queue);
}
