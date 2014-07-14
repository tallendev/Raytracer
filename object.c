/*
 * Author: Tyler Allen
 * Date: 03/11/2013
 * 
 * Source file representing an object. Objects are also the nodes in the 
 * list_t structure.
 */

/* Header file for this source file. */
#include "object.h"


/* 
 * Initializes an object struct. 
 *
 * Param: in  The input stream to read from.
 * Param: objtype  The type of object being created. Only necessary for setting
 *                 the field. 
 *
 * Return: obj  The object that has just been created and initialized.
 */
obj_t* object_init (FILE *in, int objtype)
{
    /* Static counter of how many objects have been created. */
    static int objid = OBJID_INIT;
    obj_t* obj = Malloc(sizeof(obj_t));
    obj->next = NULL;
    obj->objtype = objtype;
    obj->objid = objid;
    objid++; /*increments objid by 1.*/
    obj->priv = NULL;
    set_nan(obj);
    if (!is_light(objtype))
    {
        int rc = material_load(in, &obj->material);
        if (rc != SUCCESS)
        {
            fprintf(stderr, "Error found in obj_init...");
            obj = NULL;
            objtype = 0;
        }
        else
        {
            obj->getamb = default_getamb;
            obj->getdiff = default_getdiff;
            obj->getspec = default_getspec;
        }
    }
    else
    {
        obj->hits = NULL;
        obj->getamb = NULL;
        obj->getdiff = NULL;
        obj->getspec = NULL;
    }
    return obj;
}

void set_nan(obj_t* obj)
{
    obj->hitloc[X] = FP_NAN; 
    obj->hitloc[Y] = FP_NAN; 
    obj->hitloc[Z] = FP_NAN;
    obj->normal[X] = FP_NAN; 
    obj->normal[Y] = FP_NAN; 
    obj->normal[Z] = FP_NAN; 
}

void kill_dummy(void* obj)
{
    obj_t* object = (obj_t*)obj;
    fprintf(stderr, "Kill_dummy called. This may indicated a problem.\n"
                    "Objtype = %d", object->objtype);
}
