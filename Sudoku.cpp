/****************************************************************************************************
/
/Created By: Colton Smith
/
/Start Date: August 10, 2016
/
/Last Edited: May 27, 2017
/
/Description: A simplistic Sudoku game with integrated solver using Dancing Links Algorithm. All code
/is original and based on a previous project completed by Colton Smith. 
/
*****************************************************************************************************/

#include <iostream>
#include <fstream>
#include <string>

using namespace std;

//////////////////////////////////////////PROTOTYPES/////////////////////////////////////////////////

//Checks
bool rowCheck(int (&puzzle)[9][9]);
bool columnCheck(int (&puzzle)[9][9]);
bool subGridCheck(int (&puzzle)[9][9]);
bool puzzleComplete(int (&puzzle)[9][9]);
bool puzzleSolved(int (&puzzle)[9][9]);
bool editableCell(int (&originalInput)[9][9], int y, int x);

//Methods
void possibleValuesUpdate(int (&puzzle)[9][9], int (&originalPuzzle)[9][9][9]);
void printPuzzle(int (&puzzle)[9][9]);
void getRow(int (&puzzle)[9][9], int (&result)[9], int row);
void getColumn(int (&puzzle)[9][9], int (&result)[9], int column);
void getSubGrid(int (&puzzle)[9][9], int (&result)[9], int subGrid);
void possibleValues(int (&puzzle)[9][9], int (&originalPuzzle)[9][9][9], int y, int x);

//Details
int count(int number, int (&array)[9]);

////////////////////////////////////////SUDOKU GAME MAIN/////////////////////////////////////////////

