#pragma once
#include <string>
#include <iostream>
#include <vector>
#include <chrono>

int countSetBits(int n);
std::vector<int> getSetDigits(int n);
int numToBit(int num);
int bitToNum(int bit);
std::string getPuzzleConfig(char puzzleNum);
template <typename Func>
long long timer(Func f)
{
	auto begin = std::chrono::steady_clock::now();
	f();
	auto end = std::chrono::steady_clock::now();
	return std::chrono::duration_cast<std::chrono::milliseconds>(end - begin).count();

}