#pragma once
#include <iostream>
#include <vector>
#include <string>
#include <fstream>

using namespace std;

class Crossword {
public:
	Crossword(int size, int countOfWords);
	~Crossword();
	void readVoc();
	void generate();
	void showCross();
	
private:
	int size;
	int countOfWords;
	char ** arrayCross;
	int ** mapPreCross;
	bool * isInsertedArray;
	int MAP_COUNTER;
	string * insertedArr;
	string * vocabulary;
	string * descriptions;
	void sortVoc();
	int * findPossibleCoordinates(char symb);
	bool isPossibleToInsert(string word, int counter, int arrayCounter);
	bool insertWordInFreePlace(string word, int counter, int arrayCounter);
};