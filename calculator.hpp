#ifndef __CALCULATOR_H__
#define __CALCULATOR_H__

#include <string>
#include <vector>

#include "token.hpp"

namespace calc {
	
	class Calculator {

	public:
		Calculator();
		~Calculator();

		void calculate();
		void to_string();
		void to_stringRPN();

		inline bool sintaxError() { return m_syntaxError; }
		
	private:
		
		void getExpression();
		void tokenizeExpression();
		void flushToken(std::string& str);
		void preparseExpression();
		void parseExpression();
		
		void processRPN();
		void getResult(std::vector<Token>& buf, int i);
		
		bool m_syntaxError;
		std::string m_inputExpression;
		std::vector<Token> m_tokenExpression;
		std::vector<Token> m_rpnExpression;

		long double m_result;
	};

}
#endif // __CALCULATOR_H__