int main(){	
	
	//Input variables
	int one, two, three, four, five, six, seven, eight, nine;
	
	//Initalize our puzzle grid
	int puzzle[9][9];
	int solved[9][9];
	int row[9] = {1, 1, 1, 1, 1, 1, 1, 1, 1};

	//Initialize our "original" puzzle which will be used to find solution; third dimension allows for possible values storage
	int originalPuzzle[9][9][9];
	int originalInput[9][9] = {
		{0,0,0,0,0,0,0,0,0},
		{0,0,0,0,0,0,0,0,0},
		{0,0,0,0,0,0,0,0,0},
		{0,0,0,0,0,0,0,0,0},
		{0,0,0,0,0,0,0,0,0},
		{0,0,0,0,0,0,0,0,0},
		{0,0,0,0,0,0,0,0,0},
		{0,0,0,0,0,0,0,0,0},
		{0,0,0,0,0,0,0,0,0}};
		
	char puzzleInput;
	cout << "Enter 'F' to enter a text file with a Sudoku Puzzle in it.\n\nEnter 'T' to manually enter in a Sudoku Puzzle.\n\n";
	cin >> puzzleInput;
	string fileName;
	
	//Handle user's decision to input puzzle or input a file with puzzle saved in it
	do{
		//if the user selects 'F', read in a file with a puzzle in it
		if(puzzleInput == 'F'){
			ifstream file;
			
			//while a puzzle has not been successfully open, loop
			do{
				cout << "Input file name of puzzle: ";
				cin >> fileName;
				cout << "\nImporting Sudoku puzzle...\n";
				
				file.open(fileName);
				
				//if the file is open
				if(file.is_open()){
					//input from the file
					for(int i = 0; i < 9; ++i){
                                		file >> one; puzzle[i][0] = one; if(one != 0){originalInput[i][0] = 1;}
                                		file >> two; puzzle[i][1] = two; if(two != 0){originalInput[i][1] = 1;}
                                		file >> three; puzzle[i][2] = three; if(three != 0){originalInput[i][2] = 1;}
                                		file >> four; puzzle[i][3] = four; if(four != 0){originalInput[i][3] = 1;}
                                		file >> five; puzzle[i][4] = five; if(five != 0){originalInput[i][4] = 1;}
                                		file >> six; puzzle[i][5] = six; if(six != 0){originalInput[i][5] = 1;}
                                		file >> seven; puzzle[i][6] = seven; if(seven != 0){originalInput[i][6] = 1;}
                                		file >> eight; puzzle[i][7] = eight; if(eight != 0){originalInput[i][7] = 1;}
                                		file >> nine; puzzle[i][8] = nine; if(nine != 0){originalInput[i][8] = 1;}
                        		}
					cout << "Sudoku puzzle imported successfully!\n";
				}
				//if file does not exists or could not be opened
				else
					cout << "File does not exist, please try another file...\n";
			}while(!file.is_open());
			
			break;
		}
		//if the user selects 'T', allow user to input puzzle one row at a time
		else if(puzzleInput == 'T'){
	
			cout << "\nEnter a Sudoku Puzzle one line at a time, with numbers separated by a space. Input a zero for a blank space." << endl << endl;
	
			for(int i = 0; i < 9; ++i){
				cin >> one; puzzle[i][0] = one; if(one != 0){originalInput[i][0] = 1;}
				cin >> two; puzzle[i][1] = two; if(two != 0){originalInput[i][1] = 1;}
				cin >> three; puzzle[i][2] = three; if(three != 0){originalInput[i][2] = 1;}
				cin >> four; puzzle[i][3] = four; if(four != 0){originalInput[i][3] = 1;}
				cin >> five; puzzle[i][4] = five; if(five != 0){originalInput[i][4] = 1;}
				cin >> six; puzzle[i][5] = six; if(six != 0){originalInput[i][5] = 1;}
				cin >> seven; puzzle[i][6] = seven; if(seven != 0){originalInput[i][6] = 1;}
				cin >> eight; puzzle[i][7] = eight; if(eight != 0){originalInput[i][7] = 1;}
				cin >> nine; puzzle[i][8] = nine; if(nine != 0){originalInput[i][8] = 1;}
			}
			break;
		}
		
		//if user selects 'E', exit program
		else if(puzzleInput == 'E')
			exit(0);
		else
			cout << "\nPlease enter either 'F', 'T' or 'E'\n\n";
	}while(puzzleInput != 'F' && puzzleInput != 'T');

	//copy original puzzle into a puzzle that the user can manipulate by playing the game
	for(int i = 0; i < 9; ++i){
		for(int j = 0; j < 9; ++j){
			originalPuzzle[i][j][0] = puzzle[i][j];
		}
	}

	cout << endl;

	int selection = 0;
	bool test;

	while(1){
		cout << "Select an action from the following menu:" << endl;
		cout << "1. Play Sudoku" << endl;
		cout << "2. Solve Sudoku" << endl;
		cout << "3. Print Puzzle and Exit" << endl;
		cout << endl;
	
		cin >> selection;
		cout << endl;	
		switch(selection){
			case 1 :
				//Functions for Game:
					//Ability to enter number into a cell: COMPLETE :)
					//Ability to delete number in a cell: COMPLETE :)
					//Ability to check possible numbers for cell if enabled: In Progress; possible numbers available, but user can't access function
					//Ability to give up (shows solution): In Progress; give up complete :/
				cout << "******************************************************************************************************************\n\n";
				cout << "INSTRUCTIONS:\n";
				cout << "Enter 'P' to place a number into a cell, and provide the row and column, followed by the number you want to enter.\nExample: P 0 5 1\n\n";
				cout << "Enter 'D' to remove a number from a cell, and provide the row and column, followed by a zero.\nExample: D 0 5 0\n\n";
				cout << "Enter 'G' and three 0's to give up and reveal the solution.\nExample: G 0 0 0\n\n";
				cout << "Enter 'H' and three 0's to receive a hint (this will reveal the solution to one of the cells).\nExample: H 0 0 0\n\n";
				cout << "Enter 'E' and three 0's to exit to main menu.\nExample: E 0 0 0\n\n";
				cout << "You can replace a cell simply by entering a different number into the cell.\n";
				cout << "0's represent an empty cell.\n\n";
				cout << "GOOD LUCK! :)\n\n";
				cout << "*******************************************************************************************************************\n\n";
				
				printPuzzle(puzzle);
				
				char action;
				int x, y, cellValue, a, b;
				bool noChanges;

				//If the puzzle entered contains empty cells (0's), then they can play the Sudoku, otherwise, print that the puzzle is completed
				if(puzzleComplete(puzzle) == false){
					do{
						//cout << endl;
						cin >> action;
						cin >> x;
						cin >> y;
						cin >> cellValue;
						
						switch(action){
							case 'P':
								if(originalInput[x][y] != 1){
									puzzle[x][y] = cellValue;
									cout << cellValue << " Placed in Row: " << x << " Column: " << y << endl <<  endl;
								}
								else
									cout << "Cannot place value in position of original puzle values...\n" << endl;
								printPuzzle(puzzle);
								break;
							case 'D':
								if(originalInput[x][y] != 1){
									puzzle[x][y] = 0;
									cout << "Row: " << x << " Column: " << y << " Deleted.\n" << endl;
								}
								else
									cout << "Can not delete original puzzle values...\n" << endl;
								printPuzzle(puzzle);	
								break;
							case 'G':
								cout << "\nHere is the puzzle as of now. Solution functionality is coming soon!\n" << endl;
								printPuzzle(puzzle);
								
								break;
							case 'H':
								cout << "\nHint functionality is coming soon!" << endl;
								cout << "\nPossible values for empty cells:\n";
								possibleValuesUpdate(puzzle, originalPuzzle);
								
								break;
							case 'E':
								break;
							default:
								cout << "Please enter a valid command...\n" << endl;
						}
						if(puzzleComplete(puzzle) == true && puzzleSolved(puzzle) == false)
							cout << "Solution entered is invalid... Check your work and enter another commnd!\n" << endl;
					}while(action != 'G' && action != 'E' && (puzzleComplete(puzzle) == false || puzzleSolved(puzzle) == false));
					
					//Now that the puzzle is completed, we want to test if it is correct and display the appropriate message
					if(puzzleComplete(puzzle) == true && puzzleSolved(puzzle) == true){
						cout << "\n*** CONGRATULATIONS! YOU SOLVED THE SUDOKU PUZZLE! :) ***\n";
					}
				}
				else
					cout << "The puzzle entered is already completed!" << endl;				
				
				if(action == 'E')
					break;
				
				exit(0);
			case 2 :
				//Show the solution to the Sudoku puzzle	
				cout << "\nSolution functionality is coming soon!\n" << endl;
				do{
					noChanges = true;
					int possibleValuesArray[] = {0, 0, 0, 0, 0, 0, 0, 0, 0};
					possibleValuesUpdate(puzzle, originalPuzzle);
					for(int i = 0; i < 9; i++){
						for(int j = 0; j < 9; j++){
							if(originalInput[i][j] == 0){
								for(int k = 0; k < 9; k++){
									possibleValuesArray[k] = originalPuzzle[i][j][k];
								}
								if(count(0, possibleValuesArray) == 8){
									cout << "Found a value!" << endl;
									for(int k = 0; k < 9; k++){
										if(possibleValuesArray[k] != 0){
											noChanges = false;
											originalInput[i][j] = 1;
											puzzle[i][j] = possibleValuesArray[k];
											cout << "Cell (" << i << ", " << j << ") -> " << puzzle[i][j] << endl;
											possibleValuesUpdate(puzzle, originalPuzzle);
											break;
										}
									}
								}
							}
						}
					}
							
				}while(!noChanges);
				cout << "\nVerification: \n" 
					<< "Row Check: \t\t" << rowCheck(puzzle) << endl
					<< "Column Check: \t\t" << columnCheck(puzzle) << endl
					<< "Sub-grid Check:  \t" << subGridCheck(puzzle) << endl
					<< "Puzzle Complete Check: \t" << puzzleComplete(puzzle) << endl
					<< "Puzzle Solved Check: \t" << puzzleSolved(puzzle) << endl << endl;

				printPuzzle(puzzle);
				break;
			case 3 :
				//Print puzzle in current state
				printPuzzle(puzzle);
				//Exit game
				exit(0);
			//DEBUG: This will print out Verification for: rowCheck, columnCheck, subGridCheck, puzzleComplete, and puzzleSolved. 1 = good, 0 = bad
			case 0 :
				cout << "Verification: \n" 
                                        << "Row Check: \t\t" << rowCheck(puzzle) << endl
                                        << "Column Check: \t\t" << columnCheck(puzzle) << endl
                                        << "Sub-grid Check:  \t" << subGridCheck(puzzle) << endl
                                        << "Puzzle Complete Check: \t" << puzzleComplete(puzzle) << endl 
                                        << "Puzzle Solved Check: \t" << puzzleSolved(puzzle) << endl << endl;
                                break;
			//DEBUG: This will print a grid that shows where initial values were input that != 0
			case 9 :
				printPuzzle(originalInput);
				break;
			//DEBUG: This will allow us to check if a cell is editable or not. Result of 1 = editable, 0 = !editable
			case 8 :
				cout << "Enter coordinates you wish to check editability of. Example: 0 0" << endl;
				cin >> a;
				cin >> b;

				cout << editableCell(originalInput, b, a) << endl << endl;
				break;
			default:
				cout << "Please select a menu item..." << endl;
		}
	}

	return 0;
}



