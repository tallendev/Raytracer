/*
 * Author: Tyler Allen
 * Date: 04/11/2013
 *
 * This file contains the functions used for modifying and constructing 
 * finite plane objects.
 */

/* The header for this source file. */
#include "fplane.h"

obj_t* fplane_init(FILE* in, int objtype)
{
    char buff[BUFF_SIZE];
    obj_t* obj = plane_init(in, objtype); 
    if(obj)
    {
        int pcount = 0;
        plane_t* plane = (plane_t*)obj->priv;
        fplane_t* fplane = Malloc(sizeof(fplane_t));
        plane->priv = fplane;
        pcount += fscanf(in, "%lf %lf %lf",
                         &(fplane->xdir[X]),
                         &(fplane->xdir[Y]),
                         &(fplane->xdir[Z]));
        fgets(buff, BUFF_SIZE, in);
        pcount += fscanf(in, "%lf %lf", &(fplane->size[X]),&(fplane->size[Y]));
        fgets(buff, BUFF_SIZE, in);
        if (pcount == FPLANE_OBJS)
        {
            double unit_norm[XYZ];
            unitvec3(plane->normal, unit_norm);
            double unit_xdir[XYZ];
            unitvec3(fplane->xdir, unit_xdir);
            projection(unit_xdir, unit_norm, unit_xdir);
            unitvec3(unit_xdir, unit_xdir);
            copy3(unit_xdir, fplane->rotmat[X]); 
            copy3(unit_norm, fplane->rotmat[Z]);

            cross_prod(fplane->rotmat[Z], fplane->rotmat[X], 
                       fplane->rotmat[Y]);
            obj->hits = hits_fplane;
        }
        else
        {
            fprintf(stderr, "Error detected in fplane_init...\n");
            obj = NULL;
        }
    }
    return obj;
}

/*
 * Function for determining if a ray hits a finite plane.
 *
 * Param: base  The starting point of the ray.
 * Param: dir   The direction of the approaching ray.
 * Param: obj   The plane object that is being tested for hits.
 *
 * Return: The distance to the object.
 */
double hits_fplane(double* base, double* dir, obj_t* obj)
{
    double t = hits_plane(base, dir, obj);
    if (t != MISS)
    {
        plane_t* plane = (plane_t*)obj->priv;
        fplane_t* fplane = (fplane_t*)plane->priv;
        double newhit[XYZ];
        diff3(plane->point, obj->hitloc, newhit);
        transform(fplane->rotmat, newhit, newhit);
        if (newhit[X] > fplane->size[X] || newhit[X] < 0.0 ||
             newhit[Y] > fplane->size[Y] || newhit[Y] < 0.0)
        {
            t = MISS;
        }
    }
    return t;
}

/*
 * Function for dumping a finite plane object.
 * Param: out  The stream to dump to.
 * Param: obj  The finite plane object that we are dumping.
 */
void dump_fplane(FILE* out, obj_t* obj)
{
    fprintf(out, "\nFinite Plane Data\n"); 
    plane_t* plane = (plane_t*)obj->priv;
    fplane_t* fplane = (fplane_t*)plane->priv;
    fprintf(out, "\nX direction: %f %f %f\nSize: %f %f\n",
                 fplane->xdir[X], fplane->xdir[Y], fplane->xdir[Z],
                 fplane->size[X], fplane->size[Y]);
    dump_plane(out, obj);
}

/*
 * Kill function for cleaning up a pplane.
 *
 * Param: plane  The private data of an obj_t type of type fplane.
 */
void kill_fplane(void* plane)
{
    plane_t* aplane = (plane_t*)plane;
    fplane_t* temp = aplane->priv;
    free(temp);
    kill_plane(aplane); /* Add more if pplane gets more complicated */
}
