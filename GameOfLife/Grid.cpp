#include "Grid.h"
#include <iostream>
#include <fstream>
#include <string>
#include <sstream>
#include "Timing.h"

void Grid::openFile(const std::string &fileName, bool measure) {
	measure = measure;
	if (measure)
		timer = Timing::getInstance();	
	
	if(timer != nullptr)
		timer->startSetup();

	std::ifstream thisFile(fileName);
	std::string line;
	if (thisFile.is_open()) {
		// flag to only look for dimensions in first line
		bool firstLine = true;

		// go through the whole file and search load the data
		int lineCounter = 0;
		while (std::getline(thisFile, line)) {

			// if it is the first line initialize dimmensions variables
			if (firstLine)
			{
				// obtain the data for height and width fro the first line of the file
				firstLine = false;
				int commaIndex = line.find(",");
				std::string x = line.substr(0, commaIndex);
				std::string y = line.substr(commaIndex + 1);
				std::stringstream toIntWidth(x);
				toIntWidth >> width;
				std::stringstream toIntHeight(y);
				toIntHeight >> height;

				// now that we know the height and width, initialize arrays for this system
				cellsArray = new bool*[height];
				for (int i = 0; i < height; i++) {
					cellsArray[i] = new bool[width];
				}

				workArray = new bool*[height];
				for (int i = 0; i < height; i++) {
					workArray[i] = new bool[width];
				}
				continue;
			}

			// if it is not the first line, start saving the data to the array
			for (int i = 0; i < width; i++) {
				char thisChar = line.at(i);
				if (thisChar == '.')
					cellsArray[lineCounter][i] = false;
				else
					cellsArray[lineCounter][i] = true;
			}
			lineCounter++;
		}
		thisFile.close();
	}
	else {
		std::cout << "could not open file" << std::endl;
	}

	if (timer != nullptr)
		timer->stopSetup();
}

void Grid::writeResult(const std::string &path) {

	if (timer != nullptr)
		timer->startFinalization();

	std::ofstream thisFile(path);
	if (thisFile.is_open()) {
		thisFile << width << "," << height << std::endl;
		for (int i = 0; i < height; i++) {
			for (int j = 0; j < width; j++) {
				char thisChar;
				if (cellsArray[i][j])
					thisChar = 'x';
				else
					thisChar = '.';
				thisFile << thisChar;
			}
			thisFile << std::endl;
		}
		thisFile.close();
	}
	else {
		std::cout << "could not save file" << std::endl;
	}

	if (timer != nullptr) {
		timer->stopFinalization();
	}
}

void Grid::computeUntilThisGeneration(int generation) {

	if (timer != nullptr)
		timer->startComputation();

	while (currentGeneration < generation) {
		computeNextGeneration();
	}

	if (timer != nullptr)
		timer->stopComputation();
}

void Grid::computeNextGeneration() {
	currentGeneration++;
	for (int i = 0; i < height; i++) {
		for (int j = 0; j < width; j++) {
			workArray[i][j] = isCellAlive(i,j);
		}
	}

	for (int i = 0; i < height; i++) {
		for (int j = 0; j < width; j++) {
			cellsArray[i][j] = workArray[i][j];
		}
	}
}

bool Grid::isCellAlive(int y, int x) {
	int liveNeighbourCounter = 0;
	bool lastState = cellsArray[y][x];
	for (int i = -1; i < 2; i++) {
		int yValue = y + i;
		if (yValue < 0)
			yValue = height - 1;
		else if (yValue >= height)
			yValue = 0;
		for (int j = -1; j < 2; j++) {
			int xValue = x + j;
			if (xValue < 0)
				xValue = width - 1;
			else if (xValue >= width)
				xValue = 0;

			if (xValue == x && yValue == y)
				continue;

			if (cellsArray[yValue][xValue])
				liveNeighbourCounter++;
		}
	}
	if (!lastState && liveNeighbourCounter == 3)
		return true;
	
	if (lastState) {
		if (liveNeighbourCounter == 2 || liveNeighbourCounter == 3)
			return true;
	}
	return false;
}