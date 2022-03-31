/*
MIT License

Copyright (c) 2022 Christopher Irvine

Permission is hereby granted, free of charge, to any person obtaining a copy
of this software and associated documentation files (the "Software"), to deal
in the Software without restriction, including without limitation the rights
to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
copies of the Software, and to permit persons to whom the Software is
furnished to do so, subject to the following conditions:

The above copyright notice and this permission notice shall be included in all
copies or substantial portions of the Software.

THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
SOFTWARE.
*/

#include <unity.h>

#include <scds/linked_list.h>

static int compare_int_descending(const void *a, const void *b);
static bool is_not_equal_to_x(void* value, void* x);

void setUp(void) { }
void tearDown(void) { }

void test_GIVEN_linked_list_WHEN_new_THEN_memory_is_allocated_and_linked_list_is_initialised() {
    linked_list_t* list = linked_list_new();
    
    TEST_ASSERT_NOT_NULL(list);
    TEST_ASSERT_EQUAL(0, list->size);
    TEST_ASSERT_NULL(list->head);
    TEST_ASSERT_NULL(list->tail);
    
    linked_list_free(list);
}

void test_GIVEN_linked_list_WHEN_init_THEN_linked_list_in_initialised_state() {
    linked_list_t list;
    linked_list_init(&list);

    TEST_ASSERT_EQUAL(0, list.size);
    TEST_ASSERT_NULL(list.head);
    TEST_ASSERT_NULL(list.tail);
}

