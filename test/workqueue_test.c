#include "workqueue.h"

#include "greatest.h"

TEST test_workqueue(void)
{
	struct mushroom_workqueue *queue = mushroom_workqueue_new();
	ASSERT_EQ_FMT(NULL, (void *)queue->head, "%p");

	/* no items in the queue */
	void *value = mushroom_workqueue_get(queue);
	ASSERT_EQ_FMT(NULL, value, "%p");

	PASS();
}

SUITE(workqueue_suite)
{
	RUN_TEST(test_workqueue);
}

GREATEST_MAIN_DEFS();

int main(int argc, char **argv)
{
	GREATEST_MAIN_BEGIN();
	RUN_SUITE(workqueue_suite);

	GREATEST_MAIN_END();
}
