#pragma once

#include <stdint.h>
#include <stdlib.h>
#include <stdio.h>

/**
 * @file queue.h
 * @brief Functions and structures for handling a queue data structure.
 */

/**
 * @brief Structure to represent a queue.
 */
typedef struct {
    uint8_t front;      /**< Index of the front element */
    uint8_t rear;       /**< Index of the rear element */
    uint8_t size;       /**< Current size of the queue */
    unsigned capacity;  /**< Maximum capacity of the queue */
    uint8_t * data;     /**< Array to store queue elements */
} Queue;

/**
 * @brief Creates a queue with the specified capacity.
 *
 * @param capacity The maximum number of elements the queue can hold.
 * @return Pointer to the created queue.
 */
Queue * initQueue(unsigned capacity);

/**
 * @brief Returns the current size of the queue.
 *
 * @param queue Pointer to the queue.
 * @return The current size of the queue.
 */
int getQueueSize(Queue * queue);

/**
 * @brief Checks if the queue is full.
 *
 * @param queue Pointer to the queue.
 * @return 1 if the queue is full, 0 otherwise.
 */
int isQueueFull(Queue * queue);

/**
 * @brief Checks if the queue is empty.
 *
 * @param queue Pointer to the queue.
 * @return 1 if the queue is empty, 0 otherwise.
 */
int isQueueEmpty(Queue * queue);

/**
 * @brief Adds an element to the rear of the queue.
 *
 * @param queue Pointer to the queue.
 * @param element The element to be added to the queue.
 */
void push(Queue * queue, uint8_t element);

/**
 * @brief Removes and returns the front element of the queue.
 *
 * @param queue Pointer to the queue.
 * @return The front element of the queue.
 */
uint8_t pop(Queue * queue);

/**
 * @brief Returns the front element of the queue without removing it.
 *
 * @param queue Pointer to the queue.
 * @return The front element of the queue.
 */
uint8_t getFront(Queue * queue);

/**
 * @brief Returns the rear element of the queue without removing it.
 *
 * @param queue Pointer to the queue.
 * @return The rear element of the queue.
 */
uint8_t getRear(Queue * queue);

/**
 * @brief Clears all elements from the queue.
 *
 * @param queue Pointer to the queue.
 */
void clearQueue(Queue * queue);
