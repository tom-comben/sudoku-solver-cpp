#include "utils.h"

#include <cmath>
#include <vector>
#include <string>
#include <chrono>

using std::string;
using std::vector;

int countSetBits(int n) {
	int setBits = 0;
	while (n > 0) {
		setBits++;
		n = n & (n - 1);
	}
	return setBits;
}

vector<int> getSetDigits(int n) {
	vector<int> digits = {};
	while (n > 0) {
		int digitBit = n & ~(n - 1);
		digits.push_back(bitToNum(digitBit));
		n = n & ~digitBit;
	}
	return digits;
}

int numToBit(int num) {
	return std::pow(2, num - 1);
}

int bitToNum(int bit) {
	return std::log2(bit) + 1;
}

std::string getPuzzleConfig(char puzzleNum) {
	switch (puzzleNum) {
	case '1':
		//puzzle: easy, 7th Nov 2022
		return "007350020900042003000010470403000080170080000028005004000004010260000340380007569";
	case '2':
		//puzzle: easy, 15th Nov 2022
		return "060000000870241903000009400010856209000030058083002140600490001000300020000608094";
	case '3':
		//puzzle: easy, 14th Nov 2022
		return "029600007000005000070890650100900300000451200406007008000008193910534020003009700";
	case '4':
		//puzzle: medium, 15th Nov 2022
		return "000006000300102900007000004000720100009008000020064300018000700200097000506030200";
	case '5':
		//puzzle: medium, 14th Nov 2022
		return "760002000000006300800507400400000608390000000000040100007090003508700090040000006";
	case '6':
		//puzzle: medium, 13th Nov 2022
		return "000040050030000498000200000020010700490006805350400000000060000600000500000709102";
	case '7':
		//puzzle: hard, 15th Nov 2022
		return "000000090004009002005010470070000080000703000200840006500000900090060100081000050";
	case '8':
		//puzzle: hard, 14th Nov 2022
		return "004000030060705009300091002008014700007380025000200000006000090000006000520900000";
	case '9':
		//puzzle: hard, 13th Nov 2022
		return "300290000500000900008300000000080010200700500007002064010000053060100720030600000";
	case 't':
		//test puzzle
		return "000000000008000000000000000000000000290014000000000000000000000000800000000000000";
	default:
		return "";
	};
}