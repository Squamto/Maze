#pragma once
#include <array>
#include <string>
#include <vector>
#include <random>

namespace Maze
{
	enum class Direction
	{
		NONE,
		LEFT,
		RIGHT,
		UP,
		DOWN
	};

	class Maze
	{
	public:
		Maze(unsigned int width, unsigned int height);
		~Maze();

		void generate(std::vector<Direction>& directions);

		bool saveToFile(const std::string& filePath) const;

		unsigned int width, height;

		friend std::ostream& operator <<(std::ostream& outs, const Maze& other);

	private:
		bool* m_cell;

		int exitX;
		std::mt19937 rng;

		bool canRemoveWall(int x, int y) const;
		Direction seekPath(int x, int y);
		unsigned int toIndex(int x, int y) const;


	};

}
