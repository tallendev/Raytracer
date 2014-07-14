/*
 * Author: Tyler Allen
 * Date: 03/09/2013
 *
 * An implementation of a very simple linked list using a list struct 
 * made up of node structs. It does not allow for insertions to anywhere 
 * but the end of the list. It does not allow for node deletions after 
 * they have been added, except when the list is being decomposed.
 */

/* This file includes utils.c for the Malloc wrapper, in addition to the 
 * function prototypes for the functions in this file. */
#include "linked_list.h"

/* 
 * A function that initializes a linked list struct.
 *
 * Return: list  The list that was just initialized.
 */
list_t* list_init(void)
{
    list_t* list = Malloc(sizeof(list_t));
    list->head   = NULL;
    list->tail   = NULL;
    list->head   = NULL;
    list->tail   = NULL;
    return list;
}


/* 
 * A function for adding a node to a list. It contains two simple cases;
 * the list passed in has no elements, and the list passed in has one or more
 * elements.
 *
 * Param: list  The list to add a node to.
 * Param: new   The new node to add to list.
 */
void list_add(list_t* list, obj_t* new)
{
    if (NULL == list->head)
    {
        list->head = new;
        list->tail = list->head;
    }
    else
    {
        list->tail->next = new;
        list->tail       = list->tail->next;
    }
}

/*
 * A function for decomposing the list when we are done with it. It calls the 
 * recursive helper function delete_nodes to decompose the inner nodes, and 
 * then frees the head of the list itself.
 *
 * Param: list  The list to be decomposed.
 */
void delete_list(list_t* list)
{
    if (list && list->head)
    {
        delete_nodes(list->head);
        free(list->head);
    }
    free(list);
}

/*
 * A recursive function for deleting the nodes of a list. The first argument
 * should be the head of the list. It will free the private data member,
 * and then call itself with the next link until it hits the end of the list,
 * repeating the process. Once it hits the end of the list, it will free the
 * "next" field every time it bumps up from the last call.
 *
 * Param: node  The node which is to be freed.
 */
void delete_nodes(obj_t* node)
{
    /* List of functions pointers of object killing functions. */
    static void (*obj_killers[])(void* obj) =
    {
        kill_light,
        kill_spotlight,
        kill_dummy,
        kill_sphere,
        kill_plane,
        kill_fplane, //finite_plane
        kill_tplane, //tiled_plane
        kill_dummy, //tex_plane
        kill_dummy, //ref_sphere
        kill_sphere, //p_sphere
        kill_plane, //pplane
        kill_parab, //parabaloid
        kill_cyl,
        kill_cone,
        kill_hyperb
    };
    if (node->priv)
    {
        obj_killers[node->objtype - FIRST_TYPE](node->priv);
    }
    if(node->next)
    {
        delete_nodes(node->next);
        free(node->next);
    }
}
