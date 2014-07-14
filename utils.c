/*
 * Author: Tyler Allen
 * Date: 03/08/2013
 *
 * A set of utility functions for general use in the ray tracer. Currently,
 * only including the Malloc and Calloc wrapper functions. The header file 
 * also includes the stdio and stdlib functions.
 */

/* Includes stdio, stdlib, and the prototypes for the functions found in this
 * file. */
#include "utils.h"

/*
 * A wrapper function for the malloc function in stdlib. It will catch a null
 * return from malloc indicating that there was a problem aquiring the 
 * requested memory, and will allow the program to exit with error as opposed
 * to crashing. Otherwise, it will simply return the aquired memory address. 
 * 
 * Param: size  The amount (in bytes) of memory to request from malloc.
 *
 * Return: address The aquired memory address returned from the malloc call if 
 * it is succesful.
 */
void* Malloc(size_t size)
{
    void* address = malloc(size);
    if (NULL == address)
    {
        fprintf(stderr, "Unable to aquire memory of size %zd. Now exiting...\n",
                size);
        exit(EXIT_FAILURE);
    }
    return address;
}

/*
 * A wrapper function for the malloc function in stdlib. It will catch a null
 * return from calloc indicating that there was a problem aquiring the 
 * requested memory, and will allow the program to exit with error as opposed
 * to crashing. Otherwise, it will simply return the aquired memory address.
 * 
 * Param: spaces The number of spaces to request from calloc.
 * Param: size   The size (in bytes) of the data type to request from calloc.
 *
 * Return: address The aquired memory address returned from the malloc call if 
 * it is succesful.
 */
void* Calloc(int spaces, size_t size)
{
    void* address = calloc((size_t)spaces, size);
    if (NULL == address)
    {
        fprintf(stderr, "Unable to aquire memory of size %zd. Now exiting...\n",
                size);
        exit(EXIT_FAILURE);
    }
    return address;
}

/*
 * This function determines if an input objtype is within the range of our 
 * defined lights in our object enumeration.
 *
 * Param: objtype  The object type to test.
 * 
 * Return: boolean 1 if the object is a light, 0 otherwise.
 */
int is_light(int objtype)
{
    int boolean;
    if(objtype <= LAST_LIGHT)
    {
        boolean = TRUE;
    }
    else
    {
        boolean = FALSE;
    }
    return boolean;
}

/*
 * Reads in one number from file input. We will only succeed
 * if we read exactly one item.
 *
 * Param: in  The file to read from.
 * Param: ndx The output double to store the value in.
 * Return: pcount  The number of objects read in. Only one is success.
 */
int vec_get1(FILE* in, double* ndx)
{
    char buff[BUFF_SIZE];
    int pcount = fscanf(in, "%lf", ndx);
    fgets(buff, BUFF_SIZE, in);
    return pcount;
}
