#include "utils.h"
#include "cconnect.h"
#include <iostream>


//bitcoin amount * bitcoin value in usd
utils::utils(){}
int utils::cryptoValue(double cryptoAmount, double cryptoValue) {
	double convert;

	convert = cryptoValue * cryptoAmount;

	std::cout << convert << std::endl;

	return 0;
}

/*
Percentage difference equals the absolute value of the change in value
divided by the average of the 2 numbers, all multiplied by 100.
We then append the percent sign, %, to designate the % difference.


int percDif(double x, double y) {

	double avg = (x + y) / 2;
	double change = x - y;
	double difference = (change / avg) * 100;

	std::cout << difference << "%" << std::endl;

	return 0;
}*/