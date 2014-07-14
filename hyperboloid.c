/*
 * Author: Tyler Allen
 * Date: 05/02/2013
 * 
 * This file contains logic for creating, dumping, detecting hits, and killing
 * a hyperboloid.
 */

/* Header file for this source file. */
#include "hyperboloid.h"

/*
 * Function for initializing a hyperboloid.
 * Param: in  The file we are reading in from.
 * Param: objtype  The object type of the object we are creating.
 * Return:  The new object that has been created.
 */
obj_t* hyperb_init(FILE* in, int objtype)
{
    char buff[BUFF_SIZE];
    int pcount = 0;
    obj_t* obj = object_init(in, objtype);
    if (obj)
    {
        hyperb_t* hyperb = Malloc(sizeof(hyperb_t));
        pcount += fscanf(in, "%lf %lf %lf",
                         &hyperb->center[X], &hyperb->center[Y], &hyperb->center[Z]);
        fgets(buff, BUFF_SIZE, in);
        pcount += fscanf(in, "%lf %lf %lf",
                         &hyperb->centerline[X], &hyperb->centerline[Y], 
                         &hyperb->centerline[Z]);
        fgets(buff, BUFF_SIZE, in);
        pcount+= fscanf(in, "%lf %lf",
                        &hyperb->radius, &hyperb->height);
        fgets(buff, BUFF_SIZE, in);
        pcount += fscanf(in, "%lf", &hyperb->radiusc);
        fgets(buff, BUFF_SIZE, in);
        if (pcount == HYPER_OBJS)
        {
            double id_matrix[XYZ][XYZ];
            get_id_matrix(id_matrix);
            /* Adjustment for shapes aligned with the negative y axis. This
             * is a minor hack to adjust for this bad case. */
            if ((hyperb->centerline[X] == id_matrix[Y][X] && 
                 hyperb->centerline[Y] < 0 &&
                 hyperb->centerline[Z] == id_matrix[Y][Z]))
            {
                hyperb->centerline[X] = ROUNDING_ADJUSTMENT;
            }
            unitvec3(hyperb->centerline, hyperb->centerline);
            copy3(hyperb->centerline, hyperb->rotmat[Y]);
            cross_prod(hyperb->centerline, id_matrix[Y], hyperb->rotmat[X]);
            unitvec3(hyperb->rotmat[X], hyperb->rotmat[X]);
            cross_prod(hyperb->rotmat[X], hyperb->rotmat[Y], hyperb->rotmat[Z]);
            unitvec3(hyperb->rotmat[Z], hyperb->rotmat[Z]);
            transpose_mat(hyperb->rotmat, hyperb->irot);

            hyperb->scale = (pow(hyperb->radius, SQUARED) - 
                             pow(hyperb->radiusc, SQUARED)) 
                            / pow(hyperb->height, SQUARED);
            obj->hits = hyperb_hits;
            obj->priv = hyperb;
        }
        else
        {
            fprintf(stderr, "Error found in hyperb_init...\n");
            obj = NULL;
        }
    }
    return obj;
}

/*
 * Function for determining if a ray hit a hyperboloid.
 *
 * Param: base  The starting point for the ray.
 * Param: dir   The direction of the approaching ray.
 * Param: obj   The plane object that is being tested for hits.
 *
 * Return: The distance from our viewpoint that the object is at, if we hit.
 *         If we miss, returns -1.
 */
