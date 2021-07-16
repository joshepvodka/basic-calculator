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

		inline bool sintax_error() { return m_syntaxError; }
		
	private:
		
		void reset();

		void getStringExpression();
		
		void tokenizeExpression();
		void flushToken(std::vector<Token>& arr, std::string& str);
		
		void preparseExpression();
		void parseExpression();
		bool parseSigns(std::vector<Token>& expr, int& i);
		bool parseImplicitMultiplication(std::vector<Token>& expr, int& i);
		
		void processRPN();
		void getResult(std::vector<Token>& buf, int i);
		
		void to_string(const std::vector<Token>& expr);
		void to_stringRPN(const std::vector<Token>& expr);

		std::vector<long double> m_results;

		bool m_syntaxError;
		std::string m_inputExpression;
		std::vector<std::vector<Token>> m_tokenExpressions;
		std::vector<std::vector<Token>> m_rpnExpressions;

		std::unordered_map<const char*, long double> m_variables;

	};

}
#endif // __CALCULATOR_H__