#include "CellGroup.h"
#include "Cell.h"
#include "utils.h"

#include <algorithm>
#include <cmath>
#include <tuple>
#include <iostream>
#include <bitset>

using std::vector;
using std::tuple;
using std::shared_ptr;
using std::for_each;
using std::cout;
using std::endl;

CellGroup::CellGroup(int gridSize, GroupType groupType) : isSolved(false), id(-1), type(groupType)
{
	cells.reserve(gridSize);
}

void CellGroup::addCell(shared_ptr<Cell> newPCell)
{
	cells.push_back(newPCell);
}

void CellGroup::updateCell(int cellId, int digitBit)
{
	auto newEnd = std::remove_if(cells.begin(), cells.end(), [cellId](shared_ptr<Cell> pCell)
		{
			Cell& cell = *pCell;
			return cell.getId() == cellId;
		}
	);
	cells.erase(newEnd, cells.end());
	for_each(cells.begin(), cells.end(), [digitBit](shared_ptr<Cell> pCell)
		{
			Cell& cell = *pCell;
			cell.removeCandidates(digitBit);
		}
	);
	if (cells.size() == 0) {
		isSolved = true;
	}
}

bool CellGroup::removeCandidates(const vector<int>& cellIds, int candidates) {
	bool candidatesChanged = false;
	for_each(cellIds.begin(), cellIds.end(), [this, candidates, &candidatesChanged](int cellId)
		{
			auto cellIterator = std::find_if(cells.begin(), cells.end(), [cellId](shared_ptr<Cell> pCell)
				{
					Cell& cell = *pCell;
					return cell.getId() == cellId;
				}
			);
			int cellPos = std::distance(cells.begin(), cellIterator);
			bool candidateRemoved = getCell(cellPos).removeCandidates(candidates);
			if (candidateRemoved == true) { candidatesChanged = true; }
		}
	);
	return candidatesChanged;
}