#include "Sudoku.h"


Sudoku::Sudoku(char input[9][9]) {
	try {
		if (!GridChecker(input)) {
			throw(ImpossiblePuzzleException("Duplicate value in grid"));
		}
	}
	catch (ImpossiblePuzzleException& e) {
		//msg = " duplicate in row, col or 3x3 "
		//print exception message
		std::cout << e.getMessage() << std::endl;
		//exit program
		std::cout << std::endl << "exiting program" << std::endl;
		exit(0);
	}
	for (short i = 0; i < 9; i++) {

		for (short j = 0; j < 9; j++) {

			//pointer to cell object
			Cell* currCell = &grid[i][j];

			try {
				//if ! ('0' <= char <= '9') 
				if (input[i][j] < 48 || input[i][j] > 57) {
					//create message string for exception
					std::string errorCode = "Invalid character in input grid at (" + std::to_string(j) + ", " + std::to_string(i) + ")";
					//throw exception
					throw (InputException(errorCode));
				}
				//set grid Cell's value to the inputted Grid's value at the same index
				currCell->value = input[i][j];
			} 
			catch (InputException& e) {
				//print exception message
				std::cout << e.getMessage() << std::endl;
				//exit program
				std::cout << std::endl << "exiting program" << std::endl;
				exit(0);
			}

			//if value isn't 0 it will be a constant vbalue so cell's locked == true
			if (input[i][j] != '0') {
				currCell->Lock();
			}
			//set cell's coordinates
			currCell->SetY(&i); //y coord
			currCell->SetX(&j);//x coord

			short rowStart;
			//set cell's 3x3 index
			//there is some simple calculations and conditions here to work out the 3x3 index
			if (i >= 0 && i < 3) {
				rowStart = 0;
			}
			else if (i >= 3 && i < 6) {
				rowStart = 3;
			}
			else {
				rowStart = 6;
			}
			short col;
			if (j >= 0 && j < 3) {
				col = 0;
			}
			else if (j >= 3 && j < 6) {
				col = 1;
			}
			else {
				col = 2;
			}
			short ThreesIndex = rowStart + col;
			currCell->setThreex3Index(&ThreesIndex);

			//Cell* smallestOptions is consciously left uninitialised
			//this is as it will have a cell assigned to it as a pointer, rather than it being dynamic memory
			//initialising now would be uneeded work (memory allocation & construction) for this variable's use

		}

	}
	solved = false;
}

bool Sudoku::GridChecker(char gridIn[][9]) {
	short x = 0;
	short y = 0;
	//pattern repeats in 3s, 3 times
	for (short i = 0; i < 3; i++) {
		for (short j = 0; j < 3; j++) {
			
			char val;

			//CheckRow
			std::vector<char>* tmpList = new std::vector<char>;
			for (int z = 0; z < 9; z++) {
				
				val = gridIn[y][z];
				if (val != '0') {
					if (Is_x_Possible(tmpList, &val)) {
						tmpList->push_back(val);
					}
					else {
						return false;
					}
				}
				
			}
			tmpList->clear();
			//CheckCol
			
			for (int z = 0; z < 9; z++) {
				
				val = gridIn[z][x];
				if (val != '0') {
					if (Is_x_Possible(tmpList, &val)) {
						tmpList->push_back(val);
					}
					else {
						return false;
					}
				}
				
			}
			tmpList->clear();

			//get 3x3 starting point for the cell passed
			short  rowStart = y - (y % 3);
			short  colStart = x - (x % 3);

			
			//loop through all cells in  3x3
			for (short rowIndex = rowStart; rowIndex < (rowStart + 3); rowIndex++) {
				for (short colIndex = colStart; colIndex < (colStart + 3); colIndex++) {
					//if the cell is still unsolved
					
					val = gridIn[colIndex][rowIndex];
					if (val != '0') {
						if (Is_x_Possible(tmpList, &val)) {
							tmpList->push_back(val);
						}
						else {
							return false;
						}
					}
					
				}
			}
			delete tmpList;
			x += 3;
			y++;
		}
		x -= 8;
	}
	
	return true;

}

