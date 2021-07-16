#include <vector>
#include <stack>
#include <string>
#include <unordered_map>
#include <iostream>
#include <cmath>

#include "calculator.hpp"

#include "expression.hpp"
#include "token.hpp"
#include "lexer.hpp"

#define start_over i = -1; continue;

namespace math {

	Calculator::Calculator() {
		reset();
	}

	Calculator::~Calculator() {
		m_tokenExpressions.~vector();
		m_rpnExpressions.~vector();
		m_results.~vector();
		m_inputExpression.~basic_string();
	}

	void Calculator::reset() {
		m_syntaxError = false;
		m_inputExpression = std::string();
		m_tokenExpressions = std::vector<Expression>();
		m_rpnExpressions = std::vector<Expression>();
		m_results = std::vector<long double>();
	}

	void Calculator::calculate() {

		reset();

		getStringExpression();
		tokenizeExpression();
		if (m_syntaxError) {
			std::cout << "Syntax Error" << std::endl;
			return;
		}
		preparseExpression();
		if (m_syntaxError) {
			std::cout << "Syntax Error" << std::endl;
			return;
		}
		parseExpression();
		if (m_syntaxError) {
			std::cout << "Syntax Error" << std::endl;
			return;
		}
		
		for(const auto& expr : m_tokenExpressions)
			to_string(expr);
		for(const auto& expr : m_rpnExpressions)
			to_stringRPN(expr);
		
		processRPN();
		if (m_syntaxError) {
			std::cout << "Syntax Error" << std::endl;
			return;
		}
		for(const auto& result : m_results)
			std::cout << "Result: " << result << std::endl;
	}

	void Calculator::getStringExpression() {
		std::getline(std::cin, m_inputExpression);
		if (m_inputExpression.empty())
			getStringExpression();
	}

	void Calculator::tokenizeExpression() {

		std::string tokenBuffer;
		Expression expressionBuffer;

		SymType lastToken = null;

		for (const char& chr : m_inputExpression) {
			const char* nextChar = &chr+1;
			if (m_syntaxError)
				return;
			
			if (isBlank(chr)) {
				flushToken(expressionBuffer, tokenBuffer);
				continue;
			}
			
			if (isSign(chr)) {
				if (lastToken != numeric && isNumber(*nextChar)) {
					lastToken = numeric;
					flushToken(expressionBuffer, tokenBuffer);
					tokenBuffer += chr;
					continue;
				}
			}
			
			if (isNumber(chr)) {
				if (lastToken != numeric) {
					lastToken = numeric;
					flushToken(expressionBuffer, tokenBuffer);
				}
				tokenBuffer += chr;
				continue;
			}

			if (isBrakets(chr)) {
				flushToken(expressionBuffer, tokenBuffer);
				lastToken = brakets;
				tokenBuffer += chr;
				continue;
			}

			if (isOperator(chr)) {
				flushToken(expressionBuffer, tokenBuffer);
				lastToken = op;
				tokenBuffer += chr;
				continue;
			}

			if (isalpha(chr)) {
				if (lastToken != str) {
					lastToken = str;
					flushToken(expressionBuffer, tokenBuffer);
				}
				tokenBuffer += chr;
				continue;
			}

			m_syntaxError = true;
			return;
		}
		flushToken(expressionBuffer, tokenBuffer);
		if(!expressionBuffer.empty())
			m_tokenExpressions.push_back(expressionBuffer);
	}

	void Calculator::flushToken(Expression& expr, std::string& str) {

		if (str.empty())
			return;

		if (isSign(str[0]) && str.size() > 1 && isNumber(str[1])) {
			expr.push_back(Token(std::stold(str)));
			str.clear();
			return;
		}
		
		if (isNumber(str[0])) {
			expr.push_back(Token(std::stold(str)));
			str.clear();
			return;
		}

		if (isOperator(str[0])) {
			if (str == "+") {
				expr.push_back(Token(opPlus));
				str.clear();
				return;
			}

			if (str == "-") {
				expr.push_back(Token(opMinus));
				str.clear();
				return;
			}

			if (str == "*") {
				expr.push_back(Token(opMulti));
				str.clear();
				return;
			}

			if (str == "/") {
				expr.push_back(Token(opDiv));
				str.clear();
				return;
			}

			if (str == "^") {
				expr.push_back(Token(opPower));
				str.clear();
				return;
			}

			if (str == "=") {
				expr.push_back(Token(symEqual));
				str.clear();
				return;
			}

			if (str == ";") {
				//expr.push_back(Token(symSemicolon));
				m_tokenExpressions.push_back(expr);
				expr.clear();
				str.clear();
				return;
			}
		}

		if (isalpha(str[0])) {
			if (str == "sqrt") {
				expr.push_back(Token(opSqrt));
				str.clear();
				return;
			}

			if (str == "sin") {
				expr.push_back(Token(funcSin));
				str.clear();
				return;
			}

			if (str == "cos") {
				expr.push_back(Token(funcCos));
				str.clear();
				return;
			}

			if (str == "tan") {
				expr.push_back(Token(funcTan));
				str.clear();
				return;
			}

			expr.push_back(Token(str.c_str()));
			str.clear();
			return;
		}

		if (str == "(") {
			expr.push_back(Token(openBrakets));
			str.clear();
			return;
		}

		if (str == ")") {
			expr.push_back(Token(closeBrakets));
			str.clear();
			return;
		}

		m_syntaxError = true;
	}