//////////////////////////////Functions////////////////////////////////////////////


//////////////////////////////Checks///////////////////////////////////////////////

//Check to see if the row doesn't have repeating numbers
bool rowCheck(int (&puzzle)[9][9]){
	bool isOK = true;
	int row[9];
	for(int i = 0; i < 9; ++i){
		getRow(puzzle, row, i);
		for(int k = 1; k <= 9; ++k){
				if(count(k, row) > 1)
					isOK = false;
		} 
	}
	return isOK;
}

//Check to see if the column doesn't have repeating numbers
bool columnCheck(int (&puzzle)[9][9]){
	bool isOK = true;
	int column[9];
        for(int i = 0; i < 9; ++i){
                getColumn(puzzle, column, i);
                for(int k = 1; k <= 9; ++k){
                                if(count(k, column) > 1)
                                        isOK = false;
                }
        }
	return isOK;
}

//Check to see if the subgrid doesn't have repeating numbers
bool subGridCheck(int (&puzzle)[9][9]){
	bool isOK = true;
	int subGrid[9];
	for(int i = 0; i < 9; ++i){
		getSubGrid(puzzle, subGrid, i);
		for(int j = 1; j <= 9; ++j){
			if(count(j, subGrid) > 1)
				isOK = false;
		}
	}
	return isOK;
}

