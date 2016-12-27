#include "polish.h"
#include "stack.h"
#include <string.h>
#include <stdexcept>
#include <math.h>
#include <stdio.h>

CNode* Add(CNode* CurNode, char* data) 
{
	CNode* Node = new CNode;
	Node->data = data;
	Node->next = 0;
	CurNode->next = Node;
	return Node;
}
int Polish::prioritet(char k) 
{
	int res;
	switch (k) 
	{
	case '(':
	case '|': res = 0;
		break;
	case'_':
	case '+':
	case '-': res = 1;
		break;
	case '*':
	case '/': res = 2;
		break;
	case '^': res = 3;
	}
	return res;
}
bool Polish::isoperation(char a)
{
	return ((a == '+') || (a == '-') || (a == '/') || (a == '*') ||
		(a == '^') || (a == '(') || (a == ')') || (a == '|') || (a == ' '));
}
bool Polish::isStr(char a)
{
	return (((a >= '0') && (a <= '9')) || ((a >= 'a') && (a <= 'z')));
}

void Polish::AddX(char a, int c) 
{
	Perem* help = x;
	int ex = 0;
	while (help) {
		if (a == help->ch) 
		{
			ex = 1;
			break;
		}
		help = help->next;
	}

	if (ex)
		help->val = c;
	else 
	{
		Perem* newv = new Perem;
		newv->ch = a;
		newv->val = c;
		newv->next = x;
		x = newv;
	}
}
CNode* Polish::perevod(char* s) 
{
	int pos;
	CNode* Perevod = new CNode;
	CNode* help = Perevod;
	char * str = new char[10];
	int len = 0, hp = 0;
	char* a;
	if (!s[0])
		throw std::logic_error("stroka empty");
	int sl = strlen(s);
	char *exp = new char[sl + 2];
	exp = strcpy(exp, s);
	exp[sl] = ' ';
	exp[sl + 1] = 0;
	if (isStr(exp[0]))
		pos = 0;
	else
		pos = 1;

	for (int i = 0; exp[i] != 0; i++) 
	{
		a = new char(exp[i]);
		if (pos == 0)
			if (isStr(*a) || ((*a == '.') && (!hp))) 
			{
				if (*a == '.')
					hp = 1;
				str[len] = *a;
				len++;
				delete a;
			}
			else 
			{
				if (!isoperation(*a))
					throw std::logic_error("oshibka!");
				str[len] = 0;
			    help = Add(help, str);
				pos = 1;
				str = new char[10];
				len = 0;
				hp = 0;
				if (*a != ' ')
					help = Add(help, a);
				continue;
			}
			if (pos == 1)
				if (isStr(*a)) {
					str[len] = *a;
					len++;
					pos = 0;
					delete a;
				}
				else {
					if (!isoperation(*a))
						throw std::logic_error("oshibka");
					if (*a != ' ') {
						help = Add(help, a);
					}
				}
	}
	delete[]exp;
	Perevod = Perevod->next;
	return Perevod;
}

Polish::Polish(char* exp) 
{
	x = 0;
	char* res = new char[strlen(exp) * 2];
	int pos = 0;
	int mod = 0, br = 0, clmod = 0;
	char last = ' ';
	CNode* parse = perevod(exp);

	Stack<char> st;

	if (parse == 0)
		throw std::logic_error("oshibka");

	while (parse) 
	{
		char * el = parse->data;
		if (isStr(*el)) 
		{
			for (int i = 0; el[i] != 0; i++, pos++)
				res[pos] = el[i];
			res[pos] = ' ';
			pos++;
		}
		else 
		{
			switch (*el) 
			{
			case '(':
				st.push(*el);
				br = 1;
				break;
			case '|':
				if (mod || !pos || (isoperation(last) && (last != '|')))
				{
					st.push(*el);
					if (parse->next)
						if (*parse->next->data == '|')
							mod = 1;
						else
							mod = 0;
					clmod = 1;
				}
				else 
				{
					while (st.gettop() != '|') 
					{
						res[pos++] = st.gettop();
						st.pop();
					}
					res[pos++] = '|';
					st.pop();
					clmod = 0;
				}
				break;
			case ')':
				if (st.gettop() == '(')
					throw std::logic_error("oshibka");
				if (br)
				{
					while (st.gettop() != '(') {
						res[pos] = st.gettop();
						pos++;
						st.pop();
					}
				}
				else
					throw std::logic_error("oshibka");
				st.pop();
				break;
			default:
				while (!st.empty()) 
				{
					if ((prioritet(*el)) <= prioritet(st.gettop())) 
					{
						res[pos] = st.gettop();
						pos++;
						st.pop();
					}
					else
						break;
				}

				if (*el == '-') 
				{
					if (!pos)
						st.push('_');
					else 
					{
						if ((last == '|') && (clmod))
							st.push('_');
						else
							if (!st.empty()) 
							{
								if (last == '(') 
								{
									st.push('_');
								}
								else
									st.push(*el);
							}
							else
								st.push(*el);
					}
				}
				else
					st.push(*el);
			}
		}
		parse = parse->next;
		last = *el;
	}

	while (!st.empty()) {
		if ((st.gettop() != '(') && (st.gettop() != '|'))
			res[pos] = st.gettop();
		else
			throw std::logic_error("oshibka");
		pos++;
		st.pop();
	}

	res[pos] = 0;

	Pols = res;
}

double Polish::Culc()
{
	if (!Pols)
		throw std::logic_error("oshibka");
	int len = strlen(Pols), slen;
	double a, b;
	Perem* help;
	Stack<double> st;
	char* num;
	for (int i = 0; i < len; i++)
	{
		if ((Pols[i] >= 'a') && (Pols[i] <= 'z')) 
		{
			help = x;
			while (help)
			{
				if (help->ch == Pols[i])
					break;
				help = help->next;
			}
			if (!help)
				throw std::logic_error("oshibka");
			st.push(help->val);
			i++;
		}
		else
			if ((Pols[i] >= '0') && (Pols[i] <= '9')) 
			{
				num = new char[10];
				slen = 0;
				while (Pols[i] != ' ') 
				{
					if ((Pols[i] >= 'a') && (Pols[i] <= 'z'))
						throw std::logic_error("oshibka");
					num[slen] = Pols[i];
					slen++;
					i++;
				}
				num[slen] = 0;
				st.push(atof(num));
			}
			else
				if (!st.empty()) 
				{
					b = st.gettop();
					st.pop();
					if ((Pols[i] != '|') && (Pols[i] != '_')) 
					{
						if (!st.empty()) 
						{
							a = st.gettop();
							st.pop();
						}
						else
							throw std::logic_error("Wrong expression!");
					}

					switch (Pols[i])
					{
					case '+':
						st.push(a + b);
						break;
					case '-':
						st.push(a - b);
						break;
					case '/':
						st.push(a / b);
						break;
					case '*':
						st.push(a * b);
						break;
					case '^':
						st.push(pow(a, b));
						break;
					case '|':
						st.push(abs(b));
						break;
					case '_':
						st.push(-b);
						break;
					default:
						throw std::logic_error("Wrong expression!");
					}
				}
				else
					throw std::logic_error("Wrong expression!");
	}

	double result = st.gettop();
	st.pop();
	if (st.empty())
		return result;
	else
		throw std::logic_error("Wrong expression!");
}