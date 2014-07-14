/*
 * Author: Tyler Allen
 * Date: 3/15/2013
 *
 * This file contains functions used to calculate pixel color after a hit and
 * also to determine the closest object that a ray hits.
 */

/*
 * The header file containing our includes.
 */
#include "raytrace.h"
/* 
 * This function traces a ray for an individual pixel.
 *
 * Param: model  Our current model that we are trying to draw.
 * Param: base   The starting point of our ray.
 * Param: dir    A unit vector representing the direction of our ray.
 * Param: intensity The output array that we will store out pixel data into.
 * Param: total_dist The total distance to the object.
 * Param: last_hit   The object that was hit previously.
 *
 */
void ray_trace(model_t *model, double base[DIMENSIONS], double dir[DIMENSIONS], 
               double intensity[DIMENSIONS], double total_dist, obj_t *last_hit)
{
    double mindist = MISS;
    obj_t* closest = NULL;
    double specref[3] = {0.0, 0.0, 0.0};
    if (total_dist > MAX_DIST)
    {
        return;
    }

    closest = find_closest_object(model->scene, base, dir, last_hit, &mindist);
    
    if (closest == NULL)
    {
        return;
    }
    #ifdef DBG_HIT
        fprintf(stderr, "\nHIT %4d: %5.11f (%5.11lf, %5.11lf, %5.11lf) - The" 
                        "total dist is %lf\n",
                        closest->objid, mindist,
                        closest->hitloc[X], closest->hitloc[Y],
                        closest->hitloc[Z], mindist);
    #endif
    total_dist += mindist;
    double ambient[RGB_SIZE];
    closest->getamb(closest, ambient);
    sum3(ambient, intensity, intensity);
    diffuse_illumination(model, closest, intensity);
    intensity[R] /= total_dist;
    intensity[G] /= total_dist;
    intensity[B] /= total_dist;
    #ifdef DBG_AMB
        fprintf(stderr, "AMB (%5.11f, %5.11f, %5.11f) - \n\n", 
                        intensity[R], intensity[G], intensity[B]);
    #endif

    closest->getspec(closest, specref);
    if (specref[R] == 0 && specref[G] == 0 && specref[B] == 0)
        return;
    if (dot3(specref, specref) > 0)
    {
        double specint[RGB_SIZE] = {0.0, 0.0, 0.0};
        double ref_dir[XYZ];
        reflect3(dir, closest->normal, ref_dir);
        ray_trace(model, closest->hitloc, ref_dir, specint, 
                  total_dist, closest);
        specref[R] = specref[R] * specint[R];
        specref[G] = specref[G] * specint[G];
        specref[B] = specref[B] * specint[B];
    }
    sum3(intensity, specref, intensity);
}

/*
 * This function attempts to determine the closest object that our ray hits.
 *
 * Param: base The starting point of the current ray.
 * Param: dir  A unit vector representing the current direction of our ray.
 * Param: last_hit The last object that we hit.
 * Param: mindist Output pointer that will hold the distance to the object.
 *
 * Return: The closest object in the path of our ray (or NULL if no objects are
 *         in our path).
 */
obj_t* find_closest_object(list_t* scene, double base[DIMENSIONS], 
                            double dir[DIMENSIONS], obj_t* last_hit, 
                            double* mindist)
{
    obj_t* node = scene->head;
    obj_t* closest = NULL;
    while (node != NULL)
    {
        if (last_hit == NULL || last_hit != node)
        {
            double dist = node->hits(base, dir, node);
            #ifdef DBG_FIND
                fprintf(stderr, "\nFND %4d: %5.11lf - base X: %f Y: %f Z: %f\n"
                       "dir X: %lf Y: %lf Z: %lf\nhits: X: %lf Y: %lf Z: %lf\n", 
                        node->objid, dist, base[X], base[Y], base[Z], 
                        dir[X], dir[Y], dir[Z], node->hitloc[X], 
                        node->hitloc[Y], node->hitloc[Z]);
            #endif
            if ((*mindist == MISS || dist < *mindist) && 
                 dist >= ROUNDING_ADJUSTMENT)
            {
                *mindist = dist;
                closest = node;
            }
        }
        node = node->next;
    }
    return closest;
}
