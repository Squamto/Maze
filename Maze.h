#pragma once
#include <array>
#include <string>

class Maze
{
public:
	Maze(unsigned int width, unsigned int height);
	~Maze();

	void generate();

	bool saveToFile(const std::string& filePath) const;

	unsigned int width, height;

	friend std::ostream& operator <<(std::ostream& outs, const Maze& other);

private:
	enum class Direction
	{
		LEFT = -2,
		RIGHT = -1,
		NONE = 0,
		UP = 1,
		DOWN = 2
	};

	bool* m_walls;

	bool canRemoveWall(int x, int y) const;
	Direction seekPath(int x, int y);
	unsigned int toIndex(int x, int y) const;


};

