#include "Maze.h"

#include <iostream>
#include <fstream>
#include <stack>
#include <random>
#include <vector>

namespace Maze
{

	Maze::Maze(unsigned int width, unsigned int height)
		: width(width), height(height), exitX(0)
	{
		std::random_device dev;
		rng = std::mt19937(dev());

		m_cell = reinterpret_cast<bool*>(std::calloc(width * height, sizeof(bool)));
		if(!m_cell)
		{
			std::cerr << "Failed to allocate Memory for Maze::m_cell" << std::endl;
			return;
		}
	}

	Maze::~Maze()
	{
		std::free(m_cell);
	}

	void Maze::generate(std::vector<Direction>& solution)
	{
		std::stack<int> xStack;
		std::stack<int> yStack;

		bool solved = false;

		int cx = 0, cy = 0;
		xStack.push(cx);
		yStack.push(cy);
		while(!xStack.empty())
		{
			if(!solved && cy == height - 1)
			{
				solved = true;
				exitX = cx;
			}
			Direction dir = seekPath(cx, cy);
			if(dir == Direction::NONE)
			{
				if(!solved)
					solution.pop_back();
				cx = xStack.top();
				cy = yStack.top();
				xStack.pop();
				yStack.pop();
			}
			else
			{
				if(!solved)
					solution.push_back(dir);
				xStack.push(cx);
				yStack.push(cy);
				switch(dir)
				{
					case Direction::LEFT:
						cx -= 1;
						break;
					case Direction::RIGHT:
						cx += 1;
						break;
					case Direction::UP:
						cy -= 1;
						break;
					case Direction::DOWN:
						cy += 1;
						break;
				}
			}
		}
	}

	Direction Maze::seekPath(int x, int y)
	{
		m_cell[toIndex(x, y)] = true;

		int n = 0;
		std::array<Direction, 4> possibleDirections{};

		if(canRemoveWall(x + 1, y))
			possibleDirections[n++] = Direction::RIGHT;
		if(canRemoveWall(x, y - 1))
			possibleDirections[n++] = Direction::UP;
		if(canRemoveWall(x - 1, y))
			possibleDirections[n++] = Direction::LEFT;
		if(canRemoveWall(x, y + 1))
			possibleDirections[n++] = Direction::DOWN;

		if(n == 0)
			return Direction::NONE;


		std::uniform_int_distribution<std::mt19937::result_type> dist(0, n - 1);
		int r = dist(rng);

		return possibleDirections[r];
	}

	bool Maze::canRemoveWall(int x, int y) const
	{
		int count = 0;

		if(x < 0 || y < 0 || x >= width || y >= height || m_cell[toIndex(x, y)])
			return false;

		if(x == 0 || !m_cell[toIndex(x - 1, y)])
			count++;
		if(x == width - 1 || !m_cell[toIndex(x + 1, y)])
			count++;
		if(y == 0 || !m_cell[toIndex(x, y - 1)])
			count++;
		if(y == height - 1 || !m_cell[toIndex(x, y + 1)])
			count++;

		return (count > 2);
	}

	inline unsigned int Maze::toIndex(int x, int y) const
	{
		return x + y * width;
	}

	bool Maze::saveToFile(const std::string& filePath) const
	{
		std::ofstream file(filePath);
		file << *this << std::endl;
		file.close();
		return true;
	}

	std::ostream& operator <<(std::ostream& outs, const Maze& other)
	{
		for(int i = 0; i < other.width + 2; i++)
			if(i == 1)
				outs << "E";
			else
				outs << "W";
		outs << "\n";
		for(int y = 0; y < other.height; y++)
		{
			for(int x = 0; x < other.width; x++)
			{
				if(x == 0)
					outs << "W";

				if(other.m_cell[other.toIndex(x, y)])
					outs << " ";
				else
					outs << "W";

				if(x == other.width - 1)
					outs << "W";
			}
			outs << '\n';
		}

		outs << "W";
		for(int i = 0; i < other.width; i++)
			if(i == other.exitX)
				outs << "X";
			else
				outs << "W";
		outs << "W";

		return outs << std::endl;
	}

}
