#include "Cell.h"

Cell::Cell(int initialDigit, int initialCandidates, int id, int row, int col, int block)
	: digit(initialDigit), candidates(initialCandidates), id(id), row(row), col(col), block(block)
{
}

bool Cell::removeCandidates(int digitBit) {
	int newCandidates = candidates & ~digitBit;
	if (newCandidates != candidates) {
		candidates = newCandidates;
		return true;
	}
	return false;
}