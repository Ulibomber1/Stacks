// infix to postfix conversion; simple case with operands that can
// only be single characters or single digits, no white space;
// Kerstin Voigt, CSE 2020  Spring 2022; Lab 7 

#include <iostream>
#include <string>
#include <sstream>
#include <cassert>
#include <cctype>
#include "Stack.h"
#include "Vector.h"

using namespace std;

int eval_postfix(string);
bool is_operator(string);
int string_to_integer(string);
int apply_op(string, int, int);

int main()
{
	string postfix;
	string ws;
	bool quit = false;

	do
	{
		cout << "Enter postfix expr: ";
		getline(cin, postfix);
		cout << "The postfix expr is: " << postfix;
		cout << endl << endl;
		int value = eval_postfix(postfix);
		cout << "The value of this expression is " << value << endl << endl;

		cout << "More? [y/n] ";
		char more;
		cin >> more;
		cout << "more is " << more << endl << endl;
		if (more != 'y' && more != 'Y')
			quit = true;
		getline(cin, ws);    // NEEDED to ready off the \n left over by getline
	} while (!quit);
	return 0;
}


int eval_postfix(string postfix)
{
	Stack<Vector<int> > stack;
	string next;
	int num;
	int result;

	istringstream istrm(postfix);
	istrm >> next;

	while (!istrm.fail())
	{
		//cout << "Reading " << next << " --> ";

		if (isalnum(next[0]))
		{
			num = string_to_integer(next);
			stack.push(num);
			cout << "push onto stack" << endl << endl;
		}
		else
		{
			assert(is_operator(next));
			cout << "operator " << next << " to be applied ..." << endl << endl;
			int x2 = stack.top();
			stack.pop();
			cout << "... pop " << x2 << endl << endl;

			int x1 = stack.top();
			stack.pop();
			cout << "... pop " << x1 << endl << endl;

			result = apply_op(next, x1, x2);
			stack.push(result);
			cout << " --> push " << result << " onto stack" << endl << endl;
		}
		istrm >> next;
	}
	assert(stack.size() == 1);
	result = stack.top();
	stack.pop();

	return result;
}

// several utility functions

// true when string is an operator symbol
bool is_operator(string x)
{
	return (x == "+" || x == "-" || x == "*" || x == "/");
}

// converts string of digits to integer value
int string_to_integer(string x)
{
	istringstream istrm(x);
	int num;
	istrm >> num;
	return num;
}

// returns value after operator op applied to 
// int operands x and y
int apply_op(string op, int x, int y)
{
	assert(is_operator(op));
	if (op == "+")
		return x + y;
	else if (op == "-")
		return x - y;
	else if (op == "*")
		return x * y;
	else
		return x / y;
}