//Check to see that the puzzle has been filled in completely (no zeros left)
bool puzzleComplete(int (&puzzle)[9][9]){
	bool isComplete = true;
	for(int i = 0; i < 9; ++i){
		for(int j = 0; j < 9; ++j){
			if(puzzle[i][j] == 0){
				isComplete = false;
			}
		}
	}
	return isComplete;
}

//Calls our row, column, and sub grid checks to make sure the puzzle is valid
bool puzzleSolved(int (&puzzle)[9][9]){
	bool isSolved = true;
	
	if(puzzleComplete(puzzle)){
		if(rowCheck(puzzle) == false)
			isSolved = false;
		if(columnCheck(puzzle) == false)
			isSolved = false;
		if(subGridCheck(puzzle) == false)
			isSolved = false;
	}
	else
		isSolved = false;

	return isSolved;
}

bool editableCell(int (&originalInput)[9][9], int y, int x){
	bool isEditable = false;
	if(originalInput[y][x] != 1)
		isEditable = true;
	return isEditable;
}
///////////////////////////////Methods////////////////////////////////////////////////
//Update all empty cells with possible values
void possibleValuesUpdate(int (&puzzle)[9][9], int (&originalPuzzle)[9][9][9]){
	for(int i = 0; i < 9; ++i){
		for(int j = 0; j < 9; ++j){
			if(puzzle[i][j] == 0){
				cout << "Cell (" << i << ", " << j << "): ";
				possibleValues(puzzle, originalPuzzle, i, j);
			}
		}
	}
}

//Print out the Sudoku Puzzle
void printPuzzle(int (&puzzle)[9][9]){
	cout << "   |";
	for(int i = 0; i < 9; i++)
	{
		if(i > 9)
			cout << " " << i;
		else
			cout << "  " << i;
	}
	cout << endl << "---+";
	for(int i = 0; i < 9; i++)
		cout << "---";
	cout << endl;

	for(int i = 0; i < 9; i++)
	{
		if(i%3 == 0)
			if(i != 0)
				cout << "---+---------+--------+---------\n";
		if(i < 10)
			cout << " " << i << " |";	
		else
			cout << i << " |";
		for(int j = 0; j < 9; j++)
		{
			if(j%3 == 0)
				if(j != 0)
					cout << "| " << puzzle[i][j];
				else
					cout << "  " << puzzle[i][j];
			else
				cout << "  " << puzzle[i][j];
		}
		cout << endl;
	}
	cout << endl;

/*	This would print the puzzle only as a 2D array
	for(int i = 0; i < 9; ++i){
		for(int j = 0; j < 9; ++j){
			cout << puzzle[i][j] << " ";
		}
		cout << endl;
	}
	cout << endl;
*/
}

//Get a row into an array; Used to verify uniqueness of numbers and grab rows for solving algorithm
void getRow(int (&puzzle)[9][9], int (&result)[9], int row){
	for(int i = 0; i < 9; ++i)
		result[i] = puzzle[row][i];
}

