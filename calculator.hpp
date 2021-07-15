#ifndef __CALCULATOR_H__
#define __CALCULATOR_H__

#include <string>
#include <vector>
#include <unordered_map>

#include "token.hpp"

namespace math {
	
	class Calculator {

	public:
		Calculator();
		~Calculator();

		void calculate();
		void to_string();
		void to_stringRPN();

		inline bool sintax_error() { return m_syntaxError; }
		
	private:
		
		void getStringExpression();
		void tokenizeExpression();
		void flushToken(std::string& str);
		void preparseExpression();
		void parseExpression();
		
		void processRPN();
		void getResult(std::vector<Token>& buf, int i);
		
		long double m_result;

		bool m_syntaxError;
		std::string m_inputExpression;
		std::vector<Token> m_tokenExpression;
		std::vector<Token> m_rpnExpression;

		std::unordered_map<const char*, long double> m_variables;

	};

}
#endif // __CALCULATOR_H__