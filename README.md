
# Generic Linked List Library (C)

This repository contains a generic linked list implementation in C. It supports both copy-by-value and copy-by-reference modes for storing data. The list is also capable of maintaining sorted order, allowing efficient insertions, retrievals, and removals.

## Features

- **Dynamic Memory Management**: Supports dynamic memory allocation for linked list nodes.
- **Sorted Insertion**: Elements are inserted into the list while maintaining a sorted order.
- **Copy Modes**: Supports both **copy-by-value** and **copy-by-reference** for element insertion.
- **Flexible Comparator**: Users can provide custom comparator functions to define sorting behavior.

## Usage

### CMake Setup (Library + Example Executable)

To build the library and the example program, you can use CMake to generate the build system. Here's an updated `CMakeLists.txt` file to configure and build the library as well as an example executable.

### Folder Structure

```
/your_project/
│
├── CMakeLists.txt         # CMake configuration file
├── list.h                 # Header file for the linked list library
├── list.c                 # Source file for the linked list library
├── list_example.c                 # Example program using the linked list
└── README.md              # This README file
```



### Build Instructions

To build the project and test the linked list library with the example program, follow these steps:

```bash
mkdir build
cd build
cmake ..
make
./list_example
```

This will generate the `generic_list` static library (`libgeneric_list.a`) and build the example executable (`list_example`), which demonstrates how to use the library.

## Example: Using the List

The `list_example.c` file demonstrates how to use the library by creating a list, inserting elements, and performing various operations like sorting and peeking at the front element.

### Example Code:

```c
#include <stdio.h>
#include "list.h"

// Comparator function for ascending order
bool int_comparator(const void *a, const void *b)
{
    int int_a = *(const int *)a;
    int int_b = *(const int *)b;
    return int_a < int_b;
}

void print_list(const list_t *list)
{
    node_t *current = list->head;
    printf("List: ");
    while (current != NULL)
    {
        printf("%d -> ", *(int *)(current->element));
        current = current->next;
    }
    printf("NULL
");
}

int main(void)
{
    list_t *my_list = NULL;
    list_status_t status;

    // Create list
    status = list_create(&my_list, sizeof(int), LIST_COPY_BY_VALUE, int_comparator);
    if (status != LIST_OK)
    {
        printf("Failed to create list. Status: %d
", status);
        return -1;
    }

    // Insert integers
    int values[] = {42, 10, 7, 99, 25};
    for (int i = 0; i < sizeof(values) / sizeof(values[0]); i++)
    {
        status = list_insert_sorted(&my_list, &values[i]);
        if (status != LIST_OK)
        {
            printf("Failed to insert %d. Status: %d
", values[i], status);
        }
    }

    // Print current list
    print_list(my_list);

    // Peek at front
    int front;
    status = list_peek_front(my_list, &front);
    if (status == LIST_OK)
    {
        printf("Peek front: %d
", front);
    }

    // Pop front repeatedly
    while ((status = list_front(&my_list, &front)) == LIST_OK)
    {
        printf("Popped: %d
", front);
        print_list(my_list);
    }

    // Clear list
    status = list_clear(&my_list);
    if (status == LIST_OK)
    {
        printf("List cleared successfully.
");
    }

    /******************* Pass by Reference (List of Pointers) ******************* */
    printf("************************************************
");
    list_t *listOfPointers = NULL;
    int arr[] = {90, -5, 6, -7, 9, 0, 100};

    status = list_create(&listOfPointers, sizeof(int *), LIST_COPY_BY_REFERENCE, int_comparator);
    if (status != LIST_OK)
    {
        printf("Failed to create list. Status: %d
", status);
    }
    
    for (int i = 0; i < sizeof(arr) / sizeof(arr[0]); i++)
    {
        status = list_insert_sorted(&listOfPointers, &arr[i]);
        if (status != LIST_OK)
        {
            printf("Failed to insert %d. Status: %d
", values[i], status);
        }
    }
    print_list(listOfPointers); // List: -7 -> -5 -> 0 -> 6 -> 9 -> 90 -> 100 -> NULL
    printf("
");

    // Update arr[0]
    arr[0] = 6; // Since it's a list of pointers, the change will reflect in the list value
    print_list(listOfPointers); // -7 -> -5 -> 0 -> 6 -> 9 -> 6 -> 100 -> NULL

    return 0;
}
```

---

## License

MIT License (or your preferred license)