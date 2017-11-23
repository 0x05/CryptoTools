#pragma once
#include <string>
#include <map>
class apireq {
public:
	apireq();
	int getBinaAll();
	double getBinaPair(std::string const &pair);
	void listBinaSymbols();

private:
	std::string BINANCE_HOST = "https://www.binance.com/";
	std::map<std::string, double> pairMap;
	double dPrice = 0;
};