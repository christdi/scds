#include <unity.h>

#include <scds/linked_list.h>

void setUp(void) { }
void tearDown(void) { }

void test_GIVEN_linked_list_when_linked_list_new_THEN_memory_is_allocated_and_linked_list_is_initialised() {
    linked_list_t* list = linked_list_new();
    
    TEST_ASSERT_NOT_NULL(list);
    TEST_ASSERT_EQUAL(0, list->size);
    TEST_ASSERT_NULL(list->head);
    TEST_ASSERT_NULL(list->tail);
    
    linked_list_free(list);
}

void test_GIVEN_linked_list_WHEN_linked_list_init_THEN_linked_list_in_initialised_state() {
    linked_list_t list;
    linked_list_init(&list);

    TEST_ASSERT_EQUAL(0, list.size);
    TEST_ASSERT_NULL(list.head);
    TEST_ASSERT_NULL(list.tail);
}

void test_GIVEN_linked_list_WHEN_linked_list_insert_THEN_node_is_inserted() {
    linked_list_t list;
    linked_list_init(&list);

    int data = 1;
    
    linked_list_insert(&list, &data);
    
    TEST_ASSERT_EQUAL(1, list.size);
    TEST_ASSERT_NOT_NULL(list.head);
    TEST_ASSERT_NOT_NULL(list.tail);
    TEST_ASSERT_EQUAL(1, *(int*)list.head->data);
    TEST_ASSERT_EQUAL(1, *(int*)list.tail->data);
}

void test_GIVEN_linked_list_WHEN_linked_list_remove_THEN_node_is_removed() {
    linked_list_t list;
    linked_list_init(&list);

    int data = 1;
    
    linked_list_insert(&list, &data);
    
    TEST_ASSERT_EQUAL(1, list.size);
    TEST_ASSERT_NOT_NULL(list.head);
    TEST_ASSERT_NOT_NULL(list.tail);
    TEST_ASSERT_EQUAL(1, *(int*)list.head->data);
    TEST_ASSERT_EQUAL(1, *(int*)list.tail->data);
    
    linked_list_remove(&list, &data);
    
    TEST_ASSERT_EQUAL(0, list.size);
    TEST_ASSERT_NULL(list.head);
    TEST_ASSERT_NULL(list.tail);
}

int main(int argc, char* argv[]) {
    UNITY_BEGIN();

    RUN_TEST(test_GIVEN_linked_list_when_linked_list_new_THEN_memory_is_allocated_and_linked_list_is_initialised);
    RUN_TEST(test_GIVEN_linked_list_WHEN_linked_list_init_THEN_linked_list_in_initialised_state);
    RUN_TEST(test_GIVEN_linked_list_WHEN_linked_list_insert_THEN_node_is_inserted);

    return UNITY_END();
}
