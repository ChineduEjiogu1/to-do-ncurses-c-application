#include <stdlib.h>
#include <stdbool.h>
#include <stdio.h>
#include "doubly_linked_list.h"
#include "doubly_linked_list_api.c"
#define MAX_CAPACITY 1000

int main()
{
    DoublyLinkedList *list = create_list(MAX_CAPACITY);

    struct Data *data1 = (struct Data *)malloc(sizeof(struct Data));
    data1->value = (void *)1;

    struct Data *data2 = (struct Data *)malloc(sizeof(struct Data));
    data2->value = (void *)2;

    struct Data *data3 = (struct Data *)malloc(sizeof(struct Data));
    data3->value = (void *)3;

    struct Data *data4 = (struct Data *)malloc(sizeof(struct Data));
    data4->value = (void *)4;

    struct Data *data5 = (struct Data *)malloc(sizeof(struct Data));
    data5->value = (void *)5;

    struct Data *data6 = (struct Data *)malloc(sizeof(struct Data));
    data6->value = (void *)6;

    struct Data *data7 = (struct Data *)malloc(sizeof(struct Data));
    data7->value = (void *)7;

    struct Data *data8 = (struct Data *)malloc(sizeof(struct Data));
    data8->value = (void *)8;

    struct Data *data9 = (struct Data *)malloc(sizeof(struct Data));
    data9->value = (void *)9;

    struct Data *data10 = (struct Data *)malloc(sizeof(struct Data));
    data10->value = (void *)10;

    struct Data *data11 = (struct Data *)malloc(sizeof(struct Data));
    data11->value = (void *)11;

    insert_by_position(list, 0, data1);
    insert_by_position(list, 1, data3);
    insert_by_position(list, 2, data2);
    insert_by_position(list, 3, data4);
    insert_by_position(list, 4, data5);

    printf("Original list before any additions:\n");
    traverse_forward(list);

    Node *first_node = list->head;
    Node *second_node = list->head->next;
    Node *fourth_node = list->head->next->next->next;
    Node *fifth_node = list->head->next->next->next->next;

    insert_after(list, second_node, data7);
    printf("After inserting 7 after second node: \n");
    traverse_forward(list);

    insert_after(list, fifth_node, data6);
    printf("After inserting 6 after fifth node: \n");
    traverse_forward(list);

    insert_before(list, first_node, data8);
    printf("After inserting 8 at the head: \n");
    traverse_forward(list);

    insert_before(list, fourth_node, data9);
    printf("After inserting 9 before fourth node: \n");
    traverse_forward(list);

    int target_value = 3;
    list->head = delete_by_value(list, target_value);
    printf("After deleting node with value %d: \n", target_value);
    traverse_forward(list);

    int target_value_2 = 5;
    list->head = delete_by_value(list, target_value_2);
    printf("After deleting node with value %d: \n", target_value_2);
    traverse_forward(list);

    insert_front(list, data10);
    printf("Inserting 10 in front of the list: \n");
    traverse_forward(list);

    insert_back(list, data11);
    printf("Inserting 11 at the back of the list: \n");
    traverse_forward(list);

    printf("Testing is_empty on empty list: %s\n",
           is_empty(list) ? "true" : "false");

    // Test is_full on empty list
    printf("Testing is_full on empty list: %s\n",
           is_full(list) ? "true" : "false");

    printf("List size is: %d\n", list_size(list));

    free_list(list);
    return 0;
}