void Sudoku::SolveInit() {
	//fill vectors for all cells, rows, cols and 3x3s
	FindImpossibleValues();
	//While puzzle is imcomplete
	while (!CheckComplete()) {
		//set a pointer to the next cell to fill
		Cell* c =FindNextCell();
		//if there is more than one option for the best next cell then estimate
		if (c == nullptr) {
			Estimate(smallestOptions);
		}
		//if there is only one option for best next cell
		else {
			char x = '1';
			//find the fill option and fill
			for (short i = 0; i < 8; i++) {
				if (Is_x_Possible(&c->impossibleValues, &x))
					break;
				x++;

			}
			FillCell(c, x);
		}
	}
	PrintSudoku();
}

void Sudoku::PrintSudoku() {
	for (short i = 0; i < 9; i++) {
		if (i % 3 == 0) {
			std::cout << " ========================================= \n";
		}
		else {
			std::cout << " ----------------------------------------- \n";
		}

		for (short j = 0; j < 9; j++) {
			if (j % 3 == 0) {
				std::cout << " || " << grid[i][j].value;
			}
			else {
				std::cout << " | " << grid[i][j].value;
			}
		}
		std::cout << " || \n";
	}
	std::cout << " ========================================= \n";
}

bool Sudoku::CheckComplete()
{
	for (short i = 0; i < 9; i++) {
		for (short j = 0; j < 9; j++) {
			if (grid[j][i].value == '0')
				return false;
		}
	}
	return true;

}

void Sudoku::FillCell(Cell* cellIn, char c) {

	if (c == '0') {
		int a = 1;
	}
	//if a cell is trying to be filled but all possible values are tried
	if (cellIn->impossibleValues.size() == 9) {
		//Backtrack
		Backtrack(cellIn);
		//Update all vectors
		FindImpossibleValues();
		return;
	}
	//Fill Cell with char
	cellIn->Fill(c);
	//update cell's impossible values
	cellIn->PushToImpossibleValues(cellIn->value);
	//get coords of cell
	short* y = cellIn->getY();
	short* x = cellIn->getX();
	short* threesIndex = cellIn->get3x3();
	//Add cell's values to Puzzle's relevant vectors
	AddToVector(&rows[*y], &c);
	AddToVector(&cols[*x], &c);
	AddToVector(&threeBy3s[*threesIndex], &c);

	//update the estiamations path for potential, future backtracking
	if (estimating == true) {
		path[long long (estimateDepth) - 1].push_back(cellIn);
	}
	//update relevant
	FindImpossibleValues(cellIn);
	noOfFills++;
}
//empties cell
void Sudoku::EmptyCell(Cell* cellIn) {
	cellIn->Empty();
	
}
//checks if a char value is possible for a passed vector
bool Sudoku::Is_x_Possible(std::vector<char>* vectIn, char* valueIn) {
	std::vector<char>::iterator it;

	for (it = begin(*vectIn); it != end(*vectIn); it++) {
		if (*it == *valueIn) {
			return false;
		}
	}
	return true;

}

void Sudoku::AddToVector(std::vector<char>* vectPtr, char* cIn) {
	//check vector for cIn value
	if (Is_x_Possible(vectPtr, cIn)) {
		//if cIn not already in vector, add it
		vectPtr->push_back(*cIn);
	}
}

