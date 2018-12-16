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

static bool queue_is_empty(struct mushroom_workqueue *q)
{
	return (q->head == NULL);
}

bool mushroom_workqueue_put(struct mushroom_workqueue *q, void *data)
{
	struct mushroom_workqueue_node *new_node = malloc(sizeof(new_node));

	new_node->data = data;
	new_node->next = NULL;

	if (queue_is_empty(q)) {
		q->head = new_node;
		q->tail = new_node;
	} else {
		q->tail->next = new_node;
		q->tail = new_node;
	}

	return true;
}

void *mushroom_workqueue_get(struct mushroom_workqueue *q)
{
	if (queue_is_empty(q)) {
		return NULL;
	}

	struct mushroom_workqueue_node *old_head = q->head;
	void *data = old_head->data;
	q->head = old_head->next;
	free(old_head);

	return data;
}