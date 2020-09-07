# SmartSudoku - A C++ Sudoku-Solving algorithm

## Contents
* [Description](#description)
* [Usage](#usage)
* [Solution Structure](#solution-structure)
    * [Cell](#cell)
    * [Sudoku](#sudoku)
* [Logic](#logic)
* [Conclusion](#Conclusion)
## Description

This is my second iteration of a sudoku-solving algorithm. The first was mainly a practical exercise in learning python and solved the puzzles using a combination of brute force, constraint satisfaction and recursive backtracking. 

This "SmartSudoku" solution is more efficient in multiple manners. It is much faster simply as it is written in C++ with speed in mind, using pointers in place of object copies wherever possible. It is faster also due to the design of the algorithm, which analyses the puzzle grid looking for the next best cell to fill, avoiding unnecessary backtracking and estimation.

The logic for this solution is largely influenced by Crook's Pen and Paper Sudoku Solving Algrithm, found here:  https://www.ams.org/notices/200904/tx090400460p.pdf 

Depending on the complexity of the puzzle (how much estimation is required) the algorith performs between a n and nlog(n) algorithmic complexity.

## Usage

This C++ solution was developed and compiled with, and is best used with Visual Studio 2019. However, the content of the .cpp and .h files could be used to build another solution to be compiled using the compiler of your choice.

The solution can construct objects of the Class "Sudoku", the constructor will take an single argument of a 9x9 char array which is used to represent all cells of the sudoku puzzle to be solved. If a cell of the initial puzzle is empty, the char for that array index should be '0'.

The solution can be used as so (Source.cpp):

```
#pragma once
#include "Sudoku.h"

int main() {
    //char array representing the puzzle to be solved
    char evilArr[][9] = {
        {'0', '0', '0', '0', '0', '0', '0', '0', '0'},
        {'0', '0', '0', '2', '8', '3', '6', '0', '0'},
        {'0', '1', '0', '0', '0', '0', '8', '2', '0'},
        {'0', '0', '0', '9', '2', '5', '0', '7', '0'},
        {'0', '2', '0', '0', '0', '0', '0', '3', '0'},
        {'0', '8', '0', '4', '3', '7', '0', '0', '0'},
        {'0', '3', '9', '0', '0', '0', '0', '6', '0'},
        {'0', '0', '4', '8', '7', '1', '0', '0', '0'},
        {'0', '0', '0', '0', '0', '0', '0', '0', '0'}
    };

    //construct Sudoku object with using that array
    Sudoku Puzzle(evilArr);
    //solve puzzle
    Puzzle.SolveInit();
```
This source file will solve the puzzle "evilArr" and print the solved puzzle to the console

## Solution Structure
This solution contains two classes, Sudoku and Cell. 
### <h3 style="text-align:center">UML</h3>
<a style="padding-left:calc(50% - 180px)" data-flickr-embed="true" href="https://www.flickr.com/photos/190027932@N07/50314799043/in/dateposted-public/" title="SudokuUML"><img src="https://live.staticflickr.com/65535/50314799043_a79e8afa1d_b.jpg" width="365" height="1024" alt="SudokuUML"></a><script async src="//embedr.flickr.com/assets/client-code.js" charset="utf-8"></script>))

### Data 
There are some complicated and unique data structures inside these classes that it is imperitve to understand before one can understand the workings of the program.

#### Cell
- <i>char value</i> - this is the value in the cell. '0' indicates an empty cell, else 1-9
- <i>bool estimate</i> - this indicates if the cell is estimated or not
- <i>vector<char> impossibleValues </i> - This vector is treated like a mathematical set which contains all the values that would break the puzzles constraints if they were to be entered into the cell
- <i>short x, y, 3x3index </i> this stores the geometric coordinates and 3x3 index of the cell within the puzzle grid
- <i> bool locked </i> - this boolean value is an indicator of if the cell's value was filled when the puzzle was initialised and therefore is a const Cell, or if the value is changeable
#### Sudoku
- <i>grid Cell[9][9]</i> - A 9x9 array of Cell Objects to represent the sudoku puzzle<br>
- <i>vector<char>[9] rows </i>, cols, threeBy3s - arrays of length 9 containing vectors of type char representing each row, column and 3x3 grid in the sudoku puzzle. These vectors are filled with all the values currently in the corresponding row, col, or 3x3 grid. This is then used for checking what values can be entered cells within the corresponding set. Also, if the length of one of these vectors 1 8, the algorithm will know that there is only one possible value to fill it with, and choose the empty cell as the next cell to fill.
- <i>bool solved </i>- a boolean indicator of if the puzzle has been solved
- <i>Cell* smallestOptions</i> - a pointer to the cell with the smallest number of fill options, if estimation needs to happen this is the cell that will be estimated on
- <i>short smallestOptionsSize </i>- the number of options for the cell with the smallest options, used for tracking the current smallest length of options when deciding the Cell* smallest options
- <i>short estimateDepth</i> - the number of estimations made when solving the puzzle. This is necessary when traversing the 2D array path when backtracking
- <i>bool estimating</i> - a boolean indicator of if the puxxle is currently making estimations
- <i>vector<vector<Cell*>> </i>- a vector of vectors of pointers to cell objects. This is used to track where in the grid backtracking has taken place. Easch time a new estimation is made a new internal vector is added, whenever a cell is filled for certain whilst the bool estimating == true, then a Cell* is pushed to the top internal vector on the stack.
- <i>int noOfFills </i>- This is a counting variable used for development purposes to track the total number of times cells were filled whilst the puzzle was being solved
<br><br>

## Logic

<h4 style="text-align:center; "><u>Top-Level Logic</u></h4>
<p style="text-align:center; width: 50%; position: relative; left: 25%">This level of logic shows how the sudoku puzzle is solved without any of the smaller considerations or conditions regarding which values will be enetered into a cell, only condiering the top-level functions</p>
<a style="padding-left:calc(50% - 327px)" data-flickr-embed="true" href="https://www.flickr.com/photos/190027932@N07/50315654056/in/dateposted-public/" title="AllLogic"><img src="https://live.staticflickr.com/65535/50315654056_efd1b12ce5_c.jpg" width="654" height="771" alt="AllLogic"></a>
<br><br><br>
<h4 style="text-align:center; "><u>Lower-Level Logic</u></h4>
<p style="text-align:center; width: 50%; position: relative; left: 25%">This level of logic shows how decisions are made within the level above usch as Finding the cell with the smallest number of options, estimating, and backtracking</p>

<a style="padding-left:calc(50% - 360px)" data-flickr-embed="true" href="https://www.flickr.com/photos/190027932@N07/50315682191/in/dateposted-public/" title="FindNextCell"><img src="https://live.staticflickr.com/65535/50315682191_e2693a00e8_o.png" width="711" height="1161" alt="FindNextCell"></a>

<a style="padding-left:calc(50% - 335px)" data-flickr-embed="true" href="https://www.flickr.com/photos/190027932@N07/50315699346/in/dateposted-public/" title="backtrack"><img src="https://live.staticflickr.com/65535/50315699346_978743ed9a_o.png" width="671" height="891" alt="backtrack"></a>

All logic within the program is well commented in the C++ code, these flowcharts are only to help visualise the more complex elements. 

## Conclusion

Whilst a great improvement on the efficiency of the python sudoku-solving algorithm developed previously this solution still has plenty of room for growth and improvement.</br>
Moving forward there will be conversion from vector<char>-datatype sets for the rows, columns, 3x3s and Cells' <i>impossibleValues</i> to the use of arrays, this would make the initial memory requirement of the application larger, but will have no effect on the potential-maximum memory requirement and should, theoretically, improve general execution speeds due to a lack of need for constant dynamic memory allocation. This will be implemented and tested, then updated if an improvement, as expected. </br>

There is the potential for the implementation of a filereader to be useful for inputting puzzles to be solved into the application, possible from .csv files or JSON strings. </br>
This will then lead onto potentially using openCV or some other image-processing library to enable images of sudoku puzzles to be completed using this algorithm.

## Contact

If you want to get in touch about anything in this repository please don't hesitate to drop me a line, my contact details are in the home page of my GITHUB profile.

