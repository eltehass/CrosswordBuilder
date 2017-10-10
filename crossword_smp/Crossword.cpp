#include "Crossword.h"

Crossword::Crossword(int size, int countOfWords) {
	this->size = size;
	this->countOfWords = countOfWords;
	this->MAP_COUNTER = 2;

	this->arrayCross = new char*[size];
	for (int i = 0; i < size; i++) {
		this->arrayCross[i] = new char[size];
		for (int j = 0; j < size; j++) { this->arrayCross[i][j] = ' '; }
	}

	this->mapPreCross = new int*[size];
	for (int i = 0; i < size; i++) {
		this->mapPreCross[i] = new int[size];
		for (int j = 0; j < size; j++) { this->mapPreCross[i][j] = 0; }
	}

	this->insertedArr = new string[countOfWords]; for (int i = 0; i < countOfWords; i++) { insertedArr[i] = ""; }
	this->isInsertedArray = new bool[50]; for (int i = 0; i < 50; i++) { this->isInsertedArray[i] = false; }
	this->vocabulary = new string[50];
	this->descriptions = new string[50];
}

Crossword::~Crossword() {
	for (int i = 0; i < this->size; i++) { 
		delete[] this->arrayCross[i];
		delete[] this->mapPreCross[i];
	}
	delete[] this->arrayCross;
	delete[] this->mapPreCross;
	delete[] this->insertedArr;
	delete[] this->isInsertedArray;
	delete[] this->vocabulary;
	delete[] this->descriptions;
}

void Crossword::readVoc() {
	ifstream reader("vocabulary.txt");
	string curr;
	reader >> curr;
	for (int i = 0; i < 50; i++) { reader >> curr; this->vocabulary[i] = curr; }
	getline(reader, curr); getline(reader, curr); getline(reader, curr);
	for (int i = 0; i < 50; i++) { getline(reader, curr); this->descriptions[i] = curr; }
	this->sortVoc();
}

void Crossword::sortVoc() {
	for (int i = 0; i < 49; i++) {
		for (int j = i + 1; j < 50; j++) {
			if (vocabulary[i].length() < vocabulary[j].length()) { 
				string tmp = this->vocabulary[i]; this->vocabulary[i] = this->vocabulary[j]; this->vocabulary[j] = tmp; 
				tmp = this->descriptions[i]; this->descriptions[i] = this->descriptions[j]; this->descriptions[j] = tmp;
			}
		}
	}
}

int * Crossword::findPossibleCoordinates(char symb) {
	int * coordinates = new int[2];

	for (int i = 0; i < this->size; i++) 
		for (int j = 0; j < this->size; j++) 
			if (this->arrayCross[i][j] == symb) { coordinates[0] = i; coordinates[1] = j; return coordinates; }
	
	return nullptr;
}

