#include "Maze.h"

#include <iostream>
#include <fstream>
#include <stack>
#include <random>

Maze::Maze(unsigned int width, unsigned int height)
	: width(width), height(height)
{
	m_walls = new bool[width*height];
	for(int y = 0; y < height; y++)
	{
		for(int x = 0; x < width; x++)
		{
			m_walls[toIndex(x, y)] = true;
		}
	}
}

Maze::~Maze()
{
	//free(m_walls);
}

void Maze::generate()
{
	std::stack<int> xStack;
	std::stack<int> yStack;

	int cx = 0, cy = 0;
	xStack.push(cx);
	yStack.push(cy);
	while(!xStack.empty())
	{
		Direction dir = seekPath(cx, cy);
		if(dir == Direction::NONE)
		{
			cx = xStack.top();
			cy = yStack.top();
			xStack.pop();
			yStack.pop();
		}
		else
		{
			xStack.push(cx);
			yStack.push(cy);
			switch(dir)
			{
				case Maze::Direction::LEFT:
					cx -= 1;
					break;
				case Maze::Direction::RIGHT:
					cx += 1;
					break;
				case Maze::Direction::UP:
					cy += 1;
					break;
				case Maze::Direction::DOWN:
					cy -= 1;
					break;
			}
		}
	}
}

Maze::Direction Maze::seekPath(int x, int y)
{
	m_walls[toIndex(x, y)] = false;

	int n = 0;
	std::array<Direction, 4> possibleDirections{};

	if(canRemoveWall(x + 1, y))
		possibleDirections[n++] = Direction::RIGHT;
	if(canRemoveWall(x, y + 1))
		possibleDirections[n++] = Direction::UP;
	if(canRemoveWall(x - 1, y))
		possibleDirections[n++] = Direction::LEFT;
	if(canRemoveWall(x, y - 1))
		possibleDirections[n++] = Direction::DOWN;

	if(n == 0)
		return Direction::NONE;

	std::random_device dev;
	std::mt19937 rng(dev());
	std::uniform_int_distribution<std::mt19937::result_type> dist(0, n-1);

	int r = dist(rng);

	return possibleDirections[r];
}

bool Maze::canRemoveWall(int x, int y) const
{
	int count = 0;

	if(x < 0 || y < 0 || x >= width || y >= height || !m_walls[toIndex(x, y)])
		return false;

	if(x == 0 || m_walls[toIndex(x - 1, y)])
		count++;
	if(x == width - 1 || m_walls[toIndex(x + 1, y)])
		count++;
	if(y == 0 || m_walls[toIndex(x, y - 1)])
		count++;
	if(y == height - 1 || m_walls[toIndex(x, y + 1)])
		count++;

	return count > 2;
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
			if(other.m_walls[other.toIndex(x, y)])
				outs << "W";
			else
				outs << " ";
			if(x == other.width - 1)
				outs << "W";
		}
		outs << '\n';
	}

	bool placed = false;

	outs << "W";
	for(int i = 0; i < other.width; i++)
		if(!placed && !other.m_walls[(other.height-1) * other.width + i])
		{
			outs << "X";
			placed = true;
		}
		else
			outs << "W";
	outs << "W";
	return outs << std::endl;
}
