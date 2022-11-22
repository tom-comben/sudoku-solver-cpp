#pragma once

#include "Puzzle.h";
#include "CellGroup.h";

#include <vector>
#include <functional>

class Solver
{
private:
	Puzzle puzzle;
	template <typename Func> bool loopThroughGroups(Func& func);
	bool findGroupInteraction(CellGroup group);
	bool findHiddenSingleton();
	bool findNakedSubset(CellGroup& group);
	bool removeCandidates(CellGroup& group, const std::vector<int>& cellIds, int candidates);
	bool resolveGroupInteraction(CellGroup& group, GroupType overlapGroupType, int overlapGroupId, int digit);

public:
	Solver(Puzzle& puzzle);
	void solve();
};

