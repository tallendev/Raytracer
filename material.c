/*
 * Author: Tyler Allen
 * Date: 04/01/2013
 *
 * This file contains the material_load function, used to load material 
 * (lighting) data about objects into an obj_t.
 */

/*
 * Loads the material information for a function. Materials are parts of 
 * non-light objects, so this is a helper function. 
 *
 * Param: in  The input stream to read from.
 * Param: obj The object who's materials you are setting.
 *
 * Return: 0 for success or 1 for failure.
 */ 

#include "material.h"

int material_load(FILE* in, material_t* material)
{
     int pcount = 0;
     int rc = SUCCESS;
     char buff[BUFF_SIZE];
     pcount += fscanf(in, "%lf %lf %lf", 
                      &(material->ambient[R]), 
                      &(material->ambient[G]),
                      &(material->ambient[B]));
     fgets(buff, BUFF_SIZE, in);
     pcount += fscanf(in, "%lf %lf %lf",
                      &(material->diffuse[R]),
                      &(material->diffuse[G]),
                      &(material->diffuse[B]));
    fgets(buff, BUFF_SIZE, in);
    pcount += fscanf(in, "%lf %lf %lf",
                     &(material->specular[R]),
                     &(material->specular[G]),
                     &(material->specular[B]));
    fgets(buff, BUFF_SIZE, in);
    if (pcount != MATERIAL_OBJS)
    {
        rc = FAILURE;
    }
    return rc;
}

/*
 * This function puts the RGB ambient light values into the output array
 * specified.
 *
 * Param: obj  The object to take RGB ambient values from.
 * Param: output  The output array to store the RGB values in.
 */
void default_getamb(obj_t* obj, double* output)
{
    output[R] = obj->material.ambient[R];
    output[G] = obj->material.ambient[G];
    output[B] = obj->material.ambient[B];
}

/*
 * This function puts the RGB diffuse light values into the output array
 * specified.
 *
 * Param: obj  The object to take RGB ambient values from.
 * Param: output  The output array to store the RGB values in.
 */
void default_getdiff(obj_t* obj, double* output)
{
    output[R] = obj->material.diffuse[R];
    output[G] = obj->material.diffuse[G];
    output[B] = obj->material.diffuse[B];
}

/*
 * This function puts the RGB specular light values into the output array
 * specified.
 *
 * Param: obj  The object to take RGB ambient values from.
 * Param: output  The output array to store the RGB values in.
 */
void default_getspec(obj_t* obj, double* output)
{
    output[R] = obj->material.specular[R];
    output[G] = obj->material.specular[G];
    output[B] = obj->material.specular[B];
}

/*
 * Function for printing material data.
 * Param: material  Material to dump.
 */
void print_materials(FILE* out, material_t material)
{
    fprintf(out, "\nMaterial Data-\n"
             "Ambient  - %lf %lf %lf\n"
             "Diffuse  - %lf %lf %lf\n"
             "Specular - %lf %lf %lf\n",
             material.ambient[R],
             material.ambient[G], material.ambient[B],
             material.diffuse[R], material.diffuse[G],
             material.diffuse[B], material.specular[R],
             material.specular[G], material.specular[B]);
}
