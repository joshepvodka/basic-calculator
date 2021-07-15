#include <string>
#include <array>
#include <iostream>

#include "calculator.hpp"
#include "renderer.hpp"

int main() {

	math::Calculator calc;
	while (1) {
		calc.calculate();
	}
	
	return 0;
}
