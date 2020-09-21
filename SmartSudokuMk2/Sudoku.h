#pragma once
#include "Cell.h"

class Sudoku
{
public:

	//puzzle grid
	Cell grid[9][9];
	//puzzle rows current values (array of length 9 of (vectors of char type))
	std::vector<char> rows[9];
	//puzzle columns current values
	std::vector<char> cols[9];
	//puzzle 3x3s current values
	std::vector<char> threeBy3s[9];
	//indicates if the puzzle has been solved
	bool solved = false;
	//coordinates of the cell with the least possible values
	Cell* smallestOptions;
	//smallest options size
	short smallestOptionsSize = 0;
	//how many estimations have been made
	short estimateDepth = 0;
	//if estimation is currently occuring
	bool estimating = false;
	//solving path used for backtracking
	std::vector<std::vector<Cell*>>path;


	Sudoku(char input[9][9]);
	//prints sudoku
	void PrintSudoku();

	//checks if puzzle is complete
	bool CheckComplete();

	//intialises solving
	void SolveInit();
	//Fills cell
	void FillCell(Cell* cell, char x);
	//empties cell
	void EmptyCell(Cell* cellIn);
	//checks if a char value is possible for a passed vector
	bool Is_x_Possible(std::vector<char>* vectIn, char* valueIn);
	void AddToVector(std::vector<char>* vectPtr, char* cIn);
	//checks whole puzzle and updates the impossibleValues for all cells
	void FindImpossibleValues();
	//after a changed cells updates the impossibleValues for all rows, col or 3x3 and cells which the passed cell belongs to
	void FindImpossibleValues(Cell* cellIn);
	//impossibleValues for all cells in row, col or 3x3 which the cell passed belongs to
	void UpdateImpossibleValues(Cell* cellIn);
	//finds the best cell to solve next.
	Cell* FindNextCell();
	//updates the threeBy3s vector for the gridIndex of the passed cell
	void Check3x3(Cell* cellIn);
	//updates the colls vector for the column of the passed cell
	void CheckCol(Cell* cellIn);
	//updates the rows vector for the row of the passed cell
	void CheckRow(Cell* cellIn);
	//finds empty cell in row
	Cell* FindEmptyInRow(short* rowIndex);
	//finds empty cell in column
	Cell* FindEmptyInCol(short* colIndex);
	//finds empty cell in 3x3
	Cell* FindEmptyIn3x3(short* threeX3Index);
	//Makes estimate if no certain cell
	void Estimate(Cell* cellIn);
	void Backtrack(Cell* cellIn);
	int noOfFills = 0;
};

