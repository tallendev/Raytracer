/*
 * Author: Tyler Allen
 * Date: 03/13/2013
 *
 * This file contains functions for initializing the model of the image of which
 * we are attempting to create.
 */

/* Header file for this source file. */
#include "model.h"

/*
 * Intializes a model struct and reads in input data to create the object
 * to be contained in the model.
 *
 * Param: in   The stream to read input data from.
 * Param: model The model which are we are writing our output to.
 *
 * Return: rc   If creation of the model was succesful.
 */
int model_init(FILE* in, model_t* model)
{
    char buff[BUFF_SIZE];
    int* objtype = Malloc(sizeof(int));
    int rc = SUCCESS;
    obj_t* obj = NULL;
    /* 
     * This codeblock continues as long as decimal number can continue to be 
     * read from the input source, and as long as rc has not indicated an error.
     */
    while(fscanf(in, " %d", objtype) != -1 && !rc)
    {
        obj= NULL;
        fgets(buff, BUFF_SIZE, in);
        obj = create_objects(objtype, obj, &rc, in);
        if (obj == NULL)
        {
            rc = FAILURE;
        }
        if(rc == SUCCESS)
        {
            if(*objtype <= LAST_LIGHT)
            {
                list_add(model->lights, obj);
            }
            else
            {
                list_add(model->scene, obj);
            }
        }
    }
    free(objtype);
    return rc;
}

/* Dummy function remove me eventually. */
obj_t* dummy_init(FILE* in, int objtype)
{
    fprintf(stderr, "Dummy_init function called.");
    void* junk = 0;
    junk = in;
    junk += objtype;
    return NULL;
}

/* 
 * This function determines which init function to call based on the 
 * object type. 
 *
 * Param: objtype The type determining what init we call.
 * Param: obj     The object to init.
 * Param: rc      The number of objects we've read in. 
 * Param: in      The file we are reading in from.
 *
 * Return: obj    The object that we just initialized.
 * */
obj_t* create_objects(int* objtype, obj_t* obj, int* rc, FILE* in)
{
    /* Static list of init_functions. */
    static obj_t* (*obj_loaders[])(FILE *in, int objtype) =
    {
        light_init,
        spotlight_init,
        dummy_init,
        sphere_init,
        plane_init,
        fplane_init, //finite_plane
        tplane_init, //tiled_plane
        dummy_init, //tex_plane
        dummy_init, //ref_sphere
        psphere_init, //p_sphere
        pplane_init,
        parab_init,
        cyl_init,
        cone_init,
        hyperb_init
    };

    if ((*objtype >= FIRST_TYPE) && (*objtype <= LAST_TYPE))
    {
        obj = (*obj_loaders[*objtype - FIRST_TYPE])(in, *objtype);
    }
    else
    {
        *rc = FAILURE;
        obj = NULL;
    }
    return obj;
}

/* 
 * This function dumps the information inside of a model, for debugging 
 * purposes. Prints all information stored in the model.
 *
 * Param: out   The stream to output to.
 * Param: model The model who's information to dump.
 */
void model_dump(FILE* out, model_t* model)
{
    fprintf(out, "Now dumping lights:\n");
    if(model->lights->head)
    {
        dump_object(out, model->lights->head);
    }
    fprintf(out, "End of Light Dump\n\n");
    fprintf(out, "Now dumping scene data:\n");
    if(model->scene->head)
    {
        dump_object(out, model->scene->head);
    }
    fprintf(stderr, "End of Scene Dump\n\n");
    fprintf(stderr, "End Data Dump\n");
}

/*
 * Helper function for dump_object, used for printing detailed information about
 * what type of object is being printed.
 *
 * Param: out   The stream to output to.
 * Param: obj   The current object to dump. 
 * Param: type  The type of object being printed.
 *
 */
void print_object(FILE* out, obj_t* obj)
{
    /* List of object dumper pointers. */
    static void (*obj_dumpers[])(FILE* out, obj_t* obj) =
    {
        dump_light,
        dump_spotlight,
        dump_dummy,
        dump_sphere,
        dump_plane,
        dump_fplane, //finite_plane
        dump_tplane, //tiled_plane
        dump_dummy, //tex_plane
        dump_dummy, //ref_sphere
        dump_sphere, //p_sphere
        dump_plane,
        dump_parab, //parabaloid
        dump_cyl,
        dump_cone,
        dump_hyperb
    };
    obj_dumpers[obj->objtype - FIRST_TYPE](out, obj);
}

/* 
 * This is a helper function for model_dump. This function recursively dumps
 * information from a list of objects using print_object.
 *
 * Param: out   The stream to output data to.
 * Param: obj   The current object to dump.
 */
void dump_object(FILE* out, obj_t* obj)
{
    char* type = NULL;
    /* Here we determine the object type to print based on the number. */
    switch(obj->objtype)
    {
        case LIGHT:        type = "Light";             break;
        case SPOTLIGHT:    type = "Spotlight";         break;
        case SPHERE:       type = "Sphere";            break;
        case P_PLANE:      type = "Procedural Plane";  break;
        case P_SPHERE:     type = "Procedural Sphere"; break;
        case PLANE:        type = "Plane";             break;
        case FINITE_PLANE: type = "Finite Plane";      break;
        case TILED_PLANE:  type = "Tiled Plane";       break;
        case CYLINDER:     type = "Cylinder";          break;
        case PARABOLOID:   type = "Paraboloid";        break;
        case CONE:         type = "Cone";              break;
        case HYPERBOLOID:  type = "Hyperboloid";       break;
    }
    fprintf(out, "\nDumping object of type %s\n", type);
    if(!is_light(obj->objtype))
    {
        print_materials(out, obj->material);
    }
    print_object(out, obj);
    if (NULL != obj->next)
    {
        dump_object(out, obj->next);
    }
}

/* 
 * Dummy dump function. This should alert if something has gone wrong.
 * Param: out  Output stream.
 * Param: obj  The object that should have been printed.
 * 
 */
void dump_dummy(FILE* out, obj_t* obj)
{
    fprintf(out, "Dummy object dump called. Something may have gone wrong.\n");
    fprintf(out, "Object type -> %d\n", obj->objtype);
}


