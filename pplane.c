/*
 * @Author: Tyler Allen
 * @Date: 04/06/2013
 *
 * This file contains functions for initializing and changing a procedural
 * plane object. We use the obj_t typedef and the plane_t typedef as a basis
 * for this.
 */

/* Header for this source file. */
#include "pplane.h"

/*
 * This function is used to initialize a procedural plane object. We utilize
 * the plane_init function to give us a basis for our procedural plane.
 * 
 * Param: in  The file we are currently reading data from. 
 * Param: objtype  The type of object we are reading in so that we can send it
 *                 to plane_init.
 */
obj_t* pplane_init(FILE *in, int objtype)
{
    /* Static array containing shaders. */
    static void (*plane_shaders[])(obj_t* obj, double* intensity) = 
    {
        pplane0_amb,
        pplane1_amb,
        pplane2_amb,
        pplane3_amb
    };
    #define NUM_SHADERS (int)(sizeof(plane_shaders)/sizeof(void*))

    obj_t* new;
    double dndx;
    int ndx;
    new = plane_init(in, objtype);
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
    new->getamb = plane_shaders[ndx];
    return(new);
}

/*
 * Dr Kreahling's test function.
 */
void pplane0_amb(obj_t* obj, double* value)
{
    double vec[3];
    plane_t *p = (plane_t *)(obj->priv);
    int isum;
    double sum;
    copy3(obj->material.ambient, value);
    diff3(p->point, obj->hitloc, vec);
    sum = 1000 + vec[0] * vec[1] * vec[1] / 100 + vec[0] * vec[1] / 100;
    isum = (int)sum;
    if (isum & 1)
        value[0] = 0; // zap red 
    else
        value[2] = 0; // zap blue
}

/*
 * Dr Kreahling's rainbow test function.
 */
void pplane1_amb(obj_t* obj, double* value)
{
    double vec[3];
    double* v1 = Malloc((size_t)sizeof(double) * 3);
    double t1;
    plane_t* plane = (plane_t*)obj->priv;
    copy3(obj->material.ambient, value);
    diff3(plane->point, obj->hitloc, vec);
    *v1 = (vec[0] / sqrt(vec[0] * vec[0] + vec[1] * vec[1]));
    t1 = acos(*v1);
    if (vec[1] < 0) // acos() returns values in [0,PI]
        t1 = 2 * M_PI - t1; // extend to [0, 2PI] here
    value[0] *= (1 + cos(2 * t1));
    value[1] *= (1 + cos(2 * t1+ 2 * M_PI / 3));
    value[2] *= (1 + cos(2 * t1+ 4 * M_PI / 3));
    free(v1);
}

/*
 * This is a shader for a procedural plane object.
 * @param: obj  The object that we are changing the ambience of.
 * @param: value  The output color vector for this amb.
 */
void pplane2_amb(obj_t* obj, double* value)
{
    double vec[3];
    double* v1 = Malloc((size_t)sizeof(double) * 3);
    double t1;
    plane_t* plane = (plane_t*)obj->priv;
    copy3(obj->material.ambient, value);
    diff3(plane->point, obj->hitloc, vec);
    /* t1 borrowed from Dr. K.*/
    *v1 = (vec[0] / sqrt(vec[0] * vec[0] + vec[1] * vec[1]));
    t1 = acos(*v1);
    if (vec[1] < 0) 
        t1 = 2 * M_PI - t1;
    value[0] = 255 - t1 * (255 * (pow(obj->hitloc[Y], SQUARED) - 
                     pow(obj->hitloc[X], SQUARED))) / 
                     ((obj->hitloc[Y]) * obj->hitloc[X]);
    value[1] = 255 - t1 * (255 * (pow(obj->hitloc[Y], SQUARED) - 
                     pow(obj->hitloc[X], SQUARED))) / 
                     ((obj->hitloc[Y]) * pow(obj->hitloc[X], SQUARED));
    value[2] = 255 - t1 * (255 * (pow(obj->hitloc[Y], SQUARED) - 
                     pow(obj->hitloc[X], SQUARED))) / 
                     (pow((obj->hitloc[Y]),SQUARED) * obj->hitloc[X]);
    free(v1);
}

/*
 * This is a shader for a procedural plane object.
 * @param: obj  The object that we are changing the ambience of.
 * @param: value  The output color vector for this amb.
 */
void pplane3_amb(obj_t* obj, double* value)
{
    double vec[XYZ];
    double temp[XYZ];
    copy3(value, temp);
    double* v1 = Malloc((size_t)sizeof(double) * 3);
    double t1;
    plane_t* plane = (plane_t*)obj->priv;
    copy3(obj->material.ambient, value);
    diff3(plane->point, obj->hitloc, vec);
    /* t1 borrowed from Dr. K.*/
    *v1 = (vec[0] / sqrt(vec[0] * vec[1] + vec[1] * vec[0]));
    t1 = acos(*v1);
    if (vec[1] < 0) 
        t1 = 2 * M_PI - t1;
    value[0] = fabs(255 - t1 * (255 * sqrt((pow(obj->hitloc[Y], SQUARED) - 
                     pow(obj->hitloc[X], SQUARED)))) / 
                     ((obj->hitloc[Y]) * obj->hitloc[X]));
    value[1] = fabs(255 - t1 * (255 * (pow(obj->hitloc[Y], SQUARED) - 
                     pow(obj->hitloc[X], SQUARED))) / 
                     ((obj->hitloc[Y]) * pow(obj->hitloc[X], SQUARED)));
    value[2] = fabs(255 - t1 * sqrt((255 * (pow(obj->hitloc[Y], SQUARED) - 
                     pow(obj->hitloc[X], SQUARED))))); 
    if(value[0] > 255 || value[0] < 0 || FP_NAN == value[0])
    {
        value[0] = fabs(obj->hitloc[X]);
    }
    if(value[2] > 255 || value[2] < 0 || FP_NAN == value[2])
    {
        value[2] = fabs(obj->hitloc[Z]);
    }
    if(value[1] > 255 || value[1] < 0 || value[1] == FP_NAN)
    {
        value[1] = fabs(obj->hitloc[Y]);
    }
    free(v1);
}