bool Crossword::isPossibleToInsert(string word, int counter, int arrayCounter) {
	for (int i = 0; i < word.length(); i++) {
		int * coordinates = findPossibleCoordinates(word[i]);
		if (coordinates == nullptr) continue;

		int len_1 = i;
		int len_2 = word.length() - i - 1;

		int border_1;
		int border_2;

		//HORIZONTAL INSTERTING
		if (coordinates[1] >= len_1 && (this->size - coordinates[1] - 1) >= len_2) {
			border_1 = coordinates[1] - len_1;
			border_2 = coordinates[1] + len_2;

			bool isPossible;

			if (border_1 >= 0 && border_2 < this->size) {
				isPossible = true;
				for (int i = border_1; i <= border_2; i++) {
					if ((i != coordinates[1]) && this->arrayCross[coordinates[0]][i] != ' ') { isPossible = false; break; }
				}
			}
			else isPossible = false;

			if (isPossible) {
				for (int i = border_1; i <= border_2; i++) { this->arrayCross[coordinates[0]][i] = word[i - border_1]; }
				this->mapPreCross[coordinates[0]][border_1] = this->MAP_COUNTER; this->MAP_COUNTER++;
				//this->insertedArr[counter] = this->descriptions[arrayCounter];
				
				int i = 0;
				while (!this->insertedArr[i]._Equal("")) { i++; }
				int j = 0;
				while (!this->vocabulary[j]._Equal(word)) { j++; };
				this->insertedArr[i] = this->descriptions[j] + " _ horizontal pos";
				
				this->isInsertedArray[arrayCounter] = true;
				return true;
			}
		}

		//VERTICAL INSERTING
		if (coordinates[0] >= len_1 && (this->size - coordinates[0] - 1) >= len_2) {
			border_1 = coordinates[0] - len_1;
			border_2 = coordinates[0] + len_2;

			bool isPossible;

			if (border_1 >= 0 && border_2 < this->size) {
				isPossible = true;
				for (int i = border_1; i <= border_2; i++) {
					if ((i != coordinates[0]) && this->arrayCross[i][coordinates[1]] != ' ') { isPossible = false; break; }
				}
			}
			else isPossible = false;

			if (isPossible) {
				for (int i = border_1; i <= border_2; i++) { this->arrayCross[i][coordinates[1]] = word[i - border_1]; }
				this->mapPreCross[border_1][coordinates[1]] = this->MAP_COUNTER; this->MAP_COUNTER++;
				//this->insertedArr[counter] = this->descriptions[arrayCounter];
				
				int i = 0;
				while (!this->insertedArr[i]._Equal("")) { i++; }
				int j = 0;
				while (!this->vocabulary[j]._Equal(word)) { j++; };
				this->insertedArr[i] = this->descriptions[j] + " _ vertical pos";

				this->isInsertedArray[arrayCounter] = true;
				return true;
			}
		}	 
	}
	
	return false;
}

bool Crossword::insertWordInFreePlace(string word, int counter, int arrayCounter) {
	int length = word.length();

	//HORIZONTAL ADDING
	for (int i = 0; i < this->size; i++) {
		int counter = 0;
		int posY = -1;

		for (int j = 0; j < this->size; j++) {
			if (counter >= length) { break; }

			if (this->arrayCross[i][j] == ' ' && counter == 0) { posY = j; counter++; }
			else if (this->arrayCross[i][j] == ' ' && counter < length) { counter++; }
			else if (this->arrayCross[i][j] != ' ') { posY = -1; counter = 0; }
		}

		if (posY != -1 && counter >= length) {
			for (int k = posY; k < posY + length; k++) { this->arrayCross[i][k] = word[k - posY]; }
			
			this->mapPreCross[i][posY] = this->MAP_COUNTER; this->MAP_COUNTER++;
			//this->insertedArr[counter + 2] = this->descriptions[arrayCounter];
			
			int i = 0;
			while (!this->insertedArr[i]._Equal("")) { i++; }
			int j = 0;
			while (!this->vocabulary[j]._Equal(word)) { j++; };
			this->insertedArr[i] = this->descriptions[j] + " _ horizontal pos";

			this->isInsertedArray[arrayCounter] = true;
			return true;
		}
	}

	//VERTICAL ADDING
	for (int j = 0; j < this->size; j++) {
		int counter = 0;
		int posX = -1;

		for (int i = 0; i < this->size; i++) {
			if (counter >= length) { break; }

			if (this->arrayCross[i][j] == ' ' && counter == 0) { posX = i; counter++; }
			else if (this->arrayCross[i][j] == ' ' && counter < length) { counter++; }
			else if (this->arrayCross[i][j] != ' ') { posX = -1; counter = 0; }
		}

		if (posX != -1 && counter >= length) {
			for (int k = posX; k < posX + length; k++) { this->arrayCross[k][j] = word[k - posX]; }
			
			this->mapPreCross[posX][j] = this->MAP_COUNTER; this->MAP_COUNTER++;
			//this->insertedArr[counter + 2] = this->descriptions[arrayCounter];
			
			int i = 0;
			while (!this->insertedArr[i]._Equal("")) { i++; }
			int j = 0;
			while (!this->vocabulary[j]._Equal(word)) { j++; };
			this->insertedArr[i] = this->descriptions[j] + " _ vertical pos";

			this->isInsertedArray[arrayCounter] = true;
			return true;
		}
	}

	return false;
}

