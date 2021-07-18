#pragma once
#ifndef __TOKEN_H__
#define __TOKEN_H__

#include <inttypes.h>
#include <memory>
#include <string>

namespace math {

	extern uint8_t OpPrecedence[];

	enum SymType : uint8_t {
		null,
		numeric,
		op,
		str,
		brakets
	};

	enum TokenType : uint8_t {
		tNull,
		opPlus,
		opMinus,
		opMulti,
		opDiv,
		opPower,
		opSqrt,
		funcSin,
		funcCos,
		funcTan,
		numLiteral,
		numVar,
		openBrakets,
		closeBrakets,
		symEqual,
		symSemicolon
	};
	
	struct Token {

		Token() : type(tNull), value(0), var(std::string()) {}
		Token(const Token& o) : type(o.type), value(o.value), var(std::string(o.var)) {}
		Token(const Token&& o) : type(o.type), value(o.value), var(std::string(o.var)) {}
		Token(const long double& val) : type(TokenType::numLiteral), value(val), var(std::string()) {}
		Token(const std::string& name) : type(numVar), value(0), var(std::string(name)) {}
		Token(const char* name) : type(numVar), value(0), var(name) {}
		Token(const TokenType& t) : type(t), value(0), var(std::string()) {}

		~Token() { var.~basic_string(); }

		void operator=(const Token& o) {
			type = o.type; value = o.value; var = o.var;
		}

		void operator=(const Token&&o) {
			type = o.type; value = o.value; var = o.var;
		}

		TokenType type;
		
		long double value;
		std::string var;
	};


	struct aToken {

		aToken() : type(tNull), m_data(nullptr) {}
		aToken(const aToken& o) : type(o.type), m_data(nullptr) {
			if (o.type == numLiteral) {
				long double* ptr = new long double;
				*ptr = o.value();
				m_data = (void*)ptr;
			}
			else if (o.type == numVar) {
				std::string* ptr = new std::string;
				*ptr = o.var();
				m_data = (void*)ptr;
			}
		}
		aToken(const aToken&& o) : type(std::move(o.type)), m_data(std::move(m_data)) {}
		aToken(const long double& val) : type(numLiteral), m_data(nullptr) {
			long double* ptr = new long double;
			*ptr = val;
			m_data = (void*)ptr;
		}
		aToken(const std::string& val) : type(numVar), m_data(nullptr) {
			std::string* ptr = new std::string;
			*ptr = val;
			m_data = (void*)ptr;
		}
		aToken(const TokenType& t) : type(t), m_data(nullptr) {}
		~aToken() { delete m_data; }

		void operator=(const aToken& o) {
			if (o.type == numLiteral) {
				long double* ptr = new long double;
				*ptr = o.value();
				m_data = (void*)ptr;
			}
			else if (o.type == numVar) {
				std::string* ptr = new std::string;
				*ptr = o.var();
				m_data = (void*)ptr;
			}
			type = o.type;
		}

		void operator=(const aToken&& o) {
			type = std::move(o.type);
			m_data = std::move(o.m_data);
		}

		TokenType type;
		
		long double& value() { return *(long double*)m_data; }
		const long double& value() const { return *(long double*)m_data; }

		const std::string& var() const { return *(std::string*)m_data; }
		
		void* m_data;
	};
	
	static bool tokenIsFunc(const Token& token) {
		if (token.type == funcSin ||
			token.type == funcCos ||
			token.type == funcTan)
			return true;
		return false;
	}

	static bool tokenIsOp(const Token& token) {
		if (token.type == opPlus ||
			token.type == opMinus ||
			token.type == opMulti ||
			token.type == opDiv ||
			token.type == opPower ||
			token.type == opSqrt)
			return true;
		return false;
	}

	static bool tokenIsSign(const Token& token) {
		if (token.type == opPlus ||
			token.type == opMinus)
			return true;
		return false;
	}

	static bool tokenIsBrakets(const Token& token) {
		if (token.type == openBrakets ||
			token.type == closeBrakets)
			return true;
		return false;
	}

}
#endif // __TOKEN_H__