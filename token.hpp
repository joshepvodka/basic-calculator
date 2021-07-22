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

		Token() : type(tNull), value(0), var("") {}
		Token(const Token& o) : type(o.type), value(o.value), var(o.var) {}
		Token(const Token&& o) : type(std::move(o.type)), value(std::move(o.value)), var(std::move(o.var)) {}
		Token(const long double& val) : type(numLiteral), value(val), var("") {}
		Token(const std::string& val) : type(numVar), value(0), var(val) {}
		Token(const TokenType& t) : type(t), value(0), var("") {}
		~Token() {}

		void operator=(const Token& o) {
			type = o.type;
			value = o.value;
			var = o.var;
		}

		void operator=(const Token&& o) {
			type = std::move(o.type);
			value = std::move(o.value);
			var = std::move(o.var);
		}

		TokenType type;
		
		long double value;
		std::string var;
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