void Crossword::generate() {
	int counter = 0;
	int arrayCounter = 0;
	string tmpWord = this->vocabulary[arrayCounter];

	while (tmpWord.length() > this->size) { 
		arrayCounter++;
		if (arrayCounter >= 50) { cout << "Out of bounds, it is impossible to generate a crossword\n\n\n"; return; }
		tmpWord = this->vocabulary[arrayCounter];
	}
	
	// first word inserting
	for (int i = 0; i < tmpWord.length(); i++) { this->arrayCross[this->size / 2][i] = tmpWord[i]; } 
	this->mapPreCross[this->size / 2][0] = counter + 1; 
	this->insertedArr[counter] = this->descriptions[arrayCounter] + " _ horizontal pos";
	this->isInsertedArray[arrayCounter] = true;
	counter++;

	int firstArrayEntry = arrayCounter + 1;
	int max10_1 = 0;
	int max10_2 = 0;
	bool flag_form1 = false;

	while (counter != this->countOfWords) {
		bool isInserted = false;

		while (!isInserted) {

			if (this->isInsertedArray[arrayCounter] != true)
				isInserted = this->isPossibleToInsert(tmpWord, counter, arrayCounter);

			if (!isInserted) {
				arrayCounter++;

				if (counter < this->countOfWords && arrayCounter >= 50) { break; }
				if (counter >= this->countOfWords) { cout << "Everything is good!!!)\n\n\n" << endl; return; }

				tmpWord = this->vocabulary[arrayCounter];
			}
			else this->isInsertedArray[arrayCounter] = true;
		}

		if (isInserted) { counter++; arrayCounter++; flag_form1 = false; }

		if (arrayCounter >= 50 && flag_form1 == false) { arrayCounter = firstArrayEntry; flag_form1 = true; }
		else if (arrayCounter >= 50 && max10_2 < 10 && counter < this->countOfWords) {
			max10_2++;
			flag_form1 = false;
			arrayCounter = firstArrayEntry;

			bool isOk = false;
			
			while (arrayCounter < 50) { 
				if (this->isInsertedArray[arrayCounter] == false) { 
					isOk = this->insertWordInFreePlace(this->vocabulary[arrayCounter], counter, arrayCounter); 
				}
				
				if (isOk) { arrayCounter++; counter++; break; }
				else arrayCounter++;
			}

			if (!isOk && (counter < this->countOfWords)) {cout << "Out of bounds, it is impossible to generate a crossword\n\n\n" << "Count =" <<  counter << endl; return; }
		}


		//this->showCross();
		if (this->isInsertedArray[arrayCounter] != true) { tmpWord = this->vocabulary[arrayCounter]; }
	}
}

void Crossword::showCross() {
	for (int i = 0; i < this->size; i++) {
		for (int j = 0; j < this->size; j++) {
			if (this->arrayCross[i][j] != ' ') { cout << this->arrayCross[i][j] << " "; }
			else { cout << "- "; }
		} cout << endl;
	} cout << endl << endl;

	for (int i = 0; i < this->size; i++) {
		for (int j = 0; j < this->size; j++) {
			//cout << this->mapPreCross[i][j] << " ";
			printf("%2d ", this->mapPreCross[i][j]);
		} cout << endl;
	} cout << endl << endl;

	for (int i = 0; i < this->countOfWords; i++) {
		cout << i + 1 << " - " << this->insertedArr[i] << endl;
	}

	ofstream fout("outfile.txt"); // создаём объект класса ofstream для записи и связываем его с файлом cppstudio.txt
	for (int i = 0; i < this->size; i++) {
		for (int j = 0; j < this->size; j++) {
			if (this->arrayCross[i][j] != ' ') { fout << this->arrayCross[i][j] << " "; }
			else { fout << "- "; }
		} fout << endl;
	} fout << endl << endl;

	for (int i = 0; i < this->countOfWords; i++) {
		fout << i + 1 << " - " << this->insertedArr[i] << endl;
	}
	fout.close(); // закрываем файл
}
