#pragma once

#include "utils.h"

#include <memory>

class Cell
{
private:
	int candidates;
	int digit;
	int id;
	int row;
	int col;
	int block;

public:
	Cell(int initialDigit, int initialCandidates, int id, int row, int column, int block);
	int getCandidates() const { return candidates; }
	int getDigit() const { return digit; }
	int getRow() const { return row; }
	int getCol() const { return col; }
	int getBlock() const { return block; }
	int getSetBits() const { return countSetBits(candidates); }
	int getId() const { return id; }
	void setCandidates(int newCandidates) { candidates = newCandidates; }
	void setDigit(int newDigit) { digit = newDigit; candidates = 0; }
	bool removeCandidates(int digitBit);
};

