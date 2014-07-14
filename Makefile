CC=gcc
#AA=-DAA
CFLAGS=-Wall -std=gnu99 -Wconversion -Wextra #$(AA)
#DEBUG=-DDBG_AMB -DDBG_DIFFUSE -DDBG_AMB -DDBG_PIX -DDBG_WORLD -DDBG_FIND -DDBG_HIT 
#DEBUG=-DDBG_AMB -DDBG_HIT -DDBG_PIX -DDBG_WORLD
SOURCES=material.c object.c utils.c projection.c model.c main.c \
	linked_list.c  sphere.c plane.c light.c veclib.c image.c raytrace.c \
	pplane.c psphere.c illuminate.c matlib.c fplane.c tplane.c spotlight.c \
	cylinder.c paraboloid.c cone.c hyperboloid.c
RAYOBJS = main.o object.o utils.o projection.o model.o linked_list.o sphere.o \
			plane.o light.o veclib.o image.o raytrace.o material.o pplane.o \
			psphere.o illuminate.o matlib.o fplane.o tplane.o spotlight.o \
			cylinder.o paraboloid.o cone.o hyperboloid.o
RAYHEADERS = main.h object.h utils.h projection.h model.h linked_list.h \
			 sphere.h plane.h material.h light.h veclib.h image.h raytrace.h \
			 pplane.h psphere.h illuminate.h matlib.h fplane.h tplane.h \
			 spotlight.h cylinder.h paraboloid.h cone.h hyperboloid.h
OUTPUT=ray

INCLUDE = $(RAYHEADERS)

# Target for including absolutely no debugging, including no -g, and no 
# object files in order to remove debug code. 
#
# **Changed to run on the same 
# RAYOBJS as the others, so it now includes debug code if it is uncommented.
all:$(RAYOBJS) $(RAYHEADERS) Makefile
	$(CC) $(CFLAGS) $(SOURCES) $(DEBUG) -lm -o $(OUTPUT)

# Target for creating makefile with -g and full debug code enabled.
ray: $(RAYOBJS) $(RAYHEADERS) Makefile
	$(CC) $(CFLAGS) -o $(OUTPUT) -g $(RAYOBJS) -lm

$(RAYOBJS): $(INCLUDE)

# Target for compiling with no debugging but with clang.
clang:$(SOURCES) $(RAYHEADERS) Makefile
	clang $(CFLAGS) $(SOURCES) -lm -o $(OUTPUT)

# Target for compiling with full debugging in gcc.
gccit: $(SOURCES) $(RAYHEADERS) Makefile
	$(CC) $(CFLAGS) -g $(DEBUG) $(SOURCES) -lm -o $(OUTPUT)

# Target for compiling with full debugging and clang.
clangit:$(SOURCES) $(RAYHEADERS) Makefile
	clang $(CFLAGS) -g $(DEBUG) $(SOURCES) -lm -o $(OUTPUT)

# Target for compiling with full debugging in clang plus an addition debug 
# define for printing the rgb values of pixels.
clangitb:$(SOURCES) $(RAYHEADERS) Makefile 
	clang $(CFLAGS) -g $(DEBUG) -DDBG_BYTES $(SOURCES) -lm -o $(OUTPUT)

# Target for compiling the same as the previous with gcc.
gccitb: $(RAYOBJS) $(RAYHEADERS) Makefile
	$(CC) $(CFLAGS) -g $(DEBUG) -DDBG_BYTES $(SOURCES) -lm -o $(OUTPUT)

clean:
	rm -f *.o *.out *.err ray

.c.o: $<
	-gcc -c $(CFLAGS) $(DEBUG) -g $< 2> $(@:.o=.err)
	cat $*.err