	void Calculator::preparseExpression() {
		
		//expression isnt valid if its just 2 or more number;
		//"1 1" is not valid; "1" is valid;
		for (const auto& expr : m_tokenExpressions) {
			int i = 0;
			for (const auto& token : expr) {
				if (token.type == numLiteral)
					i++;
			}

			if ((i > 1 && i == expr.size()) || !i) {
				m_syntaxError = true;
				return;
			}
		}

		for(auto& expr : m_tokenExpressions){
			for (int i = expr.size(); i > -1; i--) {
				parseSigns(expr, i);
			}

			for (int i = 0; i < expr.size() - 1; i++) {
				if (parseImplicitMultiplication(expr, i)) {
					i = -1;
					continue;
				}
			}
		}
	}

	bool Calculator::parseSigns(Expression& expr, int& i) {

		//make -----1 work
		const Token lastToken = expr.peek(i-1);
		const Token currentToken = expr.peek(i);
		const Token nextToken = expr.peek(i+1);

		if (tokenIsSign(currentToken)) {

			if (!(lastToken.type == numLiteral || lastToken.type == closeBrakets)) {
				if (nextToken.type == opSqrt		||
					nextToken.type == openBrakets	||
					nextToken.type == numVar		||
					tokenIsFunc(nextToken)			) {

					expr.erase(expr.begin() + i);
					expr.insert(expr.begin() + i, Token(opMulti));

					if (currentToken.type == opMinus)
						expr.insert(expr.begin() + i, Token(-1));
					else
						expr.insert(expr.begin() + i, Token(1));
					return true;
				}
				else
				if (nextToken.type == numLiteral) {
					if (currentToken.type == opMinus)
						expr[i + 1].value *= -1;
					expr.erase(expr.begin() + i);
					return true;
				}
			}
		}
		return false;
	}

	bool Calculator::parseImplicitMultiplication(Expression& expr, int& i) {

		//make 2sin90 or 2(1) or 2x possible
		if (expr[i].type == numLiteral) {
			if (expr[i + 1].type == opSqrt || expr[i + 1].type == openBrakets || tokenIsFunc(expr[i + 1])) {
				expr.insert(expr.begin() + (i-- + 1), Token(opMulti));
				return true;
			}
		}
		return false;
	}

	void Calculator::parseExpression() {
		
		std::stack<Token> opStack;
		Expression rpnBuffer;

		for(const auto& expr : m_tokenExpressions) {

			if (expr[0].type == numLiteral) {
				rpnBuffer.push_back(expr[0]);
			}
			else opStack.push(expr[0]);
		
			for (int i = 1; i < expr.size(); i++) {
				Token token = expr[i];
		
				if (token.type == numLiteral) {
					rpnBuffer.push_back(token);
					continue;
				}
				else {
					if (token.type == openBrakets) {
						opStack.push(token);
						continue;
					}
			
					if (token.type == opPower) {
						opStack.push(token);
						continue;
					}
			
					if (tokenIsFunc(token)) {
						opStack.push(token);
						continue;
					}
			
					if (token.type == closeBrakets) {
						while (!opStack.empty() && opStack.top().type != openBrakets) {
							rpnBuffer.push_back(opStack.top());
							opStack.pop();
						}
						if (!opStack.empty() && opStack.top().type == openBrakets) {
							opStack.pop();
							continue;
						}
						else {
							m_syntaxError = true;
							return;
						}
					}
					if (!opStack.empty() && OpPrecedence[token.type] <= OpPrecedence[opStack.top().type]) {
						rpnBuffer.push_back(opStack.top());
					opStack.pop();
					}
					opStack.push(token);
				}
			}
			while (!opStack.empty()) {
				rpnBuffer.push_back(opStack.top());
				opStack.pop();
			}
			for (const auto& token : rpnBuffer) {
				if (tokenIsBrakets(token)) {
					m_syntaxError = true;
					return;
				}
			}
			m_rpnExpressions.push_back(rpnBuffer);
			rpnBuffer.clear();
			opStack = std::stack<Token>();
		}
	}

