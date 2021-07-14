#ifndef __SYMBOL_H__
#define __SYMBOL_H__


namespace calc {

	struct Args {

		Args() : first(0), second(0), third(0) {}
		Args(long double fst = 0, long double snd = 0, long double trd = 0) : first(fst), second(snd), third(trd) {}

		long double first, second, third;
	};

	struct Symbol {

		Symbol() : sType(Null), priv(0) {}
		Symbol(const long double& num) : sType(isNumber), value(num) {}
		Symbol(const unsigned char& op);

		unsigned char sType;
		unsigned char oType;

		const char* symbol;

		union {
			struct {
				long double (*function)(Args);
				int priv;
				unsigned char inputs;
			};
			
			long double value;
		};

		const enum : unsigned char { Null, isOperator, isNumber, isVariable, isBrakets, isImplicit };
		const enum : unsigned char { Addition, Subtraction, Multiplication, Division, Power, SquareRoot, Exponent, Sin, Cos, Tan, OpenBrakets, CloseBrakets };

	};
}

#endif // __SYMBOL_H__