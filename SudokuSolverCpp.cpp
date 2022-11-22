#include "Puzzle.h"
#include "Solver.h"
#include "utils.h"

#include <iostream>
#include <string>
#include <vector>

using std::string;
using std::cout;
using std::cin;
using std::endl;

int main()
{
	bool escape = false;
	while (escape == false) {
		string puzzleConfig = "";
		char userIn;
		cout << "Select puzzle 1-9" << endl;
		cin >> userIn;
		puzzleConfig = getPuzzleConfig(userIn);
		if (puzzleConfig == "") {
			escape = true;
			continue;
		}
		Puzzle puzzle(puzzleConfig);
		puzzle.getPuzzleOutput();
		Solver solver(puzzle);
		auto solveMilliseconds = timer([&]() { solver.solve(); });
		puzzle.getPuzzleOutput();
		cout << endl << "Solve time: " << solveMilliseconds << "ms" << endl << endl;
	};
	return 0;
}