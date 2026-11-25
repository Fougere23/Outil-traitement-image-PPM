CC      = gcc
CFLAGS  = -Wall -Wextra -std=c99 -g
SRC     = $(wildcard src/*.c)
OBJ     = $(SRC:.c=.o)
PROG    = convert

# "make" tout court : compile le programme
all: $(PROG)

# Édition des liens : on assemble tous les .o en un exécutable
$(PROG): $(OBJ)
	$(CC) $(CFLAGS) -o $@ $^

# Compilation de chaque .c en .o
src/%.o: src/%.c src/*.h
	$(CC) $(CFLAGS) -c $< -o $@

# "make valgrind" : vérifie les fuites mémoire
valgrind: (PROG)valgrind--leak-check=full./(PROG) images/test.pgm sortie.pgm negatif

# "make clean" : supprime les fichiers générés
clean:
	rm -f src/*.o $(PROG) sortie* web/avant.png web/apres.png web/index.html

.PHONY: all clean valgrind