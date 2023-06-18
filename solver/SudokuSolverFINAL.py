#Colton Smith
#12-16-2015
#CS375
#Dancing Links Algorithm - Sudoku Solver

import pygame, sys
from pygame.locals import *
from pygame.time import *
from math import *
import numpy
import time

#The following site-packages are needed for this program to work:
# numpy
# pygame

# They can be installed with the following pip commands:
# pip install pygame
# pip install numpy

########################## SETTINGS #############################
#Settings for the size of the grid/Window Size
WINDOWMULTIPLIER = 5 #alter this to change size of grid
WINDOWSIZE = 81
WINDOWWIDTH = WINDOWSIZE * WINDOWMULTIPLIER
WINDOWHEIGHT = (WINDOWSIZE * WINDOWMULTIPLIER) + 30
SQUARESIZE = (WINDOWSIZE * WINDOWMULTIPLIER) // 3
CELLSIZE = SQUARESIZE // 3
NUMBERSIZE = CELLSIZE //3

FPS = 10 #this will update the window 10 times per second

#Standardization of colors used in the game window
WHITE = (255, 255, 255)
BLACK = (0, 0, 0)
LIGHTGRAY = (200, 200, 200)
BLUE = (0, 0, 255)
GREEN = (0, 255, 0)
RED = (255, 0, 0)

############################## DRAW GRID #########################
def drawGrid():
        #CELL LINES
        #Vertical Lines
        for x in range(0, WINDOWWIDTH, CELLSIZE):
                pygame.draw.line(DISPLAYSURF, LIGHTGRAY, (x, 0), (x, WINDOWHEIGHT - 30))
        #Horizontal Lines
        for y in range(0, WINDOWHEIGHT, CELLSIZE):
                pygame.draw.line(DISPLAYSURF, LIGHTGRAY, (0, y), (WINDOWWIDTH, y))
        
        #SQUARE LINES
        #Vertical Lines
        for x in range(0, WINDOWWIDTH, SQUARESIZE):
                pygame.draw.line(DISPLAYSURF, BLACK, (x, 0), (x, WINDOWHEIGHT - 30))
        #Horizontal Lines
        for y in range(0, WINDOWHEIGHT, SQUARESIZE):
                pygame.draw.line(DISPLAYSURF, BLACK, (0, y), (WINDOWWIDTH, y))
        return None

############################# INITIATE CELLS #####################
#This function will create a "dictionary," or list of values that
#can go into each cell

def initiateCells():
        currentGrid = {}
        fullCell = [1, 2, 3, 4, 5, 6, 7, 8, 9]
        #We need to make a copy of the list for each cell
        for xCoord in range(0, 9):
                for yCoord in range(0, 9):
                        currentGrid[xCoord, yCoord] = list(fullCell)
        return currentGrid

############################# DISPLAY CELLS ######################
#Displays remaining numbers in cell
def displayCells(currentGrid):
        #We need an offset so that numbers are not overlapping in
        #each cell
        xFactor = 0
        yFactor = 0
        for item in currentGrid: #item = x and y of cell, 0 - 8
                cellData = currentGrid[item]
                for number in cellData: #iterates through possible numbers
                        if number != ' ':
                                xFactor = ((number - 1) % 3) # 1/4/7 = 0, 2/5/8 = 1, 3/6/9 = 2
                                if number <= 3:
                                        yFactor = 0
                                elif number <= 6:
                                        yFactor = 1
                                else:
                                        yFactor = 2
                                if cellData.count(' ') < 8:
                                        populateCells(number, (item[0]*CELLSIZE) + (xFactor*NUMBERSIZE), (item[1]*CELLSIZE) + (yFactor*NUMBERSIZE), 'small')
                                else:
                                        populateCells(number, (item[0]*CELLSIZE), (item[1]*CELLSIZE), 'large')

        return None

