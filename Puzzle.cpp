#include "Puzzle.h"
#include "CellGroup.h"
#include "Cell.h"
#include "utils.h"

#include <iostream>
#include <memory>
#include <algorithm>
#include <cmath>
#include <bitset>
#include <vector>

using std::cout;
using std::endl;
using std::string;
using std::wstring;
using std::vector;
using std::shared_ptr;
using std::make_shared;
using std::for_each;

Puzzle::Puzzle(string puzzleConfig)
	:
	gridSize(sqrt(puzzleConfig.size())),
	cells({}),
	blockPos("000111222000111222000111222333444555333444555333444555666777888666777888666777888"),
	rows(gridSize, CellGroup(gridSize, GroupType::row)),
	cols(gridSize, CellGroup(gridSize, GroupType::col)),
	blocks(gridSize, CellGroup(gridSize, GroupType::block))
{
	buildPuzzle(puzzleConfig);
}

void Puzzle::buildPuzzle(string puzzleConfig) {
	int cellPos = 0;
	cells.reserve(puzzleConfig.size());
	for (int i = 0; i < gridSize; i++) {
		rows[i].setId(i);
		cols[i].setId(i);
		blocks[i].setId(i);
	}
	for_each(puzzleConfig.begin(), puzzleConfig.end(), [this, &cellPos](char n)
		{
			int candidates = n - '0' ? 0 : pow(2, gridSize) - 1;
			int row = cellPos / gridSize;
			int col = cellPos % gridSize;
			int block = blockPos[cellPos] - '0';
			auto pCell = make_shared<Cell>(n - '0', candidates, cellPos, row, col, block);
			cells.push_back(pCell);
			rows[row].addCell(pCell);
			cols[col].addCell(pCell);
			blocks[block].addCell(pCell);
			cellPos++;
		}
	);
	for_each(cells.begin(), cells.end(), [this](shared_ptr<Cell> pCell)
		{
			Cell& cell = *pCell;
			int digit = cell.getDigit();
			if (digit) {
				int digitBit = numToBit(digit);
				rows[cell.getRow()].updateCell(cell.getId(), digitBit);
				cols[cell.getCol()].updateCell(cell.getId(), digitBit);
				blocks[cell.getBlock()].updateCell(cell.getId(), digitBit);
			}
		}
	);
	cout << "Puzzle built" << endl;
};

string Puzzle::getPuzzleOutput() const {
	int cellPos = 0;
	int prevRow = 0;
	for_each(cells.begin(), cells.end(),
		[this, &cellPos, &prevRow](shared_ptr<Cell> pCell)
		{
			Cell& cell = *pCell;
			if (cellPos % gridSize == 0) {
				cout << endl;
			}
			cout << cell.getDigit() << " ";
			cellPos++;
		}
	);
	cout << endl << endl;
	return "";
}