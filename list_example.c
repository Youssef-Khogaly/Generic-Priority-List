#include <stdio.h>
#include "list.h"

/**
 * @brief Comparator function for integers in ascending order.
 *
 * @param a Pointer to first integer.
 * @param b Pointer to second integer.
 * @return true if a < b (i.e., a comes before b in sorted list).
 */
bool int_comparator(const void *a, const void *b)
{
    int int_a = *(const int *)a;
    int int_b = *(const int *)b;
    return int_a < int_b;
}

/**
 * @brief Helper function to print contents of the list.
 *
 * Assumes the elements stored in the list are (or point to) integers.
 *
 * @param list Pointer to list.
 */
void print_list(const list_t *list)
{
    node_t *current = list->head;
    printf("List: ");
    while (current != NULL)
    {
        printf("%d -> ", *(int *)(current->element));
        current = current->next;
    }
    printf("NULL\n");
}

int main(void)
{
    list_t *my_list = NULL;
    list_status_t status;

    /******************* Example 1: Copy-by-Value *******************/
    printf("=== Copy-by-Value Example ===\n");

    status = list_create(&my_list, sizeof(int), LIST_COPY_BY_VALUE, int_comparator);
    if (status != LIST_OK)
    {
        printf("Failed to create list. Status: %d\n", status);
        return -1;
    }

    int values[] = {42, 10, 7, 99, 25};
    for (int i = 0; i < sizeof(values) / sizeof(values[0]); i++)
    {
        status = list_insert_sorted(&my_list, &values[i]);
        if (status != LIST_OK)
        {
            printf("Failed to insert %d. Status: %d\n", values[i], status);
        }
    }

    print_list(my_list); // Expected: 7 -> 10 -> 25 -> 42 -> 99 -> NULL

    // Peek at front (does not remove element)
    int front;
    status = list_peek_front(my_list, &front);
    if (status == LIST_OK)
    {
        printf("Peek front (copy-by-value): %d\n", front); // Expected: 7
    }

    // Pop and print elements
    while ((status = list_front(&my_list, &front)) == LIST_OK)
    {
        printf("Popped: %d\n", front);
        print_list(my_list);
    }

    status = list_clear(&my_list);
    if (status == LIST_OK)
    {
        printf("List cleared successfully.\n");
    }

    /******************* Example 2: Copy-by-Reference *******************/
    printf("\n=== Copy-by-Reference Example ===\n");

    list_t *listOfPointers = NULL;
    int arr[] = {90, -5, 6, -7, 9, 0, 100};

    status = list_create(&listOfPointers, sizeof(int *), LIST_COPY_BY_REFERENCE, int_comparator);
    if (status != LIST_OK)
    {
        printf("Failed to create list. Status: %d\n", status);
        return -1;
    }

    for (int i = 0; i < sizeof(arr) / sizeof(arr[0]); i++)
    {
        status = list_insert_sorted(&listOfPointers, &arr[i]);
        if (status != LIST_OK)
        {
            printf("Failed to insert %d. Status: %d\n", arr[i], status);
        }
    }

    print_list(listOfPointers); // Expected: -7 -> -5 -> 0 -> 6 -> 9 -> 90 -> 100 -> NULL

    // Peek front in reference mode
    int *front_ptr;
    status = list_peek_front(listOfPointers, &front_ptr);
    if (status == LIST_OK)
    {
        printf("Peek front (copy-by-reference): %d\n", *front_ptr); // Expected: -7
    }

        // Mutate arr[0] (which was 90)
    arr[0] = 6;
    print_list(listOfPointers); // Now: -7 -> -5 -> 0 -> 6 -> 9 -> 6 -> 100 -> NULL

    list_clear(&listOfPointers);
    return 0;
}
