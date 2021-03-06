#pragma once

#include <stdbool.h>

struct mushroom_workqueue_node {
	void *data;
	struct mushroom_workqueue_node *next;
};

struct mushroom_workqueue {
	struct mushroom_workqueue_node *head;
	struct mushroom_workqueue_node *tail;
};

struct mushroom_workqueue *mushroom_workqueue_new();
void mushroom_workqueue_free(struct mushroom_workqueue *q);
bool mushroom_workqueue_put(struct mushroom_workqueue *q, void *data);
void *mushroom_workqueue_get(struct mushroom_workqueue *q);
void *mushroom_workqueue_peek(struct mushroom_workqueue *q);
