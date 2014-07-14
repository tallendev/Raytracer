/*
 * Author: Tyler Allen
 * Date: 03/13/2013
 *
 * Main procedural file for reading input, constructing structures and lists
 * of objects and lights, and then finally creating and writing the image.
 */

/* The header file for this source file. */
#include "main.h"

/*
 * The main function for this program, described above.
 */
int main(int argc, char **argv)
{
    /* Initializes the model for this image. */
    model_t* model = (model_t*)Malloc(sizeof(model_t));

    /* Tracks if this file has completed operations succesfully. */
    int rc;

    /* This section does argument checking. This has been moved out of 
     * projection_init, where it was originally located in the class notes. */
    if(argc != CORRECT_ARGS)
    {
        fprintf(stderr, "Incorrect number of arguments: %d\n", argc);
        usage(argv[0]); 
    }
    /* Sets errno to zero for to verify that nothing else has affected it. */
    errno = 0;
    /* Sets secondary errno to save value after first call. */
    int errno1 = 0;
    /* Converts arg1 to int if possible. */
    int x =(int) strtol(argv[1], NULL, DECIMAL);
    errno1 = errno;
    /* Converts arg2 to int if possible. */
    int y = (int)strtol(argv[2], NULL, DECIMAL);
    /* The first statement does errno checking to check for problems from the
     * strtol function. */
    if(errno || errno1)
    {
        fprintf(stderr, "X and Y arguments could not be converted to "
                        "integer values.\n");
        usage(argv[0]);
    }
    /* 
     * This statement verifies that the input values are correct, which isn't 
     * true for values less than or equal to 0. This ray tracer will try 
     * arbitrarily large window sizes, but may fail.
     */
    else if(x <= 0 || y <= 0)
    {
        fprintf(stderr, "Command line arguments for X and Y are not integers"
                        "or are less than or equal to zero.\n This program does"
                        "not understand window sizes less than or equal to" 
                        "zero");
        usage(argv[0]);
    }
    /* Sets up the initial projection. */
    model->proj = projection_init(x, y, stdin);
    /* Dumps the projection info just read in to stderr for debugging. */
    projection_dump(stderr, model->proj);
    /* Initializes the linked list for the lights and scene object. */
    model->lights = list_init();
    model->scene = list_init();
    /* Initializes the model's values, and stores a return rc value. */
    rc = model_init(stdin, model);
    /* Dumps full model for debugging purposes. */
    model_dump(stderr, model);

    /* If no problems so far, make the image. */
    if (rc == SUCCESS)
    {
        make_image(model);
        /*fprintf(stderr, "Post-image print:\n\n");
        projection_dump(stderr, model->proj);
        model_dump(stderr, model);*/
    }
    else
    {
        fprintf(stderr, "An error in the input file has been detected.\n"
                        "Now cleaning up and exiting... no output image "
                        "produced.");
    }
    /* Frees the model */
    free(model->proj);
    fprintf(stderr, "\nNow deleting lights...");
    /* Recursively deletes the lights list. */
    delete_list(model->lights);
    fprintf(stderr, "Lights deleted succesfully.\nNow deleting scene...");
    /* Recursively deletes the scene list. */
    delete_list(model->scene);
    fprintf(stderr, "Scene deleted succesfully.\n");
    /* Finally frees the model. */
    free(model);
    fprintf(stderr, "Cleanup complete.\n");
    /* Returns an exit status based on success of previous actions. */
    return(rc);
}

/*
 * A function for printing the proper usage of this program and exiting with
 * failure in the event that command line arguments are incorrect. Proper 
 * usage of this function should include a more precise error message prior
 * to calling this function.
 *
 * Param: filename  The filename of this file to print a more helpful and 
 *                  accurate usage message.
 */
void usage(char* filename)
{
    fprintf(stderr, "Usage: %s <x world coordinate> <y world coordinate>"
                    "<output file name>",
            filename);
    exit(EXIT_FAILURE);
}
