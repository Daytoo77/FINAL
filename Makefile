
CC = g++
CFLAGS = -g -Wall -std=c++17

all: projet

tools.o: tools.cc tools.h
	$(CC) $(CFLAGS) -c tools.cc -o tools.o

chaine.o: chaine.cc chaine.h
	$(CC) $(CFLAGS) -c chaine.cc -o chaine.o

jeu.o: jeu.cc jeu.h
	$(CC) $(CFLAGS) -c jeu.cc -o jeu.o

message.o: message.cc message.h
	$(CC) $(CFLAGS) -c message.cc -o message.o

mobile.o: mobile.cc mobile.h
	$(CC) $(CFLAGS) -c mobile.cc -o mobile.o

projet.o: projet.cc
	$(CC) $(CFLAGS) -c projet.cc -o projet.o



projet: chaine.o jeu.o message.o mobile.o projet.o tools.o
	g++ -g -Wall -std=c++17 chaine.o jeu.o message.o mobile.o projet.o tools.o -o projet
# Nettoyage
clean:
	rm -f *.o projet