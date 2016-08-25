#OBJS sepecifies which files to compile as part of the project
OBJS = Sniffer.cpp Interface.cpp Packet.cpp

#CC specifies which compiler to use
CC = g++

#COMPILER_FLAGS specifies additional options
COMPILER_FLAGS = -Wall

#LINKER_FLAGS Specifies any libraries to link against
LINKER_FLAGS = -lm

#OBJ_NAME Specifies the name of the executable
OBJ_NAME = Sniffer

#The garget that compiles the executable
all: $(OBJS)
    $(CC) $(OBJS) $(COMPILER_FLAGS) $(LINKER_FLAGS) -o $(OBJ_NAME)
