#pragma once
#ifndef __CALCULATOR_H__
#define __CALCULATOR_H__

#include <string>
#include <vector>
#include <unordered_map>

#include "expression.hpp"
#include "token.hpp"

namespace math {
	
	class Calculator {

	public:
		Calculator();
		~Calculator();

		void calculate();

		inline bool sintax_error() { return m_syntaxError; }
		
	private:
		
		void reset();

		void getStringExpression();
		
		void tokenizeExpression();
		void flushToken(Expression& arr, std::string& str);
		
		void preparseExpression();
		void parseExpression();
		bool parseSigns(Expression& expr, int& i);
		bool parseImplicitMultiplication(Expression& expr, int& i);
		
		void processRPN();
		void getResult(Expression& buf, int i);
		
		void to_string(const Expression& expr);
		void to_stringRPN(const Expression& expr);

		std::vector<long double> m_results;

		bool m_syntaxError;
		std::string m_inputExpression;
		std::vector<Expression> m_tokenExpressions;
		std::vector<Expression> m_rpnExpressions;

		std::unordered_map<const char*, long double> m_variables;

	};

}
#endif // __CALCULATOR_H__