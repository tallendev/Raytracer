/*
 * Author: Tyler Allen
 * Date: 04/01/2013
 *
 * This file contains our #defines, function prototypes, and structs that we
 * use in the object.c source file, and other functions that require the
 * obj_t typedef.
 */

/* Ensure we only include this header file once.*/ 
#pragma once

/* An arbitrary buffer size used for operating on strings. */
#define BUFF_SIZE 256

/* The size of a Red Green Blue array. */
#define RGB_SIZE 3

/* The size of a vector of size 3. */
#define VEC_SIZE 3

/* The number of dimensions this ray tracer draws in. */
#define DIMENSIONS 3

/* X, Y, and Z as they appear as indexes in an array. */
#define X 0
#define Y 1
#define Z 2

/* Red, Green, and Blue as they appear as indexes in an array. */
#define R 0
#define G 1
#define B 2

/* The maximum value of red, green, or blue. */
#define MAX_COLORS   255

/* Our enumeration of our objects. */
#define FIRST_TYPE   10
#define LIGHT        10
#define SPOTLIGHT    11
#define PROJECTOR    12
#define SPHERE       13
#define PLANE        14
#define FINITE_PLANE 15
#define TILED_PLANE  16
#define TEX_PLANE    17
#define REF_SPHERE   18
#define P_SPHERE     19
#define P_PLANE      20
#define PARABOLOID   21
#define CYLINDER     22
#define CONE         23
#define HYPERBOLOID  24
#define LAST_TYPE    25
#define LAST_LIGHT   12

#define TEX_FIT       1
#define TEX_TILE      2

/* Success or failure for reading in files. */
#define SUCCESS 0
#define FAILURE 1

/* -1 represents a miss. */
#define MISS -1

/* The correct number of objects to be read in for materials. */
#define MATERIAL_OBJS 9

/* Arbitrary starting point for object id numbers. */
#define OBJID_INIT 100

/* Includes the wrapper Malloc function. */
#include "utils.h"

/* Forward declarion of the obj_t typedef for material.h. */

typedef struct obj_type obj_t;

/* This file includes the material_type struct necessary for the obj_t struct.
 */
#include "material.h"


/* 
 * The object_type struct, typedefed as obj_t. It represents a "node" in the 
 * linked list. 
 *
 * Data Member: next  The next node in the list.
 * Data Member: priv  The private data held by this node. It can be any type.
 * Data Member: objid This object's unique id, indicating in what order 
 *                    the objects were created.
 *
 * Function Member: hits  The hits function for the appropriate object type,
 *                        determining if a ray hits this object.
 * Function Member: getamb A plugin function for returning the ambient values 
 *                         of a non-light object.
 * Function Member: getdiff A plugin function for returning the diffuse light
 *                          values from a non-light object.
 * Function Member: getspec A plugin function for returning the specular light
 *                          values from a non-light object. 
 *
 * Data Member: material The material data contained by an object. material 
 *                       holds the diffuse, ambient, and specular light 
 *                       information.
 * Data Member: hitloc The position the object was hit at.
 * Data Member: normal The normal of the ray that hit the object. 
 *
 * Function Member: kill  The function containing instructions necessary to kill
 *                        inner information inside of priv data.
 *
 */
struct obj_type
{
    struct obj_type* next;
    int objtype;
    int objid;

    double  (*hits) (double* base, double* dir, struct obj_type*);
    void    (*getamb)(struct obj_type*, double*);
    void    (*getdiff)(struct obj_type*, double*);
    void    (*getspec)(struct obj_type*, double*);

    material_t material;

    void    *priv;

    double  hitloc[DIMENSIONS];
    double  normal[DIMENSIONS];
};

obj_t* object_init(FILE *in, int objtype);

void set_nan(obj_t* obj);

void kill_dummy(void* obj);

/* 
 * These header files contain functions we need to call but rely on the obj_type
 * structure above. 
 */
#include "sphere.h"
#include "plane.h"
#include "light.h"
#include "fplane.h"
#include "tplane.h"
#include "cylinder.h"
