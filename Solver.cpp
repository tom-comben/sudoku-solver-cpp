#include "Solver.h"
#include "Cell.h"

#include <vector>
#include <algorithm>
#include <functional>

using std::vector;
using std::for_each;
using std::shared_ptr;

Solver::Solver(Puzzle& puzzle) : puzzle(puzzle) {}

bool Solver::findHiddenSingleton() {
	vector<shared_ptr<Cell>> cells = puzzle.getCells();
	bool puzzleChanged = false;
	for_each(cells.begin(), cells.end(), [this, &puzzleChanged](shared_ptr<Cell> pCell) {
		Cell& cell = *pCell;
		if (cell.getSetBits() == 1) {
			puzzleChanged = true;
			int candidates = cell.getCandidates();
			puzzle.getRow(cell.getRow()).updateCell(cell.getId(), candidates);
			puzzle.getCol(cell.getCol()).updateCell(cell.getId(), candidates);
			puzzle.getBlock(cell.getBlock()).updateCell(cell.getId(), candidates);
			cell.setDigit(bitToNum(candidates));
		}
		}
	);
	return puzzleChanged;
}

bool Solver::removeCandidates(CellGroup& group, const vector<int>& cellIds, int candidates) {
	vector<shared_ptr<Cell>> cells = group.getCells();
	bool candidatesChanged = false;
	for_each(cellIds.begin(), cellIds.end(), [this, &group, &cells, candidates, &candidatesChanged](int cellId)
		{
			auto cellIterator = std::find_if(cells.begin(), cells.end(), [cellId, &group](shared_ptr<Cell> pCell) {
				Cell& cell = *pCell;
				return cell.getId() == cellId;
				}
			);
			int cellPos = std::distance(cells.begin(), cellIterator);
			bool candidateRemoved = group.getCell(cellPos).removeCandidates(candidates);
			if (candidateRemoved == true) { candidatesChanged = true; }
		}
	);
	return candidatesChanged;
}

bool Solver::findNakedSubset(CellGroup& group) {
	if (group.getIsSolved() == true) { return false; }
	vector<shared_ptr<Cell>> cells = group.getCells();
	bool candidatesChanged = false;
	auto removeSubsetCandidates = [this, &group, &cells, &candidatesChanged](vector<int>& const cellIds, int candidates)
	{
		vector<int> invertedCellIds = {};
		for_each(cells.begin(), cells.end(), [cellIds, &invertedCellIds](shared_ptr<Cell> pCell) {
			Cell& cell = *pCell;
			auto iteratorPosition = std::find(cellIds.begin(), cellIds.end(), cell.getId());
			if (iteratorPosition == cellIds.end()) {
				invertedCellIds.push_back(cell.getId());
			}
			}
		);
		if (removeCandidates(group, invertedCellIds, candidates)) {
			candidatesChanged = true;
		}
	};


	auto getNakedSubset = [this, &group, &cells, &removeSubsetCandidates](int position, int totalCandidates, vector<int> sharedCells, auto& getNakedRef) mutable
	{
		if (position == cells.size() - 1) {
			int newCandidates = totalCandidates | group.getCell(position).getCandidates();
			int candidateCount = countSetBits(newCandidates);
			if (candidateCount == sharedCells.size() + 1 && candidateCount < cells.size()) {
				vector<int> newSharedCells = sharedCells;
				newSharedCells.push_back(group.getCell(position).getId());
				removeSubsetCandidates(newSharedCells, newCandidates);
				return;
			}
			return;
		}

		getNakedRef(position + 1, totalCandidates, sharedCells, getNakedRef);

		int newCandidates = totalCandidates | group.getCell(position).getCandidates();
		int candidateCount = countSetBits(newCandidates);
		if (candidateCount < cells.size()) {
			vector<int> newSharedCells = sharedCells;
			newSharedCells.push_back(group.getCell(position).getId());
			if (candidateCount == sharedCells.size() + 1) {
				removeSubsetCandidates(newSharedCells, newCandidates);
			}
			getNakedRef(position + 1, newCandidates, newSharedCells, getNakedRef);
		}
	};
	getNakedSubset(0, 0, vector<int> {}, getNakedSubset);
	return candidatesChanged;
}

