#include "Cell.h"

Cell::Cell() {
	value = ' ';
	locked = NULL;
	threex3Index = NULL;
	estimate = NULL;
	x = NULL;
	y = NULL;
}

void Cell::SetX(short* xIn) {
	x = *xIn;
}
void Cell::SetY(short* yIn) {
	y = *yIn;
}
short* Cell::getX() {
	return &x;
}
short* Cell::getY() {
	return &y;
}

bool Cell::Is_x_Possible(char* x) {
	std::vector<char>::iterator it;
	//iterate possible values
	for (it = begin(impossibleValues); it != end(impossibleValues); it++) {
		//if the char parameter has duplicate within Cell's vector
		if (*x == *it) {
			return false;
		}
	}
	//if all possible values have been iterated without a duplicate being found
	return true;

}

//inserts value in ImpossibleValues if not duplicate
void Cell::PushToImpossibleValues(char valueIn) {
	if (Is_x_Possible(&valueIn)) {
		impossibleValues.push_back(valueIn);
	}
}

void Cell::Fill(char valueIn) {
	try {
		if (locked != true) {
			value = valueIn;
		}
		else {
			/*throw new exception;*/
		}
	}
	catch (std::exception e) {
		std::cout << "ERROR:\t Attempt made to change locked cell @(" << x << ", " << y << ")";
	}
}

char Cell::FindPossibleValue() {
	char x = '1';
	for (short i = 0; i < 8; i++) {
		if (Is_x_Possible(&x))
			return x;
		x++;
	}
}

//locks Cell
void Cell::Lock() {
	locked = true;
}
//returns if cell is locked
bool Cell::Locked() {
	return locked;
}

void Cell::setThreex3Index(short* indexIn) {
	threex3Index = *indexIn;
}

short* Cell::get3x3() {
	return &threex3Index;
}

void Cell::Empty() {
	value = '0';
	impossibleValues.clear();
	estimate = false;

}