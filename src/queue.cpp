#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <pthread.h>
#include <string.h>

#include "queue.h"

// q_create() - create an empty queue
q_queue_t * q_create(size_t size)
{
    // crea la coda
    q_queue_t *queue = (q_queue_t*)malloc(sizeof(q_queue_t));

    // inizializza la coda
    queue->head = NULL;
    queue->tail = NULL;
    queue->size = size;
    pthread_mutex_init(&queue->mutex, NULL);

    return queue;
}

// q_enqueue() - adds an item to the queue
bool q_enqueue(q_queue_t * queue, q_event_t event, const void * payload)
{
    q_node_t * new_node = (q_node_t *)malloc(sizeof(q_node_t));

    new_node->event = event;
    new_node->next = NULL;
    new_node->payload = NULL;

    if (payload != NULL) {
        new_node->payload = malloc(queue->size);
        memcpy(new_node->payload, payload, queue->size);
    }

    // block access
    pthread_mutex_lock(&queue->mutex);

    // if queue empty
    if (queue->head == NULL) {
        // front and rear are the same
        queue->tail = queue->head = new_node;
    }
    else {
        queue->tail->next = new_node;
        queue->tail = new_node;
    }

    // unlock access
    pthread_mutex_unlock(&queue->mutex);
    return true;
}

// q_dequeue() - removes element from queue
bool q_dequeue(q_queue_t * queue, q_node_t * node)
{
    // blocco l'accesso
    pthread_mutex_lock(&queue->mutex);

    q_node_t * head = queue->head;
    if (head == NULL) {
        // sblocco l'accesso ed esco
        pthread_mutex_unlock(&queue->mutex);
        return false;
    }

    // create a clone object
    node->event = head->event;
    node->next = NULL;
    node->payload = NULL;

    if (head->payload != NULL){
        node->payload = malloc(queue->size);
        memcpy(node->payload, head->payload, queue->size);
    }

    // go ahead
    queue->head = head->next;
    free(head->payload);
    free(head);

    // unlock access
    pthread_mutex_unlock(&queue->mutex);

    return true;
}
