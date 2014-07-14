/*
 * @Author: Tyler Allen
 * @Date: 04/06/2013
 *
 * This file contains functions for initializing and changing a procedural
 * sphere object. We use the obj_t typedef and the sphere_t typedef as a basis
 * for this.
 */

/* Header for this source file. */
#include "psphere.h"

/*
 * This function is used to initialize a procedural plane sphere. We utilize
 * the sphere_init function to give us a basis for our procedural sphere.
 * 
 * Param: in  The file we are currently reading data from. 
 * Param: objtype  The type of object we are reading in so that we can send it
 *                 to plane_init.
 */
obj_t* psphere_init(FILE* in, int objtype)
{
    /* Static array containing shaders. */
    static void (*sphere_shaders[])(obj_t* obj, double* intensity) = 
    {
        psphere0_amb,
        psphere1_amb,
        psphere2_amb,
        psphere3_amb
    };
    #define NUM_SHADERS (int)(sizeof(sphere_shaders)/sizeof(void*))

    obj_t* new;
    double dndx;
    int ndx;
    new = sphere_init(in, objtype);
    if (new == NULL)
    {
        return NULL;
    }
    ndx = vec_get1(in, &dndx);
    if (ndx != 1)
        return NULL;
    ndx = (int)dndx;
    if (ndx >= NUM_SHADERS)
        return NULL;
    new->getamb = sphere_shaders[ndx];
    return(new);
}

/*
 * This is a shader for a procedural plane object. This one is for testing
 * pspheres.
 * @param: obj  The object that we are changing the ambience of.
 * @param: value  The output color vector for this amb.
 */
void psphere0_amb(obj_t* obj, double* value)
{
    copy3(obj->material.ambient, value);
    value[0] = value[0] + 1;
}

/*
 * Dr. K's rainbow function converted to sphere. Used for testing.
 * This is a shader for a procedural plane object.
 * @param: obj  The object that we are changing the ambience of.
 * @param: value  The output color vector for this amb.
 */
void psphere1_amb(obj_t* obj, double* value)
{
    double vec[3];
    double* v1 = Malloc((size_t)sizeof(double) * 3);
    double t1;
    sphere_t* sphere = (sphere_t*)obj->priv;
    copy3(obj->material.ambient, value);
    diff3(sphere->center, obj->hitloc, vec);
    *v1 = (vec[0] / sqrt(vec[0] * vec[0] + vec[1] * vec[1]));
    t1 = acos(*v1);
    if (vec[1] < 0) // acos() returns values in [0,PI]
        t1 = 2 * M_PI - t1; // extend to [0, 2PI] here 
    value[0] *= (1 + cos(2 * t1));
    value[1] *= (1 + cos(2 * t1 + 2 * M_PI / 3));
    value[2] *= (1 + cos(2 * t1 + 4 * M_PI / 3));
    free(v1);
}

/*
 * This is a shader for a procedural sphere object.
 * @param: obj  The object that we are changing the ambience of.
 * @param: value  The output color vector for this amb.
 */
void psphere2_amb(obj_t* obj, double* value)
{
    value[0] = tan(tgamma((int)(255 - obj->hitloc[0] + obj->normal[1] * 
                     obj->normal[0]) % 255))+ 16;
    value[1] = cos(tgamma((int)(255 - obj->normal[1] + obj->normal[2] * 
                     obj->hitloc[1]) % 255)) + 32;
    value[2] = sin(tgamma((int)(255 - obj->normal[2] + obj->normal[0] * 
                     obj->hitloc[2]) % 255)) + 8;
}

/*
 * This is a shader for a procedural sphere object.
 * @param: obj  The object that we are changing the ambience of.
 * @param: value  The output color vector for this amb.
 */
void psphere3_amb(obj_t* obj, double* value)
{
    value[0] = 255/M_PI * cos((int)(255 - obj->hitloc[0] + obj->normal[1] * 
                     obj->normal[0]) % 255);
    value[1] = 255/M_PI * cos((int)(255 - obj->normal[1] + obj->normal[2] * 
                     obj->hitloc[1]) % 255);
    value[2] = 255/M_PI * cos((int)(255 - obj->normal[2] + obj->normal[0] * 
                     obj->hitloc[2]) % 255);
}