	void Calculator::processRPN() {

		for(auto& expr : m_rpnExpressions ) {
			while (expr.size() != 1) {
				for (int i = 0; i < expr.size(); i++) {
					if (expr[i].type != numLiteral) {
						getResult(expr, i);
						break;
					}
				}
			}
			m_results.push_back(expr.front().value);
		}
	}

	void Calculator::getResult(Expression& expr, int i) {
		
		switch (expr[i].type) {
		case numLiteral:
			break;

		case opPlus:
			expr[i] = Token(expr[i - 2].value + expr[i - 1].value);
			expr.erase(expr.begin() + (--i));
			expr.erase(expr.begin() + (--i));
			break;

		case opMinus:
			expr[i] = Token(expr[i - 2].value - expr[i - 1].value);
			expr.erase(expr.begin() + (--i));
			expr.erase(expr.begin() + (--i));
			break;

		case opMulti:
			expr[i] = Token(expr[i - 2].value * expr[i - 1].value);
			expr.erase(expr.begin() + (--i));
			expr.erase(expr.begin() + (--i));
			break;

		case opDiv:
			expr[i] = Token(expr[i - 2].value / expr[i - 1].value);
			expr.erase(expr.begin() + (--i));
			expr.erase(expr.begin() + (--i));
			break;

		case opPower:
			expr[i] = Token(pow(expr[i - 2].value, expr[i - 1].value));
			expr.erase(expr.begin() + (--i));
			expr.erase(expr.begin() + (--i));
			break;

		case opSqrt:
			expr[i] = Token(sqrt(expr[i - 1].value));
			expr.erase(expr.begin() + (--i));
			break;

		case funcSin:
			expr[i] = Token(sin(expr[i - 1].value));
			expr.erase(expr.begin() + (--i));
			break;

		case funcCos:
			expr[i] = Token(cos(expr[i - 1].value));
			expr.erase(expr.begin() + (--i));
			break;

		case funcTan:
			expr[i] = Token(tan(expr[i - 1].value));
			expr.erase(expr.begin() + (--i));
			break;

		default:
			break;
		}
	}

	void Calculator::to_string(const Expression& expr) {

		std::cout << "Input: ";
		
		for (const auto& token : expr) {
			switch (token.type) {
			case opPlus:
				std::cout << "+";
				break;

			case opMinus:
				std::cout << "-";
				break;

			case opMulti:
				std::cout << "*";
				break;

			case opDiv:
				std::cout << "/";
				break;

			case opPower:
				std::cout << "^";
				break;

			case opSqrt:
				std::cout << "sqrt";
				break;

			case funcSin:
				std::cout << "sin";
				break;

			case funcCos:
				std::cout << "cos";
				break;

			case funcTan:
				std::cout << "tan";
				break;

			case openBrakets:
				std::cout << "(";
				break;

			case closeBrakets:
				std::cout << ")";
				break;

			case numLiteral:
				std::cout << token.value;
				break;

			case numVar:
				std::cout << token.var;
				break;
			}
			std::cout << " ";
		}
		std::cout << std::endl;
	}

	void Calculator::to_stringRPN(const Expression& expr) {

		std::cout << "Parsed Input: ";
		
		for (const auto& token : expr) {
			switch (token.type) {
			case opPlus:
				std::cout << "+";
				break;

			case opMinus:
				std::cout << "-";
				break;

			case opMulti:
				std::cout << "*";
				break;

			case opDiv:
				std::cout << "/";
				break;

			case opPower:
				std::cout << "^";
				break;

			case opSqrt:
				std::cout << "sqrt";
				break;

			case funcSin:
				std::cout << "sin";
				break;

			case funcCos:
				std::cout << "cos";
				break;

			case funcTan:
				std::cout << "tan";
				break;
				
			case numLiteral:
				std::cout << token.value;
				break;

			case openBrakets:
				std::cout << "(";
				break;

			case closeBrakets:
				std::cout << ")";
				break;
			}
			std::cout << " ";
		}
		std::cout << std::endl;
	}

}

#undef start_over