########################### POPULATE CELLS #######################
def populateCells(cellData, x, y, size):
        if size == 'small':
                cellSurf = BASICFONT.render('%s' %(cellData), True, LIGHTGRAY)
        elif size == 'large':
                cellSurf = LARGEFONT.render('%s' %(cellData), True, GREEN)
        solveSurf = SOLVEFONT.render('SOLVE', True, GREEN)
        solveTuple = ((WINDOWWIDTH // 3) + (CELLSIZE // 2), WINDOWHEIGHT - 30)

        cellRect = cellSurf.get_rect()
        cellRect.topleft = (x, y)
        DISPLAYSURF.blit(cellSurf, cellRect)
        DISPLAYSURF.blit(solveSurf, solveTuple)
        
############################# DRAW BOX ###########################
def drawBox(xMouse, yMouse):
        if(yMouse < WINDOWHEIGHT - 30):
                xBox = ((xMouse * 27) // WINDOWWIDTH) * (NUMBERSIZE) #27 is the number of squares
                yBox = ((yMouse * 27) // (WINDOWHEIGHT - 30)) * (NUMBERSIZE)
                pygame.draw.rect(DISPLAYSURF, BLUE, (xBox, yBox, NUMBERSIZE, NUMBERSIZE), 1)

########################## DISPLAY SELECTED NUMBER ###############
def displaySelectedNumber(xMouse, yMouse, currentGrid):
        xNumber = (xMouse * 27) // WINDOWWIDTH #xNumber has a range of 0 - 26
        yNumber = (yMouse * 27) // (WINDOWHEIGHT - 30) #yNumber has a range of 0 - 26
        #Now we need to determine the column of the cell we have clicked in:
        xNumberMod = xNumber % 3
        yNumberMod = yNumber % 3
        if xNumberMod == 0:
                xChoices = [1, 4, 7]
                number = xChoices[yNumberMod]
        elif xNumberMod == 1:
                xChoices = [2, 5, 8]
                number = xChoices[yNumberMod]
        else:
                xChoices = [3, 6, 9]
                number = xChoices[yNumberMod]
        #Wouldn't it be nice to know which cell we are in?
        xCellNumber = xNumber // 3
        yCellNumber = yNumber // 3

        #Now shouldn't we keep track of the numbers we have selected?
        currentState = currentGrid[xCellNumber, yCellNumber]
        incrementer = 0
        while incrementer < 9:
                #if NOT selected number
                if incrementer+1 != number:
                        currentState[incrementer] = ' '
                else:
                        currentState[incrementer] = number
                #update currentGrid
                currentGrid[xCellNumber, yCellNumber] = currentState
                incrementer += 1
        currentGrid = refreshGrid(currentGrid)
        x = str(xCellNumber)
        y = str(yCellNumber)
        final = str(number)
        print("(" + x + ", " + y + ")" + " -> " + final)
        return currentGrid

############################ REFRESH GRID ########################
#If we select a number, and want to change it to another, then the
#grid must be refreshed to it's previous state
def refreshGrid(currentGrid):
        fullCell = [1, 2, 3, 4, 5, 6, 7, 8, 9]
        for xCoord in range(0, 9):
                for yCoord in range(0, 9):
                        cellData = currentGrid[xCoord, yCoord]
                        if cellData.count(' ') < 8:
                                currentGrid[xCoord, yCoord] = list(fullCell)
        return currentGrid

############################## SOLVE SUDOKU ######################
def solveSudoku(currentGrid):
        for item in currentGrid: # item is x,y co-ordinate from 0-8
                cellData = currentGrid[item] # isolates the numbers still available for that cell
                if cellData.count(' ') == 8: # only look at those with one number remaining
                        for number in cellData: # Determine the number there
                                if number != ' ':
                                        updateNumber = number

                        currentGrid = removeX(currentGrid, item, updateNumber)
                        currentGrid = removeY(currentGrid, item, updateNumber)
                        currentGrid = removeGrid(currentGrid, item, updateNumber)
        
        currentGrid = onlyNinX(currentGrid)
        currentGrid = onlyNinY(currentGrid)
        currentGrid = onlyNinGrid(currentGrid)
        
        return currentGrid
############################# REMOVEX ############################
def removeX(currentGrid, item, number):
        for x in range(0, 9):
                if x != item[0]:
                        currentState = currentGrid[(x, item[1])]
                        currentState[number-1] = ' '
                        currentGrid[(x, item[1])] = currentState
        return currentGrid

############################## REMOVEY ###########################
def removeY(currentGrid, item, number):
        for y in range(0, 9):
                if y != item[1]:
                        currentState = currentGrid[(item[0], y)]
                        currentState[number-1] = ' '
                        currentGrid[(item[0], y)] = currentState
        return currentGrid

############################## REMOVE GRID #######################
def removeGrid(currentGrid, item, number):
        if item[0] < 3:
                xGrid = [0, 1, 2]
        elif item[0] > 5:
                xGrid = [6, 7, 8]
        else:
                xGrid = [3, 4, 5]

        if item[1] < 3:
                yGrid = [0, 1, 2]
        elif item[1] > 5:
                yGrid = [6, 7, 8]
        else:
                yGrid = [3, 4, 5]

        #let's iterate through the numbers in the grid
        for x in xGrid:
                for y in yGrid:
                        if (x, y) != item: #all numbers except the one selected
                                currentState = currentGrid[(x, y)]
                                currentState[number-1] = ' '
                                currentGrid[(x, y)] = currentState
        return currentGrid

############################ ONLY N IN X #########################
#Iterate through each row, see if each number is unique
def onlyNinX(currentGrid):
        x = ''
        y = ''
        final = ''
        for item in currentGrid:
                allNumbers = []
                currentNumbers = []
                #find all possible numbers remaining in the row
                for xRange in range(0, 9):
                        for rowNumbers in currentGrid[(xRange, item[1])]:
                                if rowNumbers != ' ':
                                        allNumbers.append(rowNumbers)
                #find numbers remaining in cell
                for cellNumbers in currentGrid[item]:
                        if cellNumbers != ' ':
                                currentNumbers.append(cellNumbers)
                #compare lists; does the number only appear once in the row?
                if len(currentNumbers) > 1:
                        for checkNumber in currentNumbers:
                                if allNumbers.count(checkNumber) == 1:
                                        x = str(item[0])
                                        y = str(item[1])
                                        final = str(checkNumber)
                                        print("(" + x + ", " + y + ")" + " -> " + final)
                                #Now we know that checkNumber appears only once
                                        currentState = currentGrid[item]
                                        for individualNumber in currentState:
                                                if individualNumber != checkNumber and individualNumber != ' ':
                                                        currentState[individualNumber-1] = ' '
                                                        currentGrid[item] = currentState
        return currentGrid
        
############################# ONLY N IN Y ########################
def onlyNinY(currentGrid):
        x = ''
        y = ''
        final = ''
        for item in currentGrid:
                allNumbers = []
                currentNumbers = []
                for yRange in range(0, 9):
                        for columnNumbers in currentGrid[(item[0], yRange)]:
                                if columnNumbers != ' ':
                                        allNumbers.append(columnNumbers)
                for cellNumbers in currentGrid[item]:
                        if cellNumbers != ' ':
                                currentNumbers.append(cellNumbers)
                if len(currentNumbers) > 1:
                        for checkNumber in currentNumbers:
                                if allNumbers.count(checkNumber) == 1:
                                        x = str(item[0])
                                        y = str(item[1])
                                        final = str(checkNumber)
                                        print("(" + x + ", " + y + ")" + " -> " + final)
                                        currentState = currentGrid[item]
                                        for individualNumber in currentState:
                                                if individualNumber != checkNumber and individualNumber != ' ':
                                                        currentState[individualNumber-1] = ' '
                                                        currentGrid[item] = currentState
        return currentGrid

############################# ONLY N IN GRID #####################
def onlyNinGrid(currentGrid):
        x = ''
        y = ''
        final = ''
        for item in currentGrid:
        #determine coordinate as we did before for sub-grid
                if item[0] < 3:
                        xGrid = [0, 1, 2]
                elif item[0] > 5:
                        xGrid = [6, 7, 8]
                else:
                        xGrid = [3, 4, 5]

                if item[1] < 3:
                        yGrid = [0, 1, 2]
                elif item[1] > 5:
                        yGrid = [6, 7, 8]
                else:
                        yGrid = [3, 4, 5]

                allNumbers = []
                currentNumbers = []
                for x in xGrid:
                        for y in yGrid:
                                for gridNumbers in currentGrid[(x, y)]:
                                        if gridNumbers != ' ':
                                                allNumbers.append(gridNumbers)
                for cellNumbers in currentGrid[item]:
                        if cellNumbers != ' ':
                                currentNumbers.append(cellNumbers)
                if len(currentNumbers) > 1:
                        for checkNumber in currentNumbers:
                                if allNumbers.count(checkNumber) == 1:
                                        x = str(item[0])
                                        y = str(item[1])
                                        final = str(checkNumber)
                                        print("(" + x + ", " + y + ")" + " -> " + final)
                                        currentState = currentGrid[item]
                                        for individualNumber in currentState:
                                                if individualNumber != checkNumber and individualNumber != ' ':
                                                        currentState[individualNumber-1] = ' '
                                                        currentGrid[item] = currentState
        return currentGrid

############################# DANCING LINKS ######################
def DancingLinks(currentGrid):
        #fill matrix for solving algorithm
        start_time = time.time()
        SudokuMatrix = numpy.empty((9, 9))
        SudokuMatrix.fill(0)
        for item in currentGrid:
                cellData = currentGrid[item]
                if cellData.count(' ') == 8:
                        for number in cellData:
                                if number != ' ':
                                        SudokuMatrix[item[1]][item[0]] = number
        SudokuMatrixList = []
        CurrentGridList = []
        for y in range(0, 9):
                for x in range(0, 9):
                        SudokuMatrixList.append(SudokuMatrix[y][x])
                        CurrentGridList.append(list(currentGrid[(x, y)]))
        i = 0
        selectedValue = ' '
        while i < 81:
                x = i % 9
                y = i // 9
                #print(y)
                #print(x)
                OK = False
                if SudokuMatrixList[i] == 0: #or CurrentGridList[i].count(' ') != 8:
                        cellData = list(CurrentGridList[i])
                        while ' ' in cellData:
                                cellData.remove(' ')
                        #print(cellData)
                        for value in cellData:
                                #print(value)
                                if rowOK(SudokuMatrix, y, x, value) and columnOK(SudokuMatrix, y, x, value) and gridOK(SudokuMatrix, y, x, value):
                                        SudokuMatrix[y][x] = int(value)
                                        #Value is selected for the  square
                                        cellData.remove(value)
                                        OK = True
                                        i += 1
                                        break
                        #print(cellData)
                        #print(OK)
                        if OK == False:
                                if i == 0:
                                        break
                                SudokuMatrix[y][x] = 0
                                CurrentGridList[i] = list(currentGrid[(x, y)])
                                i -= 1
                                while CurrentGridList[i].count(' ') == 8 and i > 0:
                                        i -= 1
                                if i == 0 and CurrentGridList[i].count(' ') == 8:
                                        break
                                #print(CurrentGridList[i])
                                #print(SudokuMatrix[i//9][i%9])
                                if SudokuMatrix[i//9][i%9] != 0 and len(CurrentGridList[i]) != 0:
                                        CurrentGridList[i].remove(SudokuMatrix[i//9][i%9])
                                if CurrentGridList[i].count(' ') == 8 or len(CurrentGridList[i]) == 0:
                                        CurrentGridList[i] = list(currentGrid[(x, y)])
                                        #print(CurrentGridList[i])
                else:
                        i += 1
        print()
        print("SUDOKU SOLVED! :)")
        print()

        #ALTERNATE PRINTING STYLE
        #print(SudokuMatrix)

        DISPLAYSURF.fill(WHITE)
        displayCells(currentGrid)
        drawGrid()
        for y in range(0, 9):
                for x in range(0, 9):
                        currentGrid[(x, y)] = int(SudokuMatrix[y][x])
                        populateCells(currentGrid[(x, y)], (x*CELLSIZE), (y*CELLSIZE), 'large')

        for y in range(0, 9):
                        print(currentGrid[(0, y)], currentGrid[(1, y)], currentGrid[(2, y)], currentGrid[(3, y)], currentGrid[(4, y)], currentGrid[(5, y)], currentGrid[(6, y)], currentGrid[(7, y)], currentGrid[(8, y)])
        
        return start_time

############################### columnOK #########################
def columnOK(SudokuMatrix, row, column, value):
        SudokuMatrix[row][column] = int(value)
        columnList = []
        isOK = True
        for y in range(0, 9):
                columnList.append(SudokuMatrix[y][column])
        for number in columnList:
                if number != 0:
                        if columnList.count(number) != 1:
                                isOK = False
                                SudokuMatrix[row][column] = 0
        return isOK

############################## rowOK #############################
def rowOK(SudokuMatrix, row, column, value):
        SudokuMatrix[row][column] = int(value)
        rowList = []
        isOK = True
        for x in range(0, 9):
                rowList.append(SudokuMatrix[row][x])
        for number in rowList:
                if number != 0:
                        if rowList.count(number) != 1:
                                isOK = False
                                SudokuMatrix[row][column] = 0
        return isOK

############################### gridOK ###########################
def gridOK(SudokuMatrix, row, column, value):
        SudokuMatrix[row][column] = int(value)
        GridList = []
        isOK = True
        if row < 3:
                yGrid = [0, 1, 2]
        elif row > 5:
                yGrid = [6, 7, 8]
        else: yGrid = [3, 4, 5]

        if column < 3:
                xGrid = [0, 1, 2]
        elif column > 5:
                xGrid = [6, 7, 8]
        else: xGrid = [3, 4, 5]

        for y in yGrid:
                for x in xGrid:
                        GridList.append(SudokuMatrix[y][x])
        for number in GridList:
                if number != 0:
                        if GridList.count(number) != 1:
                                isOK = False
                                SudokuMatrix[row][column] = 0
        return isOK

############################### MAIN #############################
def main():
        global FPSCLOCK, DISPLAYSURF
        pygame.init()
        FPSCLOCK = pygame.time.Clock()
        DISPLAYSURF = pygame.display.set_mode((WINDOWWIDTH, WINDOWHEIGHT))
        mouseClicked = False
        xMouse = 0
        yMouse = 0
        pygame.display.set_caption('Sudoku Solver')

        global BASICFONT, BASICFONTSIZE, LARGEFONT, LARGEFONTSIZE, SOLVEFONT
        BASICFONTSIZE = 15
        LARGEFONTSIZE = 55
        BASICFONT = pygame.font.Font('freesansbold.ttf', BASICFONTSIZE)
        LARGEFONT = pygame.font.Font('freesansbold.ttf', LARGEFONTSIZE)
        SOLVEFONT = pygame.font.Font('freesansbold.ttf', 30)
        currentGrid = initiateCells() #prints 1-9 in each cell
        
        DISPLAYSURF.fill(WHITE)
        displayCells(currentGrid)
        drawGrid()
        
        #pygame require a loop for the game(weird)
        while True:
                mouseClicked = False
                for event in pygame.event.get():
                        if event.type == QUIT:
                                pygame.quit()
                                sys.exit()
                        #Detects mouse motion(PYGAME)
                        elif event.type == MOUSEMOTION:
                                xMouse, yMouse = event.pos
                        #Detects mouse clicks(PYGAME)
                        elif event.type == MOUSEBUTTONDOWN:
                                xMouse, yMouse = event.pos
                                mouseClicked = True
                if mouseClicked == True:
                        if(yMouse > WINDOWHEIGHT - 30):
                                start_time = DancingLinks(currentGrid)
                                print("\n--- %s seconds ---" % (time.time() - start_time))
                                delay(5000)
#                                pygame.quit()
#                                sys.exit()
                        else:
                                #the number selected is printed in the square
                                currentGrid = displaySelectedNumber(xMouse, yMouse, currentGrid)
                solveSudoku(currentGrid)

                #Now we need to update the window for results
                DISPLAYSURF.fill(WHITE)
                displayCells(currentGrid)
                drawGrid()
                #This function will draw a box around the number selected
                drawBox(xMouse, yMouse)

                pygame.display.update()
                FPSCLOCK.tick(FPS)

if __name__=='__main__':
        main()
