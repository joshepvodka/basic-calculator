#include <string>
#include <sstream>
#include <cctype>
#include <vector>
#include <iostream>

#include "calculator.hpp"

int main() {


	calc::Calculator calculator;

	while (1) {
		calculator.calculate();
	}
	
	return 0;
}
