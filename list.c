#include "list.h"

node_t *CreateNode(const list_t *const list, void *element, list_status_t *status)
{
    node_t *node = (node_t *)MALLOC_PORT(1U * sizeof(node_t));
    if (NULL == node)
    {
        *status = LIST_ALLOCATION_FAILED;
    }
    else
    {
        if (list->copy_mode == LIST_COPY_BY_VALUE)
        {
            node->element = (void *)MALLOC_PORT(list->element_size);
            if (NULL == node->element)
            {
                *status = LIST_ALLOCATION_FAILED;
                FREE_PORT(node);
                node = NULL;
            }
            else
            {
                node->next = NULL;
                memcpy(node->element, element, list->element_size);
                *status = LIST_OK;
            }
        }
        else
        {
            node->element = element;
            *status = LIST_OK;
        }
    }

    return node;
}


list_status_t list_create(list_t **list, size_t element_size, list_copy_mode_t copy_mode, ListComparator_t comparator)
{
    list_status_t ret = LIST_EMPTY;
    if (NULL == (list) || NULL == comparator)
    {
        ret = LIST_NULL_POINTER;
    }
    else
    {
        (*list) = (list_t *)MALLOC_PORT(1U * sizeof(list_t));
        if (NULL == (*list))
        {
            ret = LIST_ALLOCATION_FAILED;
        }
        else
        {
            (*list)->element_size = element_size;
            (*list)->head = NULL;
            (*list)->tail = NULL;
            (*list)->Comparator = comparator;
            (*list)->copy_mode = copy_mode;
            ret = LIST_OK;
        }
    }
    return ret;
}


list_status_t list_insert_sorted(list_t **const list, void *element)
{
    list_status_t ret = LIST_NULL_POINTER;
    ListComparator_t comp = (*list)->Comparator;
    if (NULL == list || NULL == element)
    {
        ret = LIST_NULL_POINTER;
    }
    else
    {
        node_t *node = CreateNode(*list, element, &ret);
        if (ret == LIST_OK)
        {
            if (NULL == (*list)->head)
            {
                (*list)->head = node;
                (*list)->tail = node;
            }
            else
            {
                // insert before head
                if (true == comp(element, ((*list)->head->element)))
                {
                    node->next = (*list)->head;
                    (*list)->head = node;
                }
                // insert after tail
                else if (false == comp(element, (*list)->tail->element))
                {
                    (*list)->tail->next = node;
                    (*list)->tail = node;
                }
                else
                {
                    node_t *curr = (*list)->head;
                    for (; NULL != curr->next && false == comp(element, curr->next->element); curr = curr->next)
                        ;

                    node->next = curr->next;
                    curr->next = node;
                }
            }
        }
    }
    return ret;
}


list_status_t list_front(list_t **const list, void *out_element)
{
    list_status_t ret = LIST_NULL_POINTER;
    if (NULL == list || NULL == out_element)
    {
        ret = LIST_NULL_POINTER;
    }
    else if ((*list)->head == NULL)
    {
        ret = LIST_EMPTY;
    }
    else
    {
        node_t *temp = (*list)->head;
        if ((*list)->copy_mode == LIST_COPY_BY_VALUE)
        {
            memcpy(out_element, (*list)->head->element, (*list)->element_size);
            FREE_PORT(temp->element);
        }
        else
        {
            out_element = (*list)->head->element;
        }
        (*list)->head = (*list)->head->next;
        if ((*list)->tail == temp)
        {
            (*list)->tail = NULL;
        }
        FREE_PORT(temp);
        temp = NULL;
        ret = LIST_OK;
    }
    return ret;
}


list_status_t list_peek_front(const list_t *const list, void *out_element)
{
    list_status_t ret = LIST_NULL_POINTER;
    if (NULL == list || NULL == out_element)
    {
        ret = LIST_NULL_POINTER;
    }
    else if ((list)->head == NULL)
    {
        ret = LIST_EMPTY;
    }
    else
    {
        if (list->copy_mode == LIST_COPY_BY_VALUE)
        {
            memcpy(out_element, (list)->head->element, (list)->element_size);
        }
        else
        {
            *(void**)out_element = list->head->element;
        }
        ret = LIST_OK;
    }
    return ret;
}

list_status_t list_clear(list_t **list)
{
    list_status_t ret = LIST_NULL_POINTER;
    if (NULL == list || NULL == *list)
    {
        ret = LIST_NULL_POINTER;
    }
    else
    {
        node_t *curr = (*list)->head;
        node_t *next = NULL;

        if ((*list)->copy_mode == LIST_COPY_BY_VALUE)
        {
            for (; NULL != curr; curr = next)
            {
                next = curr->next;
                FREE_PORT(curr->element);
                FREE_PORT(curr);
            }
        }
        else
        {
            for (; NULL != curr; curr = next)
            {
                next = curr->next;
                curr->element = NULL;
                FREE_PORT(curr);
            }
        }
        (*list)->head = NULL;
        (*list)->tail = NULL;
        FREE_PORT(*list);
        *list = NULL;
        ret = LIST_OK;
    }

    return ret;
}