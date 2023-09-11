CC = gcc
LD = gcc

CFLAGS = -g -Wall
LFLAGS = -lm -s

RM = /bin/rm -f

OBJ = main.o
EXEC = cq

all: $(EXEC)

$(EXEC): $(OBJ)
	$(LD) $(OBJ) $(LFLAGS) -o $(EXEC)

%.o: %.c
	$(CC) $(CFLAGS) -c $<

clean:
	$(RM) $(OBJ) $(EXEC)