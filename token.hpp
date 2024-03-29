#ifndef __TOKEN_H__
#define __TOKEN_H__

#include <inttypes.h>
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

		Token(const long double& val) : type(TokenType::numLiteral), value(val) {}
		Token(const std::string& name) : type(numVar), var(name.c_str()) {}
		Token(const char* name) : type(numVar), var(name) {}
		Token(const TokenType& t) : type(t), value(0) {}

		TokenType type;
		union {
			long double value;
			const char* var;
		};
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