/*
 * Author: Tyler Allen
 * Date: 03/14/2013
 *
 * This source file contains functions that operate on light objects. Light
 * objects are obj_ts that have sphere_t's in the private data of the object.
 * This file is not used in our first version of the ray tracer!
 */

/* The header file for this source file. */
#include "light.h"

/*
 * Initializes an obj_t struct, and then a light_t struct to store inside of
 * the private data of the obj_t.
 *
 * Param: in   The stream to read in struct information from.
 * Param: objtype   The type of the object being created.
 *
 * Return: obj   The object that was just constructed and initialized.
 */
obj_t* light_init(FILE* in, int objtype)
{
    char buff[BUFF_SIZE];
    obj_t* obj = NULL;
    int pcount = 0;
    obj = object_init(in, objtype);
    light_t* light = Malloc(sizeof(light_t));
    pcount += fscanf(in, "%lf %lf %lf",
                     &(light->emissivity[X]),
                     &(light->emissivity[Y]),
                     &(light->emissivity[Z]));
    fgets(buff, BUFF_SIZE, in);
    pcount += fscanf(in, "%lf %lf %lf",
                     &(light->location[X]),
                     &(light->location[Y]),
                     &(light->location[Z]));
    fgets(buff, BUFF_SIZE, in);
    if (pcount != LIGHT_OBJS || obj == NULL)
    {
        fprintf(stderr, "Error found in light_init...");
        obj = NULL;
    }
    else
    {
        obj->priv = (void*)light;
        light->getemiss = default_getemiss;
        light->illum_check = NULL;
    }
    return obj;
}

/*
 * Copies the emissivity from a light to an output vector.
 * Param: light  The light to copy emissivity from.
 * Param: value  The output vector.
 */
void default_getemiss(light_t* light, double* value)
{
    copy3(light->emissivity, value);
}

/*
 * Function for dumping the data of this object back out.
 * Param: out  The output stream to write to.
 * Param: obj  The object to dump.
 */
void dump_light(FILE* out, obj_t* obj)
{
    fprintf(out, "\nLight Data\n");
    light_t* light = obj->priv;
    fprintf(out, "\nLocation:   %lf  %lf  %f\n",
            light->location[X], 
            light->location[Y],
            light->location[Z]);
    fprintf(out, "Emissivity: %lf %lf %lf \n",
            light->emissivity[R],
            light->emissivity[G],
            light->emissivity[B]);
}

/* 
 * Function for allowing a light to kill itself.
 *
 * Param: light  The priv data of an obj_t of type light.
 */
void kill_light(void* light)
{
    free(light);
}