double hyperb_hits(double* base, double* dir_start, obj_t* obj)
{
    double t = MISS;
    hyperb_t* hyperb = (hyperb_t*)obj->priv;
    double newbase[XYZ];
    double newcenter[XYZ];
    diff3(hyperb->center, base, newbase);
    double dir[XYZ];
    diff3(hyperb->center, hyperb->center, newcenter);
    double idmat[XYZ][XYZ];
    get_id_matrix(idmat);
    if (!(hyperb->centerline[X] == idmat[Y][X] && hyperb->centerline[Y] != 0 &&
          hyperb->centerline[Z] == idmat[Y][Z]))
    {
        transform(hyperb->rotmat, newbase, newbase);
        transform(hyperb->rotmat, dir_start, dir);
    }
    else
    {
        transform(idmat, newbase, newbase);
        transform(idmat, dir_start, dir);
    }
    double a = pow(dir[X], SQUARED) + pow(dir[Z], SQUARED) - hyperb->scale * 
               pow(dir[Y], SQUARED);
    double b = 2 * (newbase[X] * dir[X] + newbase[Z] * dir[Z] - 
               hyperb->scale * newbase[Y] * dir[Y]);
    double c = pow(newbase[X], SQUARED) + pow(newbase[Z], SQUARED) -
               hyperb->scale * pow(newbase[Y], SQUARED) - 
               pow(hyperb->radiusc, SQUARED);
    double discrim = pow(b, SQUARED) - 4 * a * c;
    double t1 = ((-1 * b) - sqrt(discrim))/(2 * a);
    double t2 = ((-1 * b) + sqrt(discrim))/(2 * a);
    t2 = (check_hyperb_hit (obj, dir, newbase, t2));
    t1 = (check_hyperb_hit (obj, dir, newbase, t1));
    if (t1 == MISS)
    {
        t = t2;
    }
    else if (t2 == MISS)
    {
        t = t1;
    }
    else
    {
        t = t1 < t2 ? t1: t2;
    }
    return t;
}

/*
 * Function for checking both the positive and negative side of the quadratic
 * hit function.
 *
 * Param: obj  The object to check hits with and store a hitpoint in.
 * Param: dir  The direction of the current ray.
 * Param: newbase  The base of the ray we are shooting.
 * Param: t  The current distance from the object.
 *
 * return:  The new t value, representing the distance from the object.
 */
double check_hyperb_hit(obj_t* obj, double* dir, double* newbase, double t)
{
    hyperb_t* hyperb = (hyperb_t*)(obj->priv);
    double hit[XYZ];
    double scaled_d[XYZ];
    scale3(t, dir, scaled_d);
    sum3(newbase, scaled_d, hit);
    if (hit[Y] > hyperb->height  || 
        (hit[Y] < (hyperb->height + hyperb->height * -2)))
    {
        t = MISS;
    }
    else
    {
        double normal[XYZ];
        double idmat[XYZ][XYZ];
        get_id_matrix(idmat);
        normal[X] = 2 * hit[X];
        normal[Y] = -2 * hyperb->scale * hit[Y];
        normal[Z] = 2 * hit[Z];
        if (!(hyperb->centerline[X] == idmat[Y][X] && 
              hyperb->centerline[Y] != 0 &&
              hyperb->centerline[Z] == idmat[Y][Z]))
        {
            transform(hyperb->irot, hit, obj->hitloc);
            transform(hyperb->irot, normal, obj->normal);
        }
        else
        {
            transform(idmat, hit, obj->hitloc);
            transform(idmat, normal, obj->normal);
        }
        sum3(obj->hitloc, hyperb->center, obj->hitloc);
    }
    return t;
}

/*
 * Function for dumping the data about hyperboloid objects.
 * Param: out  The stream to dump to.
 * Param: obj  The object to dump.
 */
void dump_hyperb(FILE* out, obj_t* obj)
{
    fprintf(out, "\nHyperboloid Data\n");
    hyperb_t* hyperb = (hyperb_t*)obj->priv;
    fprintf(out, "\nCenter %f %f %f\nCenterline: %f %f %f\nRadius: %f\n"
                 "height: %f\n", hyperb->center[X], hyperb->center[Y], 
                 hyperb->center[Z], hyperb->centerline[X], hyperb->centerline[Y], 
                 hyperb->centerline[Z], hyperb->radius, hyperb->height);
}

/*
 * Function allowing a hyperboloid to kill itself and free its memory.
 * Param: hyperb  Hyperboloid to kill.
 */
void kill_hyperb(void* hyperb)
{
    free(hyperb);
}
