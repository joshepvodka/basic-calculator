#include <vector>
#include <stack>
#include <string>
#include <iostream>
#include <cmath>

#include "calculator.hpp"

#include "token.hpp"
#include "lexer.hpp"

namespace calc {

	Calculator::Calculator() {

		m_syntaxError = false;
		m_inputExpression = std::string();
		m_tokenExpression = std::vector<Token>();
		m_rpnExpression = std::vector<Token>();
		m_result = 0;
	}





	Calculator::~Calculator() {
		m_tokenExpression.~vector();
		m_rpnExpression.~vector();
		m_inputExpression.~basic_string();
	}





	void Calculator::calculate() {

		m_syntaxError = false;
		m_inputExpression = std::string();
		m_tokenExpression = std::vector<Token>();
		m_rpnExpression = std::vector<Token>();
		m_result = 0;

		getExpression();
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
		
		to_string();
		to_stringRPN();
		
		processRPN();
		if (m_syntaxError) {
			std::cout << "Syntax Error" << std::endl;
			return;
		}
		std::cout << "Result: " << m_result << std::endl;
	}





	void Calculator::getExpression() {
		std::getline(std::cin, m_inputExpression);
		if (m_inputExpression.empty())
			getExpression();
	}





	void Calculator::tokenizeExpression() {

		std::string tokenBuffer;
		SymType lastToken = SymType::null;

		for (const char& chr : m_inputExpression) {
			const char* nextChar = &chr+1;
			if (m_syntaxError)
				return;
			
			if (isblank(chr)) {
				flushToken(tokenBuffer);
				continue;
			}

			if (isSign(chr)) {
				if (lastToken != SymType::numeric && isNumber(*nextChar)) {
					lastToken = SymType::numeric;
					flushToken(tokenBuffer);
					tokenBuffer += chr;
					continue;
				}
			}
			
			if (isNumber(chr)) {
				if (lastToken != SymType::numeric) {
					lastToken = SymType::numeric;
					flushToken(tokenBuffer);
				}
				tokenBuffer += chr;
				continue;
			}

			if (isBrakets(chr)) {
				flushToken(tokenBuffer);
				lastToken = SymType::brakets;
				tokenBuffer += chr;
				continue;
			}

			if (isOperator(chr)) {
				flushToken(tokenBuffer);
				lastToken = SymType::op;
				tokenBuffer += chr;
				continue;
			}

			if (isalpha(chr)) {
				if (lastToken != SymType::str) {
					lastToken = SymType::str;
					flushToken(tokenBuffer);
				}
				tokenBuffer += chr;
				continue;
			}

			m_syntaxError = true;
			return;
		}
		flushToken(tokenBuffer);
	}





	void Calculator::preparseExpression() {
		{
			int i = 0;
			for (const auto& token : m_tokenExpression) {
				if (token.type == TokenType::numLiteral)
					i++;
			}

			if ((i > 1 && i == m_tokenExpression.size()) || !i) {
				m_syntaxError = true;
				return;
			}
		}


		for (int i = 0; i < m_tokenExpression.size(); i++) {
			if (i - 1 >= 0)
				Token lastToken = m_tokenExpression[i - 1];
			if (i + 1 < m_tokenExpression.size())
				Token nextToken = m_tokenExpression[i + 1];
			Token token = m_tokenExpression[i];

			if ((i + 1) < m_tokenExpression.size()) {
				//fixing signs
				if (tokenIsSign(token)) {
					
					if (i - 1 >= 0) {
						if (!(m_tokenExpression[i - 1].type == TokenType::numLiteral || m_tokenExpression[i - 1].type == TokenType::closeBrakets)) {

							if (m_tokenExpression[i + 1].type == TokenType::opSqrt ||
								m_tokenExpression[i + 1].type == TokenType::openBrakets ||
								tokenIsFunc(m_tokenExpression[i + 1])) {
								
								m_tokenExpression.erase(m_tokenExpression.begin() + i);
								m_tokenExpression.insert(m_tokenExpression.begin() + i, Token(TokenType::opMulti));
								if (token.type == TokenType::opMinus) {
									m_tokenExpression.insert(m_tokenExpression.begin() + i, Token(-1));
								}
								else m_tokenExpression.insert(m_tokenExpression.begin() + i, Token(1));
								i = -1;
								continue;
							}
							else if (m_tokenExpression[i + 1].type == TokenType::numLiteral) {
								if (token.type == TokenType::opMinus) {
									m_tokenExpression[i + 1].value *= -1;
								}
									m_tokenExpression.erase(m_tokenExpression.begin() + i);
									i = -1;
									continue;
								}
						}
					}
					else {
						if (m_tokenExpression[i + 1].type == TokenType::opSqrt ||
							m_tokenExpression[i + 1].type == TokenType::openBrakets ||
							tokenIsFunc(m_tokenExpression[i + 1])) {
							
							m_tokenExpression.erase(m_tokenExpression.begin() + i);
							m_tokenExpression.insert(m_tokenExpression.begin() + i, Token(TokenType::opMulti));
							if (token.type == TokenType::opMinus) {
								m_tokenExpression.insert(m_tokenExpression.begin() + i, Token(-1));
							}
							else m_tokenExpression.insert(m_tokenExpression.begin() + i, Token(1));
							i = -1;
							continue;
						}
						else if (m_tokenExpression[i + 1].type == TokenType::numLiteral) {
							if (token.type == TokenType::opMinus) {
								m_tokenExpression[i + 1].value *= -1;
							}
								m_tokenExpression.erase(m_tokenExpression.begin() + i);
								i = -1;
								continue;
						}
					}
				}

				//implicit multiplication
				if (token.type == TokenType::numLiteral) {
					if (m_tokenExpression[i + 1].type == TokenType::opSqrt || m_tokenExpression[i + 1].type == TokenType::openBrakets || tokenIsFunc(m_tokenExpression[i + 1])) {
						m_tokenExpression.insert(m_tokenExpression.begin() + (i-- + 1), Token(TokenType::opMulti));
						continue;
					}
				}
			}
		}
	}

	



