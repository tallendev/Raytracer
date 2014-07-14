/*
 * Author: Tyler Allen
 * Date: 03/09/2013
 *
 * A header file for the linked_list.c file. It includes the necessary 
 * #includes, structs, and function prototypes.
 */

/* Ensures this header file can only be included once. */
#pragma once

/* The linked_list.c file makes use of the free functions from stdlib, and 
 * the Malloc wrapper function located in utils.c. Both are included by 
 * utils.h
 */
#include "utils.h"

/* Includes the obj_t typedef, which are the nodes of our linked list. */
#include "object.h"

/* The following are necessary for access to their kill functions. */
#include "light.h"
#include "projection.h"
#include "linked_list.h"
#include "sphere.h"
#include "plane.h"
#include "pplane.h"
#include "fplane.h"
#include "tplane.h"
#include "spotlight.h"
#include "cylinder.h"
#include "paraboloid.h"
#include "cone.h"
#include "hyperboloid.h"

/*
 * The list_type struct, typedefed as list_t. It is the actual linked list 
 * representation.
 *
 * Data Member: head  The head of the linked list.
 * Data Member: tail  The tail of the linked list.
 */
typedef struct list_type 
{
    obj_t* head;
    obj_t* tail;
} list_t;

list_t* list_init(void);

void list_add(list_t* list, obj_t* new);

void delete_list(list_t* list);

void delete_nodes(obj_t* node);
