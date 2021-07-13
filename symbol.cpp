#include <cmath>
#include "symbol.hpp"

namespace calc {

	long double addition(Args args) {
		return args.first + args.second;
	}

	long double subtraction(Args args) {
		return args.first - args.second;
	}

	long double multiplication(Args args) {
		return args.first * args.second;
	}

	long double division(Args args) {
		return args.first / args.second;
	}

	long double power(Args args) {
		return pow(args.first, args.second);
	}

	long double squareroot(Args args) {
		return sqrt(args.first);
	}

	long double sine(Args args) {
		return sin(args.first);
	}

	long double cosine(Args args) {
		return cos(args.first);
	}

	long double tangent(Args args) {
		return tan(args.first);
	}
	

	Symbol::Symbol(const unsigned char& op) : sType(isOperator), oType(op) {

		switch (op) {
		case Addition:
			symbol = "+";
			function = addition;
			priv = 1;
			inputs = 2;
			break;

		case Subtraction:
			symbol = "-";
			function = subtraction;
			priv = 1;
			inputs = 2;
			break;

		case Multiplication:
			symbol = "*";
			function = multiplication;
			priv = 2;
			inputs = 2;
			break;

		case Division:
			symbol = "/";
			function = division;
			priv = 2;
			inputs = 2;
			break;

		case Power:
			symbol = "^";
			function = power;
			priv = 3;
			inputs = 2;
			break;

		case SquareRoot:
			symbol = "^";
			function = squareroot;
			priv = 3;
			inputs = 1;
			break;

		case Sin:
			symbol = "sin";
			function = sine;
			priv = 4;
			inputs = 1;
			break;

		case Cos:
			symbol = "cos";
			function = cosine;
			priv = 4;
			inputs = 1;
			break;

		case Tan:
			symbol = "tan";
			function = tangent;
			priv = 4;
			inputs = 1;
			break;

		case OpenBrakets:
			sType = isBrakets;
			symbol = "(";
			break;

		case CloseBrakets:
			sType = isBrakets;
			symbol = ")";
			break;
		}
	}
}
