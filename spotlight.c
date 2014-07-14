/*
 * Author: Tyler Allen
 * Date: 04/19/2013
 *
 * This is the header file for the spotlight source file.
 */
#include "spotlight.h"

/*
 * This function initializes a spotlight_t data structure and stores it inside
 * of a created light_t data structure.
 *
 * Param: in  The stream to read in frome.
 * Param: objtype  The number of the type of object that we are reading in.
 */
obj_t* spotlight_init(FILE* in, int objtype)
{
    char buff[BUFF_SIZE];
    obj_t* obj = NULL;
    light_t* light = NULL;
    spotlight_t* spot = NULL;
    int pcount = 0;
    obj = light_init(in, objtype);
    if (obj != NULL)
    {
        light = (light_t*)obj->priv;
        spot = Malloc(sizeof(spotlight_t));
        light->priv = (void*)spot;
        pcount += fscanf(in, "%lf %lf %lf",
                         &spot->direction[X],
                         &spot->direction[Y],
                         &spot->direction[Z]);
        fgets(buff, BUFF_SIZE, in);
        pcount += fscanf(in, "%lf", &spot->theta);
        fgets(buff, BUFF_SIZE, in);
        if (pcount == SPOTLIGHT_OBJS)
        {
            spot->costheta = cos(spot->theta * M_PI / HALF_CIRCLE);
            light->illum_check = spotlight_check;
            diff3(light->location, spot->direction, spot->direction);
        }
        else
        {
            fprintf(stderr, "Error found in spotlight_init...");
            obj = NULL;
        }
    }
    return(obj);
}

/*
 * Function for checking if an object is illuminated by this object.
 * Param: lobj  The spotlight we are checking.
 * Param: hitloc  The hit location on the object that we are checking.
 * Return: 0 if miss, otherwise some value greater than 0.
 */
int spotlight_check(obj_t* lobj, double* hitloc)
{
    light_t* light = (light_t*)lobj->priv;
    spotlight_t* spot = (spotlight_t*)light->priv;
    double adjust_hitloc[XYZ];
    double unit_dir[XYZ];
    diff3(light->location, hitloc, adjust_hitloc);
    unitvec3(adjust_hitloc, adjust_hitloc);
    unitvec3(spot->direction, unit_dir);
    return (dot3(adjust_hitloc, unit_dir) > spot->costheta);
}

/*
 * This function prints out information about a spotlight.
 * Param: out  The stream to print to
 * Param: obj  The object top print.
 */
void dump_spotlight(FILE* out, obj_t* obj)
{
    fprintf(out, "\nSpotlight data\n");
    light_t* light = (light_t*)obj->priv;
    spotlight_t* spot = (spotlight_t*)light->priv;
    fprintf(out, "Direction: %f %f %f\nTheta %f\nCosTheta %f\n",
                 spot->direction[X], spot->direction[Y], 
                 spot->direction[Z], spot->theta, spot->costheta);
    dump_light(out, obj);
}

/*
 * This function allows a spotlight to free all of its own data.
 * Param: spotlight  The initial light received.
 */
void kill_spotlight(void* spotlight)
{
    light_t* light = (light_t*)spotlight;
    spotlight_t* spot = (spotlight_t*)light->priv;
    free(spot);
    kill_light(light);
}
