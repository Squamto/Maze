#include "Maze.h"

#include <iostream>
#include <fstream>
#include <chrono>
#include <math.h>

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

	Maze::Maze m(width, height);


	std::vector<Maze::Direction> solution;
	solution.reserve(sqrt(width*width + height*height));

	auto t1 = std::chrono::high_resolution_clock::now();
	m.generate(solution);
	auto t2 = std::chrono::high_resolution_clock::now();

	auto ms_int = std::chrono::duration_cast<std::chrono::milliseconds>(t2 - t1);
	std::cout << "Berechnet in: " << ms_int.count() << "ms" << std::endl;
	m.saveToFile(filePath);

	std::ofstream file("Solution" + filePath);

	file << "[ ";
	for(auto dir : solution)
	{
		switch(dir)
		{
			case Maze::Direction::DOWN: file << "DOWN"; break;
			case Maze::Direction::UP: file << "UP"; break;
			case Maze::Direction::RIGHT: file << "RIGHT"; break;
			case Maze::Direction::LEFT: file << "LEFT"; break;
		}
		file << " ";
	}
	file << "]";
	file.flush();
	file.close();
}