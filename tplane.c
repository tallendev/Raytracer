/*
 * Author: Tyler Allen
 * Version: 04/12/2013
 *
 * This file includes logic for creating tiled planes and detecting if they 
 * have been hit by a ray.
 */

/* Header for this source file. */
#include "tplane.h"

/*
 * This function is used to initialize a tplane_init object.
 *
 * Param: in  The file to read input in from.
 * Param: objtype  The type of object that this object is.
 *
 * Return: The constructed tplane object.
 */
obj_t* tplane_init(FILE* in, int objtype)
{
    obj_t* obj = fplane_init(in, objtype);
    if(obj)
    {
        plane_t* plane = (plane_t*)obj->priv;
        fplane_t* fplane = (fplane_t*)plane->priv;
        tplane_t* tplane = Malloc(sizeof(tplane_t));
        fplane->priv = tplane;
        int rc = material_load(in, &tplane->background);
        if (rc != SUCCESS)
        {
            fprintf(stderr, "Error detected in tplane_init...");
            obj = NULL;
        }
        else
        {
            obj->getamb = tp_amb;
            obj->getdiff = tp_diff;
            obj->getspec = tp_spec;
            obj->hits = hits_tplane;

            double unit_norm[XYZ];
            unitvec3(plane->normal, unit_norm);

            double unit_xdir[XYZ];
            unitvec3(fplane->xdir, unit_xdir);

            projection(unit_xdir, unit_norm, unit_xdir);
            unitvec3(unit_xdir, unit_xdir);

            copy3(unit_xdir, fplane->rotmat[X]);
            copy3(unit_norm, fplane->rotmat[Z]);

            cross_prod(fplane->rotmat[Z], fplane->rotmat[X], fplane->rotmat[Y]);
            unitvec3(fplane->rotmat[Y], fplane->rotmat[Y]);
        }
    }
    return obj;
}

/*
 * This function selects which tile color we have landed on.
 * Param: obj  The object who's hitlocation we need to analyze.
 * Return: 0 or 1, alternating by tile.
 */
int tp_select(obj_t* obj)
{
    plane_t* plane = (plane_t*)obj->priv;
    fplane_t* fp = (fplane_t*)plane->priv;
    double newhit[XYZ];
    diff3(plane->point, obj->hitloc, newhit);
    transform(fp->rotmat, newhit, newhit);
    int relx = (int)((10000 + newhit[X]) / fp->size[X]);
    int rely = (int)((10000 + newhit[Y]) / fp->size[Y]);
    return ((relx + rely) & 1);
}

/*
 * This function is a get-lighting plugin used to determine the diffuse 
 * lighting on a tiled plane. 
 *
 * Param: obj The object that we are changing the light intensity on.
 * Param: value  The intensity vector that we are altering.
 */
void tp_diff(obj_t* obj, double* value)
{
    plane_t* pln = (plane_t*)obj->priv;
    fplane_t* fp = (fplane_t*)pln->priv;
    tplane_t* tp = (tplane_t*)fp->priv;
    if(tp_select(obj))
    {
        copy3(obj->material.diffuse, value);
    }
    else
    {
        copy3(tp->background.diffuse, value);
    }
}

/*
 * This function is a get-lighting plugin used to determine the ambient
 * lighting on a tiled plane. 
 *
 * Param: obj The object that we are changing the light intensity on.
 * Param: value  The intensity vector that we are altering.
 */
void tp_amb(obj_t* obj, double* value)
{
    plane_t* pln = (plane_t*)obj->priv;
    fplane_t* fpln = (fplane_t*)pln->priv;
    tplane_t* tp = (tplane_t*)fpln->priv;
    if(tp_select(obj))
    {
        copy3(obj->material.ambient, value);
    }
    else
    {
        copy3(tp->background.ambient, value);
    }
}

/*
 * This function is a get-lighting plugin used to determine the specular 
 * lighting on a tiled plane. 
 *
 * Param: obj The object that we are changing the light intensity on.
 * Param: value  The intensity vector that we are altering.
 */
void tp_spec(obj_t* obj, double* value)
{
    plane_t* pln = (plane_t*)obj->priv;
    fplane_t* fpln = (fplane_t*)pln->priv;
    tplane_t* tp = (tplane_t*)fpln->priv;
    if(tp_select(obj))
    {
        copy3(obj->material.specular, value);
    }
    else
    {
        copy3(tp->background.specular, value);
    }
}

double hits_tplane(double* base, double* dir, obj_t* obj)
{
    double t = hits_plane(base, dir, obj);
    return t;
}

/*
 * Function for dumping a tplane object. 
 * Param: out  The stream to dump object data to.
 * Param: obj  The object to dump.
 */
void dump_tplane(FILE* out, obj_t* obj)
{
    fprintf(out, "\nTiled Plane Data\n");
    plane_t* plane = (plane_t*) obj->priv;
    fplane_t* fplane = (fplane_t*)plane->priv;
    tplane_t* tplane = (tplane_t*)fplane->priv;
    print_materials(out, tplane->background);
    dump_fplane(out, obj);
}

/*
 * Cleanup function containing logic for how a tplane can kill itself.
 *
 * Param: tplane  The void* pointer to private data in an obj_t struct. We have
 *                to pull the tplane out of it.
 */
void kill_tplane(void* tplane)
{
    plane_t* plane = (plane_t*)tplane;
    fplane_t* fplane = (fplane_t*)plane->priv;
    tplane_t* newplane = fplane->priv;
    free(newplane);
    kill_fplane(tplane);
}
