
OBJS_TEST = raycaster.c src/world.c

OBJS = main.c src/raycast.c src/utils.c src/world.c

CC = gcc

INCLUDE_PATHS = -IC:\mingw_dev_lib\include\SDL2

LIBRARY_PATHS = -LC:\mingw_dev_lib\lib

COMPILER_FLAGS = -Wall 

LINKER_FLAGS = -lmingw32 -lSDL2main -lSDL2 -lSDL2_image 

OBJ_NAME = raycast

OBJ_NAME_TEST = test

all : $(OBJS)
	$(CC) $(OBJS) $(INCLUDE_PATHS) $(LIBRARY_PATHS) $(COMPILER_FLAGS) $(LINKER_FLAGS) -o $(OBJ_NAME)
compile: $(OBJS_TEST)
	$(CC) $(OBJS_TEST) $(INCLUDE_PATHS) $(LIBRARY_PATHS) $(COMPILER_FLAGS) $(LINKER_FLAGS) -o $(OBJ_NAME_TEST)