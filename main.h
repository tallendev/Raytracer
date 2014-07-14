/*
 * @Author: Tyler Allen
 * @Date: 04/01/2013
 *
 * The header file for our main file, containing the includes necessary for 
 * main.
 */

/* Ensures we only include once... */
#pragma once


/* Includes the obj_t struct required in main, and also the utils header file
 * for the Malloc function. */
#include "object.h"

/* Includes the model_t struct required by main. */
#include "model.h"

/* This includes the make_image function that we call once we have all of our 
 * object data. */
#include "image.h" 

/* 
 * Allows for accessing errno in the event string conversion to 
 * integer fails. 
 */
#include <errno.h>

/* Representing base 10 in the strtol function. */
#define DECIMAL 10

/* The correct number of command line arguments to be taken with this program. 
 */
#define CORRECT_ARGS 3


void usage(char* filename);