void Sudoku::FindImpossibleValues() {
	//perform checks on all rows, cols and 3x3s
	for (short i = 0; i < 9; i++) {
		cols[i].clear();
		rows[i].clear();
		threeBy3s[i].clear();
	}
	//check: row, col, 3x3 of following cells to check all of each type
		/*

		=========================================
		|| x |   |   ||   |   |   ||   |   |   ||
		-----------------------------------------
		||   |   |   || x |   |   ||   |   |   ||
		-----------------------------------------
		||   |   |   ||   |   |   || x |   |   ||
		=========================================
		||   | x |   ||   |   |   ||   |   |   ||
		-----------------------------------------
		||   |   |   ||   | x |   ||   |   |   ||
		-----------------------------------------
		||   |   |   ||   |   |   ||   | x |   ||
		=========================================
		||   |   | x ||   |   |   ||   |   |   ||
		-----------------------------------------
		||   |   |   ||   |   | x ||   |   |   ||
		-----------------------------------------
		||   |   |   ||   |   |   ||   |   | x ||
		=========================================

		(0,0), (3, 1), (6, 2), (1, 3), (4, 4), (7, 5), (2, 6), (5, 7), (8, 8)

		if the length returned by completeing these is ever 8:
			find and fill the empty cell then restart
		else
			UpdateImpossibleValues for all cells:
				if (Cell.impossibleValues.len() == 8):
					then input the missing value
		*/
		//

	//x + y of points to be checked
	short x = 0;
	short y = 0;

	Cell* tempCell;
	//pattern repeats in 3s, 3 times
	for (short i = 0; i < 3; i++) {
		for (short j = 0; j < 3; j++) {

			/*cout << "\n(" << x << ", " << y << ")";*/
			tempCell = &grid[y][x];


			//update row, col & 3x3 the the tempCell belongs to
			CheckRow(tempCell);
			CheckCol(tempCell);
			Check3x3(tempCell);
			
			x += 3;
			y++;
		}
		x -= 8;
	}

	//if no row, col or 3x3 only had one option, try each cell
	for (short i = 0; i < 9; i++) {
		for (short j = 0; j < 9; j++) {
			tempCell = &grid[i][j];
			if (tempCell->value == '0') {
				UpdateImpossibleValues(tempCell);
				
			}
		}
	}

}

void Sudoku::FindImpossibleValues(Cell* cellIn) {
	short* y = cellIn->getY();
	short* x = cellIn->getX();
	short* threesIndex = cellIn->get3x3();

	//update row, col & 3x3 the the passed cell belongs to
	CheckRow(cellIn);
	CheckCol(cellIn);
	Check3x3(cellIn);

	Cell* tempCell;
	//update cells in row and column
	for (short i = 0; i < 9; i++) {
		//row
		tempCell = &grid[i][*x];
		if (tempCell->value == '0') 
			UpdateImpossibleValues(tempCell);
		//column
		tempCell = &grid[*y][i];
		if (tempCell->value == '0')
			UpdateImpossibleValues(tempCell);
	}
	//get 3x3 starting point for the cell passed
	short  rowStart = *y - (*y % 3);
	short  colStart = *x - (*x % 3);

	//loop through all cells in  3x3, updating their "impvals"
	for (short rowIndex = rowStart; rowIndex < (rowStart + 3); rowIndex++) {
		for (short colIndex = colStart; colIndex < (colStart + 3); colIndex++) {
			//store pointer to cell's value var
			tempCell = &grid[rowIndex][colIndex];
			if (tempCell->value == '0')
				UpdateImpossibleValues(tempCell);
		}
	}
	UpdateImpossibleValues(cellIn);

}

void Sudoku::UpdateImpossibleValues(Cell* cellIn) {
	//std::cout << "updating (" << *cellIn->getX() << ", " << *cellIn->getY() << ")\n";
	short* y = cellIn->getY();
	short* x = cellIn->getX();
	short* threesIndex = cellIn->get3x3();
	cellIn->impossibleValues.clear();
	for (std::vector<char>::iterator it = begin(cols[*x]); it != end(cols[*x]); it++) {
		cellIn->PushToImpossibleValues(*it);
	}
	for (std::vector<char>::iterator it = begin(rows[*y]); it != end(rows[*y]); it++) {
		cellIn->PushToImpossibleValues(*it);
	}
	for (std::vector<char>::iterator it = begin(threeBy3s[*threesIndex]); it != end(threeBy3s[*threesIndex]); it++ ) {
		cellIn->PushToImpossibleValues(*it);
	}


}

