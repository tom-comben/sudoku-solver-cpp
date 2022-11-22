#pragma once
#include "Cell.h"
#include "CellGroup.h"

#include <vector>
#include <string>
#include <memory>

class Puzzle
{
private:
	int gridSize;
	std::string blockPos;
	std::vector<std::shared_ptr<Cell>> cells;
	std::vector<CellGroup> rows;
	std::vector<CellGroup> cols;
	std::vector<CellGroup> blocks;

public:
	Puzzle(std::string puzzleConfig);
	void buildPuzzle(std::string puzzleConfig);
	int getGridSize() const { return gridSize; }
	std::vector<std::shared_ptr<Cell>> const getCells() { return cells; };
	std::vector<CellGroup> getRows() { return rows; };
	std::vector<CellGroup> getCols() { return cols; };
	std::vector<CellGroup> getBlocks() { return blocks; };
	CellGroup& getRow(int row) { return rows[row]; };
	CellGroup& getCol(int col) { return cols[col]; };
	CellGroup& getBlock(int block) { return blocks[block]; };
	std::string getPuzzleOutput() const;
};