	void Calculator::flushToken(std::string& tokenStr) {

		if (tokenStr.empty())
			return;

		if (isSign(tokenStr[0]) && tokenStr.size() > 1 && isNumber(tokenStr[1])) {
			m_tokenExpression.push_back(Token(std::stold(tokenStr)));
			tokenStr.clear();
			return;
		}
		
		if (isNumber(tokenStr[0])) {
			m_tokenExpression.push_back(Token(std::stold(tokenStr)));
			tokenStr.clear();
			return;
		}

		if (isOperator(tokenStr[0])) {
			if (tokenStr == "+") {
				m_tokenExpression.push_back(Token(TokenType::opPlus));
				tokenStr.clear();
				return;
			}

			if (tokenStr == "-") {
				m_tokenExpression.push_back(Token(TokenType::opMinus));
				tokenStr.clear();
				return;
			}

			if (tokenStr == "*") {
				m_tokenExpression.push_back(Token(TokenType::opMulti));
				tokenStr.clear();
				return;
			}

			if (tokenStr == "/") {
				m_tokenExpression.push_back(Token(TokenType::opDiv));
				tokenStr.clear();
				return;
			}

			if (tokenStr == "^") {
				m_tokenExpression.push_back(Token(TokenType::opPower));
				tokenStr.clear();
				return;
			}
		}

		if (isalpha(tokenStr[0])) {
			if (tokenStr == "sqrt") {
				m_tokenExpression.push_back(Token(TokenType::opSqrt));
				tokenStr.clear();
				return;
			}

			if (tokenStr == "sin") {
				m_tokenExpression.push_back(Token(TokenType::funcSin));
				tokenStr.clear();
				return;
			}

			if (tokenStr == "cos") {
				m_tokenExpression.push_back(Token(TokenType::funcCos));
				tokenStr.clear();
				return;
			}

			if (tokenStr == "tan") {
				m_tokenExpression.push_back(Token(TokenType::funcTan));
				tokenStr.clear();
				return;
			}
		}

		if (tokenStr == "(") {
			m_tokenExpression.push_back(Token(TokenType::openBrakets));
			tokenStr.clear();
			return;
		}

		if (tokenStr == ")") {
			m_tokenExpression.push_back(Token(TokenType::closeBrakets));
			tokenStr.clear();
			return;
		}

		m_syntaxError = true;
	}




	
	void Calculator::parseExpression() {
		
		std::stack<Token> opStack;
		
		if (m_tokenExpression[0].type == TokenType::numLiteral) {
				m_rpnExpression.push_back(m_tokenExpression[0]);
		}
		else opStack.push(m_tokenExpression[0]);

		for (int i = 1; i < m_tokenExpression.size(); i++) {
			Token token = m_tokenExpression[i];
			
			if (token.type == TokenType::numLiteral) {
				m_rpnExpression.push_back(token);
				continue;
			}
			else {
				if (token.type == TokenType::openBrakets) {
					opStack.push(token);
					continue;
				}

				if (token.type == TokenType::opPower) {
					opStack.push(token);
					continue;
				}

				if (tokenIsFunc(token)) {
					opStack.push(token);
					continue;
				}
				
				if (token.type == TokenType::closeBrakets) {
					while (!opStack.empty() && opStack.top().type != TokenType::openBrakets) {
						m_rpnExpression.push_back(opStack.top());
						opStack.pop();
					}
					if (!opStack.empty() && opStack.top().type == TokenType::openBrakets) {
						opStack.pop();
						continue;
					}
					else {
						m_syntaxError = true;
						return;
					}
				}
				
				if (!opStack.empty() && OpPrecedence[token.type] <= OpPrecedence[opStack.top().type]) {
					m_rpnExpression.push_back(opStack.top());
					opStack.pop();
				}
				opStack.push(token);
			}
		}
		
		while (!opStack.empty()) {
			m_rpnExpression.push_back(opStack.top());
			opStack.pop();
		}

		for (const auto& token : m_rpnExpression) {
			if (tokenIsBrakets(token)) {
				m_syntaxError = true;
				return;
			}
		}
	}




	
	void Calculator::processRPN() {

		std::vector<Token> buf = m_rpnExpression;

		while (buf.size() != 1) {
			for (int i = 0; i < buf.size(); i++) {
				if (buf[i].type != numLiteral) {
					getResult(buf, i);
					break;
				}
			}
		}
		m_result = buf.front().value;
	}


