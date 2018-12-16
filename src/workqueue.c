#include <stdlib.h>

#include "log.h"
#include "workqueue.h"

struct mushroom_workqueue *mushroom_workqueue_new()
{
	struct mushroom_workqueue *queue = malloc(sizeof(*queue));

	queue->head = NULL;
	queue->tail = NULL;

	mushroom_log_debug("created mushroom_workqueue: %p", queue);

	return queue;
}

void *mushroom_workqueue_get(struct mushroom_workqueue *q)
{
	/* queue is empty */
	if (q->head == NULL) {
		return NULL;
	}

    q->head = q->head->next;
    return q->head;
}