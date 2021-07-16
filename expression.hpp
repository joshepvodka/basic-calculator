#pragma once
#ifndef __EXPRESSION_H__
#define __EXPRESSION_H__

#include <vector>
#include "token.hpp"

namespace math {

	class Expression {

	public:
		Expression() {}
		~Expression() {}

		inline const Token& operator[] (const int i) const  { return (i < 0 || i > m_tokens.size() ? Token() : m_tokens[i]); }
		
		inline Token& operator[] (const int i) { return m_tokens[i]; }

		inline const Token& peek(const int i) const { return (i < 0 || i >= m_tokens.size() ? Token() : m_tokens[i]); }

		inline const Token& operator() (const int i) const { return (i < 0 || i > m_tokens.size() ? Token() : m_tokens[i]); }

		inline void push_back(const Token& token) { m_tokens.push_back(token); }

		inline void erase(const std::vector<Token>::iterator& it) { m_tokens.erase(it); }

		inline void clear()  { m_tokens.clear(); }

		inline void insert(const std::vector<Token>::iterator& where, const Token& token) { m_tokens.insert(where, token); }

		inline bool empty() const  { return m_tokens.empty(); }

		inline size_t size() const  { return m_tokens.size(); }

		inline std::vector<Token>::iterator begin()  { return m_tokens.begin(); }
		
		inline std::vector<Token>::iterator end()  { return m_tokens.end(); }

		inline std::vector<Token>::const_iterator begin() const { return m_tokens.begin(); }

		inline std::vector<Token>::const_iterator end() const { return m_tokens.end(); }

		inline Token& front()  { return m_tokens.front(); }
		
		inline Token& back()  { return m_tokens.back(); }

	private:

		std::vector<Token> m_tokens;

	};
	
}


#endif // __EXPRESSION_H__