//if there is a cell with only one option, a ptr to that cell is returned
// else, the cell with the least options is pointed to with Sudoku::smallestOptions 
		//and a nullptr is returned
Cell* Sudoku::FindNextCell() {
	short length, index;
	char type = 'z';
	Cell* tempCell = &grid[0][0];
	//check for a row, column or 3x3 with 8 values
	for (int j = 0; j < 3; j++) {
		for (int i = 0; i < 9; i++) {
			if (j == 0) {
				length = short(cols[i].size());
				if (length == 8) {
					type = 'c';
					index = i;
					break;
				}
			}
			else if (j == 1) {
				length = short(rows[i].size());
				if (length == 8) {
					type = 'r';
					index = i;
					break;
				}
			}
			else if (j == 2) {
				length = short(threeBy3s[i].size());
				if (length == 8) {
					type = 't';
					index = i;
					break;
				}
			}
		}
	}
	if (type != 'z') {
		if (type == 'c') {
			tempCell = FindEmptyInCol(&index);
		}
		else if (type == 'r') {
			tempCell = FindEmptyInRow(&index);
		} 
		else if (type == 't') {
			tempCell = FindEmptyIn3x3(&index);
		}
		return tempCell;
	}
	
	smallestOptionsSize = 0;
	
	for (short i = 0; i < 9; i++) {
		for (short j = 0; j < 9; j++) {
			tempCell = &grid[i][j];
			if (tempCell->value == '0') {
				
				length = short(tempCell->impossibleValues.size());

				if (length == 8) {
					return tempCell;
				}
				if (length > smallestOptionsSize) {
					smallestOptions = tempCell;
					smallestOptionsSize = length;
				}
			}
			

		}
	}
	return nullptr;

}

void Sudoku::Check3x3(Cell* cellIn) {
	//get cell's coordinates
	short* cellX = cellIn->getX();
	short* cellY = cellIn->getY();

	//get 3x3 starting point for the cell passed
	short  rowStart = *cellY- (*cellY % 3);
	short  colStart = *cellX - (*cellX % 3);

	//loop through all cells in  3x3
	for (short rowIndex = rowStart; rowIndex < (rowStart + 3); rowIndex++) {
		for (short colIndex = colStart; colIndex < (colStart + 3); colIndex++) {
			//store pointer to cell's value var
			char* value = &grid[rowIndex][colIndex].value;
			//if the cell is still unsolved
			if (*value != '0') {
				AddToVector(&threeBy3s[*cellIn->get3x3()], value);
			}
		}
	}
	
}
void Sudoku::CheckCol(Cell* cellIn) {
	//get column position from cell objects coordinates
	short* col = cellIn->getX();
	//loow through the row's cells
	for (short i = 0; i < 9; i++) {
		//temp stor cell's value
		char* tempChar = &grid[i][*col].value;

		//if value isn't '0' try and add it to the row's vector
		if (*tempChar != '0') {
			AddToVector(&cols[*col], tempChar);
		}

	}

}
void Sudoku::CheckRow(Cell* cellIn) {
	//get row position from cell objects coordinates
	short* row = cellIn->getX();
	//loow through the row's cells
	for (short i = 0; i < 9; i++) {
		//temp stor cell's value
		char* tempChar = &grid[*row][i].value;

		//if value isn't '0' try and add it to the row's vector
		if (*tempChar != '0') {
			AddToVector(&rows[*row], tempChar);
		}
	}

}