	void Calculator::getResult(std::vector<Token>& buf, int i) {
		
		switch (buf[i].type) {
		case numLiteral:
			break;

		case opPlus:
			buf[i] = Token(buf[i - 2].value + buf[i - 1].value);
			buf.erase(buf.begin() + (--i));
			buf.erase(buf.begin() + (--i));
			break;

		case opMinus:
			buf[i] = Token(buf[i - 2].value - buf[i - 1].value);
			buf.erase(buf.begin() + (--i));
			buf.erase(buf.begin() + (--i));
			break;

		case opMulti:
			buf[i] = Token(buf[i - 2].value * buf[i - 1].value);
			buf.erase(buf.begin() + (--i));
			buf.erase(buf.begin() + (--i));
			break;

		case opDiv:
			buf[i] = Token(buf[i - 2].value / buf[i - 1].value);
			buf.erase(buf.begin() + (--i));
			buf.erase(buf.begin() + (--i));
			break;

		case opPower:
			buf[i] = Token(pow(buf[i - 2].value, buf[i - 1].value));
			buf.erase(buf.begin() + (--i));
			buf.erase(buf.begin() + (--i));
			break;

		case opSqrt:
			buf[i] = Token(sqrt(buf[i - 1].value));
			buf.erase(buf.begin() + (--i));
			break;

		case funcSin:
			buf[i] = Token(sin(buf[i - 1].value));
			buf.erase(buf.begin() + (--i));
			break;

		case funcCos:
			buf[i] = Token(cos(buf[i - 1].value));
			buf.erase(buf.begin() + (--i));
			break;

		case funcTan:
			buf[i] = Token(tan(buf[i - 1].value));
			buf.erase(buf.begin() + (--i));
			break;

		default:
			break;
		}
	}
	

	void Calculator::to_string() {

		std::cout << "Input: ";
		
		for (const auto& token : m_tokenExpression) {
			switch (token.type) {
			case TokenType::opPlus:
				std::cout << "+";
				break;

			case TokenType::opMinus:
				std::cout << "-";
				break;

			case TokenType::opMulti:
				std::cout << "*";
				break;

			case TokenType::opDiv:
				std::cout << "/";
				break;

			case TokenType::opPower:
				std::cout << "^";
				break;

			case TokenType::opSqrt:
				std::cout << "sqrt";
				break;

			case TokenType::funcSin:
				std::cout << "sin";
				break;

			case TokenType::funcCos:
				std::cout << "cos";
				break;

			case TokenType::funcTan:
				std::cout << "tan";
				break;
				
			case TokenType::numLiteral:
				std::cout << token.value;
				break;

			case TokenType::openBrakets:
				std::cout << "(";
				break;

			case TokenType::closeBrakets:
				std::cout << ")";
				break;
			}
			std::cout << " ";
		}
		std::cout << std::endl;
	}

	void Calculator::to_stringRPN() {

		std::cout << "Parsed Input: ";
		
		for (const auto& token : m_rpnExpression) {
			switch (token.type) {
			case TokenType::opPlus:
				std::cout << "+";
				break;

			case TokenType::opMinus:
				std::cout << "-";
				break;

			case TokenType::opMulti:
				std::cout << "*";
				break;

			case TokenType::opDiv:
				std::cout << "/";
				break;

			case TokenType::opPower:
				std::cout << "^";
				break;

			case TokenType::opSqrt:
				std::cout << "sqrt";
				break;

			case TokenType::funcSin:
				std::cout << "sin";
				break;

			case TokenType::funcCos:
				std::cout << "cos";
				break;

			case TokenType::funcTan:
				std::cout << "tan";
				break;
				
			case TokenType::numLiteral:
				std::cout << token.value;
				break;

			case TokenType::openBrakets:
				std::cout << "(";
				break;

			case TokenType::closeBrakets:
				std::cout << ")";
				break;
			}
			std::cout << " ";
		}
		std::cout << std::endl;
	}
}