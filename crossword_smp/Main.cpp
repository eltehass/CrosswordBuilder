#include "Crossword.h"

int main() {
	Crossword myCross = Crossword(10, 16);
	myCross.readVoc();
	myCross.generate();
	myCross.showCross();

	system("pause");
	return 0;
}