#include "utils.h"
#include "cconnect.h"
#include <iostream>


//bitcoin amount * bitcoin value in usd - only works for USDT pairs for now
utils::utils(){}
int utils::cryptoValue(double cryptoAmount, double cryptoValue) {
	double convert;

	convert = cryptoValue * cryptoAmount;

	std::cout << "\n$" << convert << std::endl;

	return 0;
}

//int utils::setBmark() {
	//user types pairs he wants bookmarked - save to array/file for display and
	//reuse with api functions
//}


/*
Percentage difference equals the absolute value of the change in value
divided by the average of the 2 numbers, all multiplied by 100.
*/
double utils::percDif(double x, double y) {

	double avg = (x + y) / 2;
	double change = x - y;
	double difference = (change / avg) * 100;

	//std::cout << difference << "%" << std::endl;

	return difference;
}