#include <iostream>
#include "Grid.h"
#include <vector>
#include <string>
#include <sstream>
#include "Timing.h"

int main(int argc, char* argv[])
{
	std::vector<std::string> consoleParams;
	std::string loadFile, saveFile;
	int generations;
	bool measure = false;

	for (int i = 1; i < argc; i++) {
		consoleParams.push_back(argv[i]);
	}

	// find the load file
	for (auto it = consoleParams.begin(); it != consoleParams.end(); it++) {
		if ((*it).compare("--load") == 0) {
			loadFile = *(it + 1);
		}
	}

	// find the save file
	for (auto it = consoleParams.begin(); it != consoleParams.end(); it++) {
		if ((*it).compare("--save") == 0) {
			saveFile = *(it + 1);
		}
	}

	// get the generations to be processed
	for (auto it = consoleParams.begin(); it != consoleParams.end(); it++) {
		if ((*it).compare("--generations") == 0) {
			std::stringstream intValue(*(it + 1));
			intValue >> generations;
		}
	}

	// get wether it has to measure
	for (auto it = consoleParams.begin(); it != consoleParams.end(); it++) {
		if ((*it).compare("--measure") == 0) {
			measure = true;
		}
	}

	Grid myGrid;
	myGrid.openFile(loadFile, measure);
	myGrid.computeUntilThisGeneration(generations);
	myGrid.writeResult(saveFile);
	Timing* timer = Timing::getInstance();
	std::cout << timer->getResults() << std::endl;
}