//Get a column into an array; Used to verify uniqueness of numbers and grab columns for solving algorithm
void getColumn(int(&puzzle)[9][9], int (&result)[9], int column){
        for(int i = 0; i < 9; ++i)
                result[i] = puzzle[i][column];
}

//Get a sub grid into an array; Used to verify uniqueness of numbers and grab sub grids for solving algorithm
void getSubGrid(int (&puzzle)[9][9], int (&result)[9], int subGrid){
	switch(subGrid){
		case 0:
			for(int i = 0; i < 9; ++i)
				result[i] = puzzle[i/3][i%3];
			break;
		case 1:
			for(int i = 0; i < 9; ++i)
				result[i] = puzzle[i/3][(i%3)+3];
			break;
		case 2:
			for(int i = 0; i < 9; ++i)
				result[i] = puzzle[i/3][(i%3)+6];
			break;
		case 3:
			for(int i = 0; i < 9; ++i)
				result[i] = puzzle[(i/3)+3][i%3];
			break;
		case 4:
			for(int i = 0; i < 9; ++i)
				result[i] = puzzle[(i/3)+3][(i%3)+3];
			break;
		case 5:
			for(int i = 0; i < 9; ++i)
				result[i] = puzzle[(i/3)+3][(i%3)+6];
			break;
		case 6:
			for(int i = 0; i < 9; ++i)
				result[i] = puzzle[(i/3)+6][i%3];
			break;
		case 7:
			for(int i = 0; i < 9; ++i)
				result[i] = puzzle[(i/3)+6][(i%3)+3];
			break;
		case 8:
			for(int i = 0; i < 9; ++i)
				result[i] = puzzle[(i/3)+6][(i%3)+6];
			break;
	}
}

//Checks for possible values of the cell based on surroundings
void possibleValues(int (&puzzle)[9][9], int (&originalPuzzle)[9][9][9], int y, int x){
	//We need to create four arrays: the row of the cell, the column of the cell, the sub grid of the cell, and the possible values of the cell
	int row[9], column[9], subGrid[9], possibleValuesArray[9] = {1, 2, 3, 4, 5, 6, 7, 8, 9};
	
	//We need to populate the arrays of values already in each row, column and subGrid
	getRow(puzzle, row, y);
	getColumn(puzzle, column, x);
	if(x < 3){
		if(y < 3)
			getSubGrid(puzzle, subGrid, 0);
		else if(y > 5)
			getSubGrid(puzzle, subGrid, 6);
		else
			getSubGrid(puzzle, subGrid, 3);
	}
	else if(x > 5){
		if(y < 3)
			getSubGrid(puzzle, subGrid, 2);
		else if(y > 5)
			getSubGrid(puzzle, subGrid, 8);
		else
			getSubGrid(puzzle, subGrid, 5);
	}
	else{
		if(y < 3)
			getSubGrid(puzzle, subGrid, 1);
		else if(y > 5)
			getSubGrid(puzzle, subGrid, 7);
		else
			getSubGrid(puzzle, subGrid, 4);
	}

	//Now we need to remove all of the unique values that are in each row, column and sub grid from possible values of the cell
	for(int i = 0; i < 9; ++i){
		for(int j = 0; j < 9; ++j){
			if(row[i] != 0 && row[i] == possibleValuesArray[j]){
				possibleValuesArray[j] = 0;
			}
			if(column[i] != 0 && column[i] == possibleValuesArray[j]){
				possibleValuesArray[j] = 0;
			}
			if(subGrid[i] != 0 && subGrid[i] == possibleValuesArray[j]){
				possibleValuesArray[j] = 0;
			}
		}
	}
	//This will print out the values of the row, column, subGrid, and possible values left for the cell
/*
	for(int i = 0; i < 9; ++i){
		cout << row[i] << " ";
		cout << column[i] << " ";
		cout << subGrid[i] << " ";
		cout << possibleValuesArray[i] << " ";
		cout << endl;
	}
*/
	
	for(int i = 0; i < 9; ++i)
		originalPuzzle[y][x][i] = possibleValuesArray[i];
	
	for(int k = 0; k < 9; ++k){
		cout << originalPuzzle[y][x][k] << " ";
	}
	cout << endl;
}

//Count how many times a number appears in an array (used to verify unique numbers in row, column and sub grid)
int count(int number, int (&array)[9]){
	int count = 0;
	for(int i = 0; i < 9; ++i){
		if(array[i] == number)
			count++;
	}
	return count;
}

