/*
 * Author: Tyler Allen
 * Date: 04/21/2013
 *
 * This is the source file containing logic for creating, destroying, printing
 * and detecting hits on a paraboloid. 
 */

#include "paraboloid.h"

/*
 * Function for initializing a paraboloid.
 * Param: in  The file we are reading in from.
 * Param: objtype  The object type of the object we are creating.
 * Return:  The new object that has been created.
 */
obj_t* parab_init(FILE* in, int objtype)
{
    char buff[BUFF_SIZE];
    int pcount = 0;
    obj_t* obj = object_init(in, objtype);
    if (obj)
    {
        parab_t* parab = Malloc(sizeof(parab_t));
        pcount += fscanf(in, "%lf %lf %lf",
                         &parab->center[X], &parab->center[Y], 
                         &parab->center[Z]);
        fgets(buff, BUFF_SIZE, in);
        pcount += fscanf(in, "%lf %lf %lf",
                         &parab->centerline[X], &parab->centerline[Y], 
                         &parab->centerline[Z]);
        fgets(buff, BUFF_SIZE, in);
        pcount += fscanf(in, "%lf %lf", &parab->radius, &parab->height);
        fgets(buff, BUFF_SIZE, in);
        if (pcount != PARAB_OBJS)
        {
            fprintf(stderr, "Error found in parab_init...\n");
            obj = NULL;
        }
        else
        {
            parab->scale = pow(parab->radius, SQUARED) / parab->height;
            double id_matrix[XYZ][XYZ];
            get_id_matrix(id_matrix);
            /* Adjustment for shapes aligned with the negative y axis. This
             * is a minor hack to adjust for this bad case. */
            if ((parab->centerline[X] == id_matrix[Y][X] && 
                 parab->centerline[Y] < 0 &&
                 parab->centerline[Z] == id_matrix[Y][Z]))
            {
                parab->centerline[X] = ROUNDING_ADJUSTMENT;
            }
            unitvec3(parab->centerline, parab->rotmat[Y]);
            cross_prod(parab->rotmat[Y], id_matrix[Y], parab->rotmat[X]);
            unitvec3(parab->rotmat[X], parab->rotmat[X]);
            cross_prod(parab->rotmat[X], parab->rotmat[Y], parab->rotmat[Z]);
            unitvec3(parab->rotmat[Z], parab->rotmat[Z]);
            transpose_mat(parab->rotmat, parab->irot);
            obj->hits = parab_hits;
            obj->priv = parab;
        }
    }
    return obj;
}

/*
 * Function for determining if a ray hit a paraboloid.
 *
 * Param: base  The starting point for the ray.
 * Param: dir   The direction of the approaching ray.
 * Param: obj   The plane object that is being tested for hits.
 *
 * Return: The distance from our viewpoint that the object is at, if we hit.
 *         If we miss, returns -1.
 */
double parab_hits(double* base, double* olddir, obj_t* obj)
{
    double t = MISS;
    parab_t* parab = (parab_t*)obj->priv;
    double newbase[XYZ];
    double newcenter[XYZ];
    double dir[XYZ];
    copy3(olddir, dir);
    diff3(parab->center, base, newbase);
    diff3(parab->center, parab->center, newcenter);
    double idmat[XYZ][XYZ];
    get_id_matrix(idmat);
    if (!(parab->centerline[X] == idmat[Y][X] && parab->centerline[Y] != 0 && 
          parab->centerline[Z] == idmat[Y][Z]))
    {
        transform(parab->rotmat, newbase, newbase);
        transform(parab->rotmat, dir, dir);
    }
    else
    {
        transform(idmat, newbase, newbase);
        transform(idmat, olddir, dir);
    }
    double a = pow(dir[X], SQUARED) + pow(dir[Z], SQUARED);
    double b = 2 * (newbase[X] * dir[X] + newbase[Z] * dir[Z]) - 
               parab->scale  * dir[Y];
    double c = (pow(newbase[X], SQUARED) + pow(newbase[Z], SQUARED)) -
                parab->scale * newbase[Y];
    double discrim = (pow(b, SQUARED) - 4 * a * c);
    double t1 = ((-1 * b) - sqrt(discrim))/(2 * a);
    double t2 = ((-1 * b) + sqrt(discrim))/(2 * a);
    t2 = (check_parab_hit (obj, dir, newbase, t2));
    t1 = (check_parab_hit (obj, dir, newbase, t1));
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
double check_parab_hit(obj_t* obj, double* dir, double* newbase, double t)
{
    parab_t* parab = (parab_t*)(obj->priv);
    double hit[XYZ];
    double scaled_d[XYZ];
    double normal[XYZ];
    scale3(t, dir, scaled_d);
    sum3(newbase, scaled_d, hit);
    if (hit[Y] < 0 || hit[Y] > parab->height) 
    {
        t = MISS;
    }
    else
    {
        double idmat[XYZ][XYZ];
        get_id_matrix(idmat);
        normal[X] = 2 * hit[X];
        normal[Y] = -1 * parab->scale;
        normal[X] = 2 * hit[Z];
        if (!(parab->centerline[X] == idmat[Y][X] && 
              parab->centerline[Y] != 0 &&
              parab->centerline[Z] == idmat[Y][Z]))
        {
            transform(parab->irot, hit, obj->hitloc);
            transform(parab->irot, normal, obj->normal);
        }
        else
        {
            transform(idmat, hit, obj->hitloc);
            transform(idmat, normal, obj->normal);
        }
        sum3(obj->hitloc, parab->center, obj->hitloc);
    }
    return t;
}

/*
 * Function for dumping the data about paraboloid objects.
 * Param: out  The stream to dump to.
 * Param: obj  The object to dump.
 */
void dump_parab (FILE* out, obj_t* obj)
{
    fprintf(out, "\nParaboloid Data\n");
    parab_t* parab = (parab_t*)obj->priv;
    fprintf(out, "\nCenter %f %f %f\nCenterline: %f %f %f\nRadius: %f\n"
                 "height: %f\nscale: %f\n", parab->center[X], parab->center[Y], 
                 parab->center[Z], parab->centerline[X], parab->centerline[Y], 
                 parab->centerline[Z], parab->radius, parab->height, 
                 parab->scale);
}
/*
 * Function for freeing a parabola.
 * Param: parab  The parabola to free.
 */
void kill_parab(void* parab)
{
    free(parab);
}
