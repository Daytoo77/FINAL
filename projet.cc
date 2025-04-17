#include <string>
#include "jeu.h"
using namespace std;

int main(int argc, char* argv[]) {
	string filename = argv[1];
	Jeu jeu;
	jeu.readFile(filename);

}