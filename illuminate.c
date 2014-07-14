/*
 * Author: Tyler Allen
 * Date: 04/09/2013
 *
 * This is the source file illuminate.c. It contains functions that we use 
 * to calculate diffuse lighting.
 */

#include "illuminate.h"

/* diffuse_illumination - This function loops through every cals process_light
 * to determine what it hits.
 *
 * Param: model - pointer to the model structure
 * Param: hitobj - object that was hit by the ray
 * Param: intensity - where to add the intensity
 */
void diffuse_illumination(model_t* model, obj_t* hitobj, 
                          double intensity[RGB_SIZE])
{
    obj_t* node = model->lights->head;
    while (node)
    {
        process_light(model->scene, hitobj, node, intensity);
        node = node->next;
    }
}

/*
 * process_light - This function tries to determine if a light hits an object
 *                 by drawing a ray from the initial hitlocation from the 
 *                 initial ray from the viewer to the light that we are 
 *                 testing.
 *
 * Param: lst - list of all objects
 * Param hitobj - The object hit by the ray
 * Param: lobj - The current light source
 * Param: ivec - [r, g, b] intensity vector
 *
 * return success 0 if failed to hit an object, 1 for success. 
 *
 */

int process_light (list_t* lst, obj_t* hitobj, obj_t* lobj, 
                    double ivec[RGB_SIZE])
{
    int success = 0;
    obj_t* closest = NULL;
    double mindist = MISS;
    double dir[DIMENSIONS];
    light_t* light = (light_t*)lobj->priv;
    diff3(hitobj->hitloc, light->location, dir);
    double dist = sqrt(pow(dir[X], SQUARED) + pow(dir[Y], SQUARED) + 
                  pow(dir[Z], SQUARED));
    double hitobj_unit[DIMENSIONS];
    double dir_unit[DIMENSIONS];
    unitvec3(hitobj->normal, hitobj_unit);
    unitvec3(dir, dir_unit);
    double theta = dot3(dir_unit, hitobj_unit);
    #ifdef DBG_DIFFUSE
        fprintf(stderr, "hit object id was       %d\n", hitobj->objid);
        vecprnN("hit point was            \n", hitobj->hitloc, DIMENSIONS);
        vecprnN("normal at hitpoint       \n", hitobj-> normal, DIMENSIONS);
        fprintf(stderr, "light object id was      %d\n", lobj->objid);
        vecprnN("light center was         \n", light->location, DIMENSIONS);
        vecprnN("unit vector to light is  \n", dir, DIMENSIONS);
        fprintf(stderr, "distance to light is     %f\n", dist);
        fprintf(stderr, "theta is                 %f\n", theta);
    #endif
    if (theta > 0)
    {
        closest = find_closest_object(lst, hitobj->hitloc, dir, hitobj, 
                                      &mindist);
        int flag = 1;
        if(light->illum_check)
        {
            flag = light->illum_check(lobj, hitobj->hitloc);
        }
        if ((!closest || (mindist != MISS && mindist > dist)) && flag)
        {
            double diffuse[RGB_SIZE];
            hitobj->getdiff(hitobj, diffuse);
            *(ivec + 0) += diffuse[R] * light->emissivity[R] * theta / dist;
            *(ivec + 1) += diffuse[G] * light->emissivity[G] * theta / dist;
            *(ivec + 2) += diffuse[B] * light->emissivity[B] * theta / dist;
            success = 1;
            #ifdef DBG_DIFFUSE
                vecprnN("Emissivity of the light    \n", light->emissivity, 
                                                         DIMENSIONS);
                vecprnN("Diffuse reflectivity       \n", 
                         hitobj->material.diffuse, DIMENSIONS);
                vecprnN("Current ivec               \n", ivec, DIMENSIONS);
            #endif
        }
        else
        {
            #ifdef DBG_DIFFUSE
                fprintf(stderr, "\nhit object occluded by %d\n", 
                        closest->objid);
                fprintf(stderr, "\ndistance was %f\n", mindist);
            #endif
        }
    }
    return success;
}


