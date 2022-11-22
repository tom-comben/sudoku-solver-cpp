#pragma once

#include "Cell.h"

#include <memory>
#include <vector>

enum class GroupType
{
	none,
	row,
	col,
	block,
};

class CellGroup
{
private:
	int id;
	GroupType type;
	std::vector<std::shared_ptr<Cell>> cells;
	bool isSolved;

public:
	CellGroup(int gridSize, GroupType groupType);
	void setId(int groupId) { id = groupId; }
	Cell& getCell(int position) { return *cells[position]; }
	GroupType getType() const { return type; }
	int getId() const { return id; }
	void addCell(std::shared_ptr <Cell> const newPCell);
	void updateCell(int cellId, int digitBit);
	bool getIsSolved() const { return isSolved; }
	bool removeCandidates(const std::vector<int>& cellIds, int candidates);
	std::vector<std::shared_ptr<Cell>> getCells() { return cells; }
};

