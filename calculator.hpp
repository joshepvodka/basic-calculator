#ifndef __CALCULATOR_H__
#define __CALCULATOR_H__

#include <string>
#include <vector>

#include "symbol.hpp"

namespace calc {
	
	class Calculator {

	public:
		Calculator();
		~Calculator();

		void calculateExpression();

	private:

		bool m_syntaxError;
		std::string m_inputExpression;
		std::vector<Symbol> m_expression;

		long double m_result;

		void getExpression();
		void tokenizeIntoSymbols();
		void flushSymbol(std::string& str);
		void preprocessSymbols();
		void calculateSymbols();

	
	};

}
#endif // __CALCULATOR_H__