bool Solver::resolveGroupInteraction(CellGroup& group, GroupType overlapGroupType, int overlapGroupId, int digit) {
	bool puzzleChanged = false;
	vector<shared_ptr<Cell>> cells = group.getCells();
	for (shared_ptr<Cell> pCell : cells) {
		Cell& cell = *pCell;
		bool isInOverlap = false;
		switch (overlapGroupType) {
		case GroupType::row:
			if (cell.getRow() == overlapGroupId) { isInOverlap = true; }
			break;
		case GroupType::col:
			if (cell.getCol() == overlapGroupId) { isInOverlap = true; }
			break;
		case GroupType::block:
			if (cell.getBlock() == overlapGroupId) { isInOverlap = true; }
			break;
		}
		if (!isInOverlap) {
			bool cellChanged = cell.removeCandidates(numToBit(digit));
			if (cellChanged) {
				puzzleChanged = true;
			}
		}
	}
	return puzzleChanged;
}

bool Solver::findGroupInteraction(CellGroup group) {
	bool puzzleChanged = false;
	GroupType type = group.getType();
	int id = group.getId();
	int gridSize = puzzle.getGridSize();
	vector<int> digitRows(gridSize + 1, 0);
	vector<int> digitCols(gridSize + 1, 0);
	vector<int> digitBlocks(gridSize + 1, 0);
	for (shared_ptr<Cell> pCell : group.getCells()) {
		Cell& cell = *pCell;
		vector<int> digits = getSetDigits(cell.getCandidates());
		for (int digit : digits) {
			digitRows[digit] |= numToBit(cell.getRow() + 1);
			digitCols[digit] |= numToBit(cell.getCol() + 1);
			digitBlocks[digit] |= numToBit(cell.getBlock() + 1);
		}
	}
	for (int iDigit = 1; iDigit < gridSize + 1; iDigit++) {
		if (digitBlocks[iDigit] == 0) { continue; }
		bool rowChanged = false;
		bool colChanged = false;
		bool blockChanged = false;
		if (type == GroupType::block && countSetBits(digitRows[iDigit]) == 1) {
			rowChanged = resolveGroupInteraction(puzzle.getRow(bitToNum(digitRows[iDigit]) - 1), type, id, iDigit);
		}
		if (type == GroupType::block && countSetBits(digitCols[iDigit]) == 1) {
			colChanged = resolveGroupInteraction(puzzle.getCol(bitToNum(digitCols[iDigit]) - 1), type, id, iDigit);
		}
		if (type != GroupType::block && countSetBits(digitBlocks[iDigit]) == 1) {
			blockChanged = resolveGroupInteraction(puzzle.getBlock(bitToNum(digitBlocks[iDigit]) - 1), type, id, iDigit);
		}
		if (rowChanged || colChanged || blockChanged) {
			puzzleChanged = true;
		}
	}
	return puzzleChanged;
}

template <typename Func>
bool Solver::loopThroughGroups(Func& func) {
	bool puzzleChanged = false;
	for (int i = 0; i < puzzle.getGridSize(); i++) {
		if (func(puzzle.getRow(i))) { puzzleChanged = true; }
		if (func(puzzle.getCol(i))) { puzzleChanged = true; }
		if (func(puzzle.getBlock(i))) { puzzleChanged = true; }
	}
	return puzzleChanged;
}

void Solver::solve() {
	bool puzzleChanged = true;
	auto nakedSubsetFunc = std::bind(&Solver::findNakedSubset, this, std::placeholders::_1);
	auto groupInteractionFunc = std::bind(&Solver::findGroupInteraction, this, std::placeholders::_1);
	int it = 0;
	while (puzzleChanged == true) {
		puzzleChanged = false;
		if (findHiddenSingleton()) {
			puzzleChanged = true;
		}
		else if (loopThroughGroups(groupInteractionFunc)) {
			puzzleChanged = true;
		}
		else if (loopThroughGroups(nakedSubsetFunc)) {
			puzzleChanged = true;
		}
		it++;
	}
}
