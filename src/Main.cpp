#include "Maze.h"

#include <iostream>
#include <fstream>
#include <chrono>

int main(int argc, char* argv[])
{
	std::string filePath = "out.txt";
	int width = 100, height = 100;
	if(argc > 1)
		filePath = argv[1];
	if(argc == 4)
	{
		width = std::stoi(argv[2]);
		height = std::stoi(argv[3]);
	}
	else
		std::cerr << "Usage: maze <outFile> <width> <height>" << std::endl;

	Maze m(width, height);

	auto t1 = std::chrono::high_resolution_clock::now();
	m.generate();
	auto t2 = std::chrono::high_resolution_clock::now();
	auto ms_int = duration_cast<std::chrono::milliseconds>(t2 - t1);
	std::cout << "Berechnet in: " << ms_int.count() << "ms" << std::endl;
	m.saveToFile(filePath);
}