#include "workqueue.h"

#include "greatest.h"

TEST test_workqueue(void)
{
	struct mushroom_workqueue *queue = mushroom_workqueue_new();
	ASSERT_EQ_FMT(NULL, (void *)queue->head, "%p");

	/* no items in the queue */
	void *value = mushroom_workqueue_get(queue);
	ASSERT_EQ_FMT(NULL, value, "%p");

	/* put an item in the queue */
	int in_value = 69;
	bool success = mushroom_workqueue_put(queue, (void *)&in_value);
	ASSERT_EQ_FMT(true, success, "%d");

	/* get the next item from the queue */
	void *out_value = mushroom_workqueue_get(queue);
	/* check it's the same value */
	ASSERT_EQ_FMT(69, *(int *)out_value, "%d");
	/* check it's the same address */
	ASSERT_EQ_FMT((void *)&in_value, out_value, "%p");

	/* put a few items in */
	int in_value_1 = 70;
	success = mushroom_workqueue_put(queue, (void *)&in_value_1);
	ASSERT_EQ_FMT(true, success, "%d");
	int in_value_2 = 71;
	success = mushroom_workqueue_put(queue, (void *)&in_value_2);
	ASSERT_EQ_FMT(true, success, "%d");
	int in_value_3 = 72;
	success = mushroom_workqueue_put(queue, (void *)&in_value_3);
	ASSERT_EQ_FMT(true, success, "%d");

	/* check they come out in the correct order */
	out_value = mushroom_workqueue_get(queue);
	ASSERT_EQ_FMT(70, *(int *)out_value, "%d");
	out_value = mushroom_workqueue_get(queue);
	ASSERT_EQ_FMT(71, *(int *)out_value, "%d");
	out_value = mushroom_workqueue_get(queue);
	ASSERT_EQ_FMT(72, *(int *)out_value, "%d");

	/* queue should now be empty */
	value = mushroom_workqueue_get(queue);
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
