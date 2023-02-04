# the name of the target program
TARGET  = blockdelta

# other source files and the associated object files (this can be blank)
SRC     = create.cc apply.cc
OBJ     = $(SRC:.cc=.o)

# special include directories
INCLUDE = -I.

# special libraries (none for now)
LIB     =

# select the compiler and flags
CC       = g++
CCFLAGS  =

.SUFFIXES: .cc

# specify how to compile the .c files
.cc.o:
	$(CC) $(CCFLAGS) $(INCLUDE) -c $<

# if you type 'make' without arguments, this is the default
all: $(TARGET)

# specify how to compile the target
$(TARGET): $(OBJ) $(TARGET).cc
	$(CC) $(CFLAGS) $(INCLUDE) $(TARGET).cc $(OBJ) $(LIB) -o $(TARGET)

# remove binaries
clean:
	rm -f $(OBJ) $(TARGET).o $(TARGET)

# remove binaries and other junk
clobber:
	make clean
	rm -f core *~

# this will add the file dependencies below, i.e. it modifies this file
depend:
	makedepend -- $(CFLAGS) -- $(INCLUDE) -- $(SRC) $(TARGET).c

