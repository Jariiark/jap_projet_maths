APPLI = jap-mhd
SRC = mhd.cpp

CC = gcc
CFLAGS = -Wall -Wextra
OBJ = ${SRC:.cpp=.o}

.c.o :
	$(CC) $(CFLAGS) -c $<

$(APPLI): $(OBJ)
	$(CC) -o $(APPLI) $(OBJ) $(CFLAGS)

clean :
	@rm -f $(APPLI) *.o *~