//finds empty cell in row
Cell* Sudoku::FindEmptyInRow(short* rowIndex) {
	for (short i = 0; i < 9; i++) {
		char value = grid[*rowIndex][i].value;
		if (value == '0') {
			return &grid[*rowIndex][i];
		}
	}
	return nullptr;
}
//finds empty cell in column
Cell* Sudoku::FindEmptyInCol(short* colIndex) {
	for (short i = 0; i < 9; i++) {
		char value = grid[i][*colIndex].value;
		if (value == '0') {
			return &grid[i][*colIndex];
		}
	}
	return nullptr;
}
//finds empty cell in 3x3
Cell* Sudoku::FindEmptyIn3x3(short* threeX3Index) {
	short rowStart;
	short colStart;
	//find starting cell of 3x3
	if (*threeX3Index < 3) {
		rowStart = 0;
	}
	else if (*threeX3Index < 6) {
		rowStart = 3;
	}
	else rowStart = 6;

	if (*threeX3Index % 3 == 0) {
		colStart = 0;
	}
	else if (*threeX3Index % 3 == 1) {
		colStart = 3;
	}
	else colStart = 6;

	for (short rowIndex = rowStart; rowIndex < rowStart + 3; rowIndex++) {
		for (short colIndex = colStart; colIndex < colStart + 3; colIndex++) {
			//store pointer to cell's value var
			char* value = &grid[rowIndex][colIndex].value;
			//if the cell is still unsolved
			if (*value == '0') {
				return &grid[rowIndex][colIndex];
			}
		}
	}
	return nullptr;
}

//Makes estimate if no certain cell
void Sudoku::Estimate(Cell* c) {
	if (c->impossibleValues.size() < 9) {
		char x = '1';
		estimating = true;
		//std::cout << "\nestimating at cell(" << *c->getX() << ", " << *c->getY() << ")\n";
		//loop '1' to '9'
		for (short i = 0; i < 9; i++) {
			//if the value is valid
			if (c->Is_x_Possible(&x) == true) {
				
				//set cell's estimation indicator to true
				c->estimate = true;
				//increase the estimation depth
				estimateDepth += 1;
				//add cell_ptr to path
				path.push_back({ });
				//input the value to the cell
				FillCell(c, x);
				return;
			}
			x++;
		}
	}
	else {
		//backtrack or broken (y) 
		Backtrack(c);
		FindImpossibleValues();
	}

}

void Sudoku::Backtrack(Cell* cellIn) {
	std::vector<Cell*>::iterator it;
	try {
		if (cellIn->value == '0') {
			EmptyCell(cellIn);
			it = path[long long(estimateDepth) - 1].end();
			it -= 1;

			Cell* cellPtr = *it;
			Backtrack(cellPtr);
			return;
		}
		else {
			if (cellIn->estimate) {
				if (estimateDepth == 9) {
					//...
					int i = 1;
				}
				//if there are more values to put in
				if (cellIn->impossibleValues.size() < 9) {
					//put in another estimate

					cellIn->value = '0';
					path.pop_back();
					estimateDepth -= 1;
					Estimate(cellIn);
					return;
				}
				else {
					if (path.size() == 1) {
						//throw impossiblePuzzle exception
						throw (ImpossiblePuzzleException("Estimation options are exhausted!"));
					}
					EmptyCell(cellIn);
					path.pop_back();
					estimateDepth -= 1;
					it = path[long long(estimateDepth) - 1].end();
					it -= 1;
					Cell* cellPtr = *it;
					Backtrack(cellPtr);
					return;

					
				}
			}
			else {
				EmptyCell(cellIn);
				path[long long(estimateDepth) - 1].pop_back();
				it = path[long long(estimateDepth) - 1].end();
				it -= 1;
				Cell* cellPtr = *it;
				Backtrack(cellPtr);
				return;
			}
		}
	}
	catch (ImpossiblePuzzleException& e) {
		//print exception message
		std::cout << e.getMessage() << std::endl;
		//exit program
		std::cout << std::endl << "exiting program" << std::endl;
		exit(0);
	}

}