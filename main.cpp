#include <string>
#include <sstream>
#include <cctype>
#include <vector>
#include <iostream>

#include "token.hpp"
#include "calculator.hpp"

std::string getInput();
int processInput(std::vector<Token>& arr, const std::string& in);
int flushToken(std::vector<Token>& arr, std::string& in, const short& nesting);
void processExpression(std::vector<Token> arr);

int main() {


	calc::Calculator calculator;

	while (1) {
		calculator.calculateExpression();
	}
	
	return 0;
}

/*
std::string getInput() {

	std::string in;
	std::getline(std::cin, in);
	return in;

}


int processInput(std::vector<Token>& arr, const std::string& in) {

	std::string buf;
	token_t type = -1;
	short nesting = 0;
	int err = 0;

	for(int i = 0; i < in.length(); i++) {

		//skip blanks
		if(isblank(in[i])) {
			flushToken(arr, buf, nesting);
			continue;
		}

		//if we wrote something before make it into a token and write the number
		if(isdigit(in[i]) || in[i] == '.') {
			if(type != Token::isNumber){
				err += flushToken(arr, buf, nesting);
				type = Token::isNumber;
			}
			buf.push_back(in[i]);
			continue;
		}

		if(in[i] == '(') {
			err += flushToken(arr, buf, nesting);
			nesting += 10;
			continue;
		}
		if(in[i] == ')') {
			err += flushToken(arr, buf, nesting);
			nesting -= 10;
			continue;
		}

		//if we arent seeing a number then it must be an operator. flush the last number and write the next operator;
		if(type != Token::isOperator) {
			err += flushToken(arr, buf, nesting);
			type = Token::isOperator;
		}
		buf.push_back(in[i]);
	}

	flushToken(arr, buf, nesting);
	return err;
}


int flushToken(std::vector<Token>& arr, std::string& in, const short& nesting) {

	if(in.empty())
		return 0;

	if(isdigit(in[0])) {
		arr.push_back(Token(std::stod(in)));
		in.clear();
		return 0;
	}

	if(in == "+") {
		arr.push_back(Token(Operator::Addition));
		arr.back().op.score += nesting;
		in.clear();
		return 0;
	}
	if(in == "-") {
		arr.push_back(Token(Operator::Subtraction));
		arr.back().op.score += nesting;
		in.clear();
		return 0;
	}
	if(in == "*") {
		arr.push_back(Token(Operator::Multiplication));
		arr.back().op.score += nesting;
		in.clear();
		return 0;
	}
	if(in == "/") {
		arr.push_back(Token(Operator::Division));
		arr.back().op.score += nesting;
		in.clear();
		return 0;
	}
	if(in == "sin") {
		arr.push_back(Token(Operator::Sin));
		arr.back().op.score += nesting;
		in.clear();
		return 0;
	}
	if(in == "cos") {
		arr.push_back(Token(Operator::Cos));
		arr.back().op.score += nesting;
		in.clear();
		return 0;
	}
	if(in == "tan") {
		arr.push_back(Token(Operator::Tan));
		arr.back().op.score += nesting;
		in.clear();
		return 0;
	}

	if(in == "^") {
		arr.push_back(Token(Operator::Exponent));
		arr.back().op.score += nesting;
		in.clear();
		return 0;
	}

	if(in == "sqrt") {
		arr.push_back(Token(Operator::SquareRoot));
		arr.back().op.score += nesting;
		in.clear();
		return 0;
	}

	return 1;
	         
}


void processExpression(std::vector<Token> arr) {

	while(arr.size() != 1) {
		
		Token next(Operator::None);
		int nextptr = 0;

		for(int i = 0; i < arr.size(); i++) {

			if(arr[i].type == Token::isOperator)
				if(arr[i].op.score > next.op.score) {
					next = arr[i];
					nextptr = i;
				}
		}

		double result;
		
		switch(next.op.inputs) {
			case 0:
				break;
			
			case 1:
				result = next.op.func(Args(arr[nextptr+1].num));
				arr[nextptr] = result;
				nextptr++;
				arr.erase(arr.begin() + nextptr);
				break;
			
			case 2:
				result = next.op.func(Args(arr[nextptr-1].num, arr[nextptr+1].num));
				arr[nextptr-1] = result;
				arr.erase(arr.begin() + nextptr);
				arr.erase(arr.begin() + nextptr);
				break;
		}
	}
	std::cout << arr.front().num << std::endl;	
}*/