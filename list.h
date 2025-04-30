#ifndef _LIST_H_
#define _LIST_H_

#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <string.h>

/******************* Macros *******************/

#define MALLOC_PORT(SIZE) (malloc((SIZE)))
#define FREE_PORT(PTR)    (free((PTR)))

/******************* User-defined Types *******************/

typedef enum
{
    LIST_OK = 0,            // Operation successful
    LIST_NULL_POINTER,      // Null pointer passed to a function
    LIST_ALLOCATION_FAILED, // malloc or calloc failed
    LIST_EMPTY,             // List is empty
    LIST_ELEMENT_NOT_FOUND, // Element not found (during removal or search)
} list_status_t;


/**
 * @note COPY MODE BEHAVIOR:
 *
 * - LIST_COPY_BY_VALUE:
 *     + Each element's data is copied into the list's memory.
 *     + Safe from external mutations.
 *     + Slower if element size is large.
 *
 * - LIST_COPY_BY_REFERENCE:
 *     + Only stores pointers to user data.
 *     + Changes to original variables affect the list.
 *     + May break sorted order if user-modified after insertion.
 *     + Ensure original data stays alive during list lifetime.
 *
 * WARNING: In reference mode, modifying the original data after insertion may invalidate
 * the sorted order. The list does not automatically re-sort or validate order after changes.
 */
typedef enum
{
    LIST_COPY_BY_VALUE,    // Default: allocates memory, copies user data
    LIST_COPY_BY_REFERENCE // Stores pointer to user data
} list_copy_mode_t;

// Comparator function pointer type for sorting or comparisons
typedef bool (*ListComparator_t)(const void *a, const void *b);

// Node structure for singly linked list
typedef struct node
{
    void *element;
    struct node *next;
} node_t;

// Main list structure
typedef struct
{
    node_t *head;                 // Pointer to first node
    node_t *tail;                 // Pointer to last node (optional, useful for queue behavior)
    ListComparator_t Comparator; // User-defined comparator function
    size_t element_size;         // Size of each stored element in bytes
    list_copy_mode_t copy_mode;  // Copy-by-value or by-reference flag
} list_t;

/********************** Interfaces **********************/

/**
 * @brief Initializes a new linked list.
 *
 * This function allocates and initializes a list structure to store elements of a given size,
 * with the specified comparator for maintaining sorted order. The list can either copy elements
 * by value or store pointers to them (by reference).
 *
 * @param[out] list           Pointer to the list pointer (allocated internally).
 * @param[in]  element_size   Size (in bytes) of each element stored in the list.
 * @param[in]  copy_mode      Determines whether elements are copied by value or stored as pointers.
 * @param[in]  comparator     Comparator function used to maintain sorted order.
 *
 * @return list_status_t
 *         - LIST_OK if the list was successfully initialized.
 *         - LIST_NULL_POINTER if the list pointer or comparator is NULL.
 *         - LIST_ALLOCATION_FAILED if memory allocation fails.
 */
list_status_t list_create(list_t **list, size_t element_size, list_copy_mode_t copy_mode, ListComparator_t comparator);

/**
 * @brief Inserts a new element into the list while maintaining sorted order.
 *
 * If the list is empty, the element is added as both head and tail.
 * The comparator determines the position of the new element.
 *
 * @param[in,out] list     Pointer to the list.
 * @param[in]     element  Pointer to the element to insert. Its treatment depends on copy mode:
 *                         - In LIST_COPY_BY_VALUE, its data is copied.
 *                         - In LIST_COPY_BY_REFERENCE, its pointer is stored.
 *
 * @return list_status_t
 *         - LIST_OK if the element was successfully inserted.
 *         - LIST_NULL_POINTER if list or element is NULL.
 *         - LIST_ALLOCATION_FAILED if memory allocation fails.
 */
list_status_t list_insert_sorted(list_t **const list, void *element);

/**
 * @brief Removes and retrieves the first element from the list.
 *
 * If the list is not empty, the front node is removed and its value is returned through `out_element`.
 *
 * @param[in,out] list         Pointer to the list.
 * @param[out]    out_element  Output pointer for the retrieved element. 
 *                             - If copy mode is LIST_COPY_BY_VALUE, data is copied to this address.
 *                             - If copy mode is LIST_COPY_BY_REFERENCE, the stored pointer is returned directly.
 *
 * @note In LIST_COPY_BY_REFERENCE mode, `out_element` must be a `void**` to receive the pointer.
 *       In LIST_COPY_BY_VALUE mode, it must point to a memory block of `element_size` bytes.
 *
 * @return list_status_t
 *         - LIST_OK if an element was retrieved.
 *         - LIST_NULL_POINTER if list or out_element is NULL.
 *         - LIST_EMPTY if the list is empty.
 */
list_status_t list_front(list_t **const list, void *out_element);

/**
 * @brief Retrieves (without removing) the first element in the list.
 *
 * @param[in]  list          Pointer to the list.
 * @param[out] out_element   Output pointer for the front element.
 *                           - LIST_COPY_BY_VALUE: data is copied.
 *                           - LIST_COPY_BY_REFERENCE: pointer is returned.
 *
 * @note Same rules apply as in `list_front()` regarding pointer types.
 *
 * @return list_status_t
 *         - LIST_OK if an element was retrieved.
 *         - LIST_NULL_POINTER if list or out_element is NULL.
 *         - LIST_EMPTY if the list is empty.
 */
list_status_t list_peek_front(const list_t *const list, void *out_element);

/**
 * @brief Frees all nodes in the list and releases the list structure itself.
 *
 * Traverses the list, deallocating all nodes. If copy mode is LIST_COPY_BY_VALUE, the memory
 * allocated for each element is also freed. In reference mode, only node memory is freed.
 *
 * @param[in,out] list Pointer to the list pointer (freed and set to NULL on success).
 *
 * @return list_status_t
 *         - LIST_OK if the list was successfully cleared.
 *         - LIST_NULL_POINTER if the list pointer is NULL.
 */
list_status_t list_clear(list_t **list);



#endif // _LIST_H_