void test_GIVEN_linked_list_WHEN_insert_THEN_node_is_inserted() {
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

void test_GIVEN_linked_list_WHEN_insert_multiple_then_nodes_are_inserted() {
    linked_list_t list;
    linked_list_init(&list);

    int data1 = 1;
    int data2 = 2;
    int data3 = 3;
    
    linked_list_insert(&list, &data1);
    linked_list_insert(&list, &data2);
    linked_list_insert(&list, &data3);
    
    TEST_ASSERT_EQUAL(3, list.size);
    TEST_ASSERT_NOT_NULL(list.head);
    TEST_ASSERT_NOT_NULL(list.tail);
    TEST_ASSERT_EQUAL(1, *(int*)list.head->data);
    TEST_ASSERT_EQUAL(3, *(int*)list.tail->data);
}

void test_GIVEN_linked_list_WHEN_remove_THEN_node_is_removed() {
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

void test_GIVEN_linked_list_WHEN_remove_multiple_THEN_nodes_are_removed() {
    linked_list_t list;
    linked_list_init(&list);

    int data1 = 1;
    int data2 = 2;
    int data3 = 3;
    
    linked_list_insert(&list, &data1);
    linked_list_insert(&list, &data2);
    linked_list_insert(&list, &data3);
    
    TEST_ASSERT_EQUAL(3, list.size);
    TEST_ASSERT_NOT_NULL(list.head);
    TEST_ASSERT_NOT_NULL(list.tail);
    TEST_ASSERT_EQUAL(1, *(int*)list.head->data);
    TEST_ASSERT_EQUAL(3, *(int*)list.tail->data);
    
    linked_list_remove(&list, &data2);
    linked_list_remove(&list, &data3);
    
    TEST_ASSERT_EQUAL(1, list.size);
    TEST_ASSERT_NOT_NULL(list.head);
    TEST_ASSERT_NOT_NULL(list.tail);
    TEST_ASSERT_EQUAL(1, *(int*)list.head->data);
    TEST_ASSERT_EQUAL(1, *(int*)list.tail->data);
}

void test_GIVEN_linked_list_WHEN_clear_THEN_list_is_empty() {
    linked_list_t list;
    linked_list_init(&list);

    int data = 1;
    
    linked_list_insert(&list, &data);
    
    TEST_ASSERT_EQUAL(1, list.size);
    TEST_ASSERT_NOT_NULL(list.head);
    TEST_ASSERT_NOT_NULL(list.tail);
    TEST_ASSERT_EQUAL(1, *(int*)list.head->data);
    TEST_ASSERT_EQUAL(1, *(int*)list.tail->data);
    
    linked_list_clear(&list);
    
    TEST_ASSERT_EQUAL(0, list.size);
    TEST_ASSERT_NULL(list.head);
    TEST_ASSERT_NULL(list.tail);
}

void test_GIVEN_linked_list_WHEN_steal_THEN_node_is_moved() {
    linked_list_t list;
    linked_list_init(&list);

    int data1 = 1;
    int data2 = 2;
    int data3 = 3;
    
    linked_list_insert(&list, &data1);
    linked_list_insert(&list, &data2);
    linked_list_insert(&list, &data3);
    
    TEST_ASSERT_EQUAL(3, list.size);
    TEST_ASSERT_NOT_NULL(list.head);
    TEST_ASSERT_NOT_NULL(list.tail);
    TEST_ASSERT_EQUAL(1, *(int*)list.head->data);
    TEST_ASSERT_EQUAL(3, *(int*)list.tail->data);
    
    linked_list_t dest;
    linked_list_init(&dest);

    linked_list_steal(&list, &dest, 2);
    
    TEST_ASSERT_EQUAL(2, list.size);
    TEST_ASSERT_NOT_NULL(list.head);
    TEST_ASSERT_NOT_NULL(list.tail);
    TEST_ASSERT_EQUAL(1, *(int*)list.head->data);
    TEST_ASSERT_EQUAL(2, *(int*)list.tail->data);

    TEST_ASSERT_EQUAL(1, dest.size);
    TEST_ASSERT_NOT_NULL(dest.head);
    TEST_ASSERT_NOT_NULL(dest.tail);
    TEST_ASSERT_EQUAL(3, *(int*)dest.head->data);
    TEST_ASSERT_EQUAL(3, *(int*)dest.tail->data);
}

void test_GIVEN_linked_list_WHEN_remove_if_THEN_remove_if_value_is_not_equal_to_x() {
    linked_list_t list;
    linked_list_init(&list);

    int data1 = 1;
    int data2 = 2;
    int data3 = 3;
    
    linked_list_insert(&list, &data1);
    linked_list_insert(&list, &data2);
    linked_list_insert(&list, &data3);
    
    TEST_ASSERT_EQUAL(3, list.size);
    TEST_ASSERT_NOT_NULL(list.head);
    TEST_ASSERT_NOT_NULL(list.tail);
    TEST_ASSERT_EQUAL(1, *(int*)list.head->data);
    TEST_ASSERT_EQUAL(3, *(int*)list.tail->data);
    
    int x = 2;
    linked_list_remove_if(&list, &x, is_not_equal_to_x);
    
    TEST_ASSERT_EQUAL(1, list.size);
    TEST_ASSERT_NOT_NULL(list.head);
    TEST_ASSERT_NOT_NULL(list.tail);
    TEST_ASSERT_EQUAL(2, *(int*)list.head->data);
    TEST_ASSERT_EQUAL(2, *(int*)list.tail->data);
}

void test_GIVEN_linked_list_WHEN_steal_if_THEN_steal_if_value_is_not_equal_to_x() {
    linked_list_t list;
    linked_list_init(&list);

    int data1 = 1;
    int data2 = 2;
    int data3 = 3;
    
    linked_list_insert(&list, &data1);
    linked_list_insert(&list, &data2);
    linked_list_insert(&list, &data3);
    
    TEST_ASSERT_EQUAL(3, list.size);
    TEST_ASSERT_NOT_NULL(list.head);
    TEST_ASSERT_NOT_NULL(list.tail);
    TEST_ASSERT_EQUAL(1, *(int*)list.head->data);
    TEST_ASSERT_EQUAL(3, *(int*)list.tail->data);
    
    linked_list_t dest;
    linked_list_init(&dest);

    int x = 2;
    linked_list_steal_if(&list, &dest, &x, is_not_equal_to_x);
    
    TEST_ASSERT_EQUAL(1, list.size);
    TEST_ASSERT_NOT_NULL(list.head);
    TEST_ASSERT_NOT_NULL(list.tail);
    TEST_ASSERT_EQUAL(2, *(int*)list.head->data);
    TEST_ASSERT_EQUAL(2, *(int*)list.tail->data);
    
    TEST_ASSERT_EQUAL(2, dest.size);
    TEST_ASSERT_NOT_NULL(dest.head);
    TEST_ASSERT_NOT_NULL(dest.tail);
    TEST_ASSERT_EQUAL(1, *(int*)dest.head->data);
    TEST_ASSERT_EQUAL(3, *(int*)dest.tail->data);    
}

void test_GIVEN_linked_list_WHEN_sort_THEN_list_is_sorted() {
    linked_list_t list;
    linked_list_init(&list);

    int data1 = 1;
    int data2 = 2;
    int data3 = 3;
    
    linked_list_insert(&list, &data1);
    linked_list_insert(&list, &data2);
    linked_list_insert(&list, &data3);
    
    TEST_ASSERT_EQUAL(3, list.size);
    TEST_ASSERT_NOT_NULL(list.head);
    TEST_ASSERT_NOT_NULL(list.tail);
    TEST_ASSERT_EQUAL(1, *(int*)list.head->data);
    TEST_ASSERT_EQUAL(3, *(int*)list.tail->data);
    
    linked_list_sort(&list, compare_int_descending);
    
    TEST_ASSERT_EQUAL(3, list.size);
    TEST_ASSERT_NOT_NULL(list.head);
    TEST_ASSERT_NOT_NULL(list.tail);
    TEST_ASSERT_EQUAL(3, *(int*)list.head->data);
    TEST_ASSERT_EQUAL(1, *(int*)list.tail->data);
}

int main(int argc, char* argv[]) {
    UNITY_BEGIN();

    RUN_TEST(test_GIVEN_linked_list_WHEN_new_THEN_memory_is_allocated_and_linked_list_is_initialised);
    RUN_TEST(test_GIVEN_linked_list_WHEN_init_THEN_linked_list_in_initialised_state);
    RUN_TEST(test_GIVEN_linked_list_WHEN_insert_THEN_node_is_inserted);
    RUN_TEST(test_GIVEN_linked_list_WHEN_insert_multiple_then_nodes_are_inserted);
    RUN_TEST(test_GIVEN_linked_list_WHEN_remove_THEN_node_is_removed);
    RUN_TEST(test_GIVEN_linked_list_WHEN_remove_multiple_THEN_nodes_are_removed);
    RUN_TEST(test_GIVEN_linked_list_WHEN_clear_THEN_list_is_empty);
    RUN_TEST(test_GIVEN_linked_list_WHEN_steal_THEN_node_is_moved);
    RUN_TEST(test_GIVEN_linked_list_WHEN_remove_if_THEN_remove_if_value_is_not_equal_to_x);
    RUN_TEST(test_GIVEN_linked_list_WHEN_steal_if_THEN_steal_if_value_is_not_equal_to_x);
    RUN_TEST(test_GIVEN_linked_list_WHEN_sort_THEN_list_is_sorted);

    return UNITY_END();
}

bool is_not_equal_to_x(void* value, void* x) {
    return *(int*)value != *(int*)x;
}

int compare_int_descending(const void *a, const void *b) {
    int x = *(int*)a;
    int y = *(int*)b;

    if (x < y) {
        return 1;
    } 
    
    if (x > y) {
        return -1;
    }

    return 0;
}
