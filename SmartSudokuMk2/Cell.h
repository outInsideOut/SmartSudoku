#pragma once
#include <vector>
#include <iostream>
#include "Exception.h"
class Cell
{
public:
	Cell();
	//disable copying
	Cell(const Cell& other) = delete;
	void SetX(short* xIn);
	void SetY(short* yIn);
	short* getX();
	short* getY();
	//will compare passed char against the "impossibleValues" vector
	bool Is_x_Possible(char* x);
	//inserts value in ImpossibleValues if not duplicate
	void PushToImpossibleValues(char valueIn);
	//fills cell with non-locked value
	void Fill(char valueIn);
	//finds a possible value for the cell
	char FindPossibleValue();
	//locks the cell
	void Lock();
	//returns the Cell's locked var
	bool Locked();
	void setThreex3Index(short* indexIn);
	short* get3x3();
	//empty's cell if backtracking
	void Empty();
	//value of the cell
	char value;
	//if the cell has been estimated
	bool estimate;
	//will store the values that cannot be placed into this cell without breaking the system's constraints
	std::vector<char> impossibleValues;

private:
	//coordinates of cell in grid (0-index) 
	short x, y; 
	//index of 3x3 grid 
	short threex3Index; 
	//defines if the cell's value is const 
	bool locked; 
	
};

