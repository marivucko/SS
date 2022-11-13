/*
 * AuxiliaryClass.cpp
 *
 *  Created on: Mar 1, 2019
 *      Author: user
 */

#include "AuxiliaryClass.h"
#include "FirstPass.h"


void infix_to_postfix(Line** infix, Line** postfix, int lineCounter)
{
	Line* stackHead = 0; Line* stackTail = 0;
	Line* postfixHead = 0; Line* postfixTail = 0;
	int rank = 0;
	Line* next = *infix;
	string s;
	Line* x = 0;
	while (next != 0)
	{
		if ((next->word != "+") && (next->word != "-") && (next->word != "*") && (next->word != "/") && (next->word != "(") && (next->word != ")"))
		{
			pushWord(&postfixHead, &postfixTail, next->word);
			rank = rank + 1;
		}
		else
		{
			//Line* top = topWord(&stackHead, &stackTail);
			//if (top != nullptr)
				//cout << "*********" << top->word << "************\n";
			//while ((stackHead != nullptr) && (top != nullptr) && (ipr(next->word) <= spr(top->word)))
			while ((stackHead != 0) && (ipr(next->word) <= spr(topWord(&stackHead, &stackTail)->word)))
			{
				x = popWord(&stackHead, &stackTail);
				pushWord(&postfixHead, &postfixTail, x->word);
				rank = rank - 1;
				if (rank < 1)
					throw new SyntaxError(lineCounter, "Error::::::::::::::::: directive EQU does not have a correct assignment.");
			}
			if (next->word != ")")
				pushWord(&stackHead, &stackTail, next->word);
			else
				x = popWord(&stackHead, &stackTail);
		}
		*infix = (*infix)->nextWord;
		next = *infix;
		//cout << "=======\n";
		//printLine(&postfixHead);
		//cout << "\n";
		//printLine(&stackHead);

	}

	while (stackHead != 0)
	{
		x = popWord(&stackHead, &stackTail);
		//cout << "--" << x->word << "--";
		pushWord(&postfixHead, &postfixTail, x->word);
		rank = rank - 1;
	}
	*postfix = postfixHead;

	//if (rank != 1)
		//	throw new SyntaxError(lineCounter, "Error: directive EQU does not have a correct assignment.");

}

int convert_to_int(string s)
{
	int x = 0;
	unsigned int i;

	if (s[0] == '-')
	{
		for (i = 1; i < s.length(); i++)
		{
			x = x * 10 + (s[i] - '0');
		}
		x = -1 * x;
		return x;
	}
	else
	{
		for (i = 0; i < s.length(); i++)
		{
			x = x * 10 + (s[i] - '0');
		}
		return x;
	}
}

string convert_to_string(int n)
{
	bool negative = false;
	string ret = "";
	if (n == 0)
	{
		return "0";
	}
	if (n < 0)
	{
		negative = true;
		n = -1 * n;
	}
	while (n > 0)
	{
		switch (n % 10)
		{
		case 0: ret = '0' + ret;
			break;
		case 1: ret = '1' + ret;
			break;
		case 2: ret = '2' + ret;
			break;
		case 3: ret = '3' + ret;
			break;
		case 4: ret = '4' + ret;
			break;
		case 5: ret = '5' + ret;
			break;
		case 6: ret = '6' + ret;
			break;
		case 7: ret = '7' + ret;
			break;
		case 8: ret = '8' + ret;
			break;
		case 9: ret = '9' + ret;
			break;
		}
		n /= 10;
	}
	if (negative)
		ret = '-' + ret;
	return ret;
}


string convert_negative_to_hex(int n, int num_of_Bytes)
{
	string ret = "";
	int x = -1 * n;
	int k = 0;
	int k1 = 0;
	string c, c1;
	while ((x > 0) && (k < num_of_Bytes))
	{
		c = nibble_to_16(x % 16);

		if (k1 == 0)
		{
			k1++;
			if (c == "0") { c1 = "0"; k1 = 0; }
			else if (c == "1") c1 = "F";
			else if (c == "2") c1 = "E";
			else if (c == "3") c1 = "D";
			else if (c == "4") c1 = "C";
			else if (c == "5") c1 = "B";
			else if (c == "6") c1 = "A";
			else if (c == "7") c1 = "9";
			else if (c == "8") c1 = "8";
			else if (c == "9") c1 = "7";
			else if (c == "A") c1 = "6";
			else if (c == "B") c1 = "5";
			else if (c == "C") c1 = "4";
			else if (c == "D") c1 = "3";
			else if (c == "E") c1 = "2";
			else if (c == "F") c1 = "1";
		}
		else
		{
			if (c == "0") c1 = "F";
			else if (c == "1") c1 = "E";
			else if (c == "2") c1 = "D";
			else if (c == "3") c1 = "C";
			else if (c == "4") c1 = "B";
			else if (c == "5") c1 = "A";
			else if (c == "6") c1 = "9";
			else if (c == "7") c1 = "8";
			else if (c == "8") c1 = "7";
			else if (c == "9") c1 = "6";
			else if (c == "A") c1 = "5";
			else if (c == "B") c1 = "4";
			else if (c == "C") c1 = "3";
			else if (c == "D") c1 = "2";
			else if (c == "E") c1 = "1";
			else if (c == "F") c1 = "0";
		}
		ret = c1 + ret;
		x = x / 16;
		k++;
	}
	while (k < num_of_Bytes)
	{
		ret = "F" + ret;
		k++;
	}
	//cout << "\n" << ret;
	return ret;
}

string convert_negative_to_hex_permutated(int n, int num_of_Bytes)
{
	string ret = "";
	int x = -1 * n;
	int k = 0;
	int k1 = 0;
	string c, c1;
	while ((x > 0) && (k < num_of_Bytes))
	{
		c = nibble_to_16(x % 16);

		if (k1 == 0)
		{
			k1++;
			if (c == "0") { c1 = "0"; k1 = 0; }
			else if (c == "1") c1 = "F";
			else if (c == "2") c1 = "E";
			else if (c == "3") c1 = "D";
			else if (c == "4") c1 = "C";
			else if (c == "5") c1 = "B";
			else if (c == "6") c1 = "A";
			else if (c == "7") c1 = "9";
			else if (c == "8") c1 = "8";
			else if (c == "9") c1 = "7";
			else if (c == "A") c1 = "6";
			else if (c == "B") c1 = "5";
			else if (c == "C") c1 = "4";
			else if (c == "D") c1 = "3";
			else if (c == "E") c1 = "2";
			else if (c == "F") c1 = "1";
		}
		else
		{
			if (c == "0") c1 = "F";
			else if (c == "1") c1 = "E";
			else if (c == "2") c1 = "D";
			else if (c == "3") c1 = "C";
			else if (c == "4") c1 = "B";
			else if (c == "5") c1 = "A";
			else if (c == "6") c1 = "9";
			else if (c == "7") c1 = "8";
			else if (c == "8") c1 = "7";
			else if (c == "9") c1 = "6";
			else if (c == "A") c1 = "5";
			else if (c == "B") c1 = "4";
			else if (c == "C") c1 = "3";
			else if (c == "D") c1 = "2";
			else if (c == "E") c1 = "1";
			else if (c == "F") c1 = "0";
		}
		ret = ret + c1;
		x = x / 16;
		k++;
	}
	while (k < num_of_Bytes)
	{
		ret = ret + "F";
		k++;
	}
	//cout << "\n" << ret;
	return ret;
}

string convert_negative_to_hex_permutated2(int n, int num_of_Bytes)
{
	string ret = "";
	int x = -1 * n;
	int k = 0;
	int k1 = 0;
	string c;
	string c1 = ""; string c2 = "";
	while ((x > 0) && (k < num_of_Bytes))
	{
		c = nibble_to_16(x % 16);

		if (k1 == 0)
		{
			k1++;
			if (c == "0") { c1 = "0"; k1 = 0; }
			else if (c == "1") c1 = "F";
			else if (c == "2") c1 = "E";
			else if (c == "3") c1 = "D";
			else if (c == "4") c1 = "C";
			else if (c == "5") c1 = "B";
			else if (c == "6") c1 = "A";
			else if (c == "7") c1 = "9";
			else if (c == "8") c1 = "8";
			else if (c == "9") c1 = "7";
			else if (c == "A") c1 = "6";
			else if (c == "B") c1 = "5";
			else if (c == "C") c1 = "4";
			else if (c == "D") c1 = "3";
			else if (c == "E") c1 = "2";
			else if (c == "F") c1 = "1";
		}
		else
		{
			if (c == "0") c1 = "F";
			else if (c == "1") c1 = "E";
			else if (c == "2") c1 = "D";
			else if (c == "3") c1 = "C";
			else if (c == "4") c1 = "B";
			else if (c == "5") c1 = "A";
			else if (c == "6") c1 = "9";
			else if (c == "7") c1 = "8";
			else if (c == "8") c1 = "7";
			else if (c == "9") c1 = "6";
			else if (c == "A") c1 = "5";
			else if (c == "B") c1 = "4";
			else if (c == "C") c1 = "3";
			else if (c == "D") c1 = "2";
			else if (c == "E") c1 = "1";
			else if (c == "F") c1 = "0";
		}
		if (k % 2 == 0)
		{
			ret = ret + c2;
			c2 = c1;
		}
		else
			c2 = c1 + c2;
		cout << " *"<<c2<<"* ";
		x = x / 16;
		k++;
	}
	ret = ret + c2;
	k1  = k;
	while (k < num_of_Bytes)
	{
		if (k1 % 2 == 1)
		{
			ret = ret + 'F';
			//cout << " length " << ret.length() << " " << ret << " ";
			ret[k1] = ret[k1-1];
			ret[k1-1] = 'F';
			k1++; k++;
			//cout << " " << ret << " tuturutuututuut\n";
		}
		else
		{
			ret = ret + "F";
			k++;
		}
	}
	return ret;
}

string convert_positive_to_hex_permutated(int n, int num_of_Bytes)
{
	string ret = "";
	int x = n;
	int k = 0;
	int k1;
	string c;
	string c1 = "";
	while ((x > 0) && (k < num_of_Bytes))
	{
		c = nibble_to_16(x % 16);
		if (k % 2 == 0)
		{
			ret = ret + c1;
			c1 = c;
		}
		else
			c1 = c + c1;
		x = x / 16;
		k++;
	}
	ret = ret + c1;
	k1  = k;
	while (k < num_of_Bytes)
	{
		if (k1 % 2 == 1)
		{
			ret = ret + '0';
			//cout << " length " << ret.length() << " " << ret << " ";
			ret[k1] = ret[k1-1];
			ret[k1-1] = '0';
			k1++; k++;
			//cout << " " << ret << " tuturutuututuut\n";
		}
		else
		{
			ret = ret + "0";
			k++;
		}
	}
	//cout << "\n" << ret;
	return ret;
}

int convert_to_int_from_hex(string s)
{
	int ret = 0; int j = 0; int k = 1;

	if((s[0] == 'F') || (s[0] == 'E') || (s[0] == 'D') || (s[0] == 'C')
	|| (s[0] == 'B') || (s[0] == 'A') || (s[0] == '9') || (s[0] == '8'))
	{
		while(s.length() > 0)
		{
			if (j == 0)
			{
				j++;
				string c = s.substr(s.length() - 1, 1);
				if (c == "0") { ret += 0; j = 0; }
				else if (c == "1") ret += 15;
				else if (c == "2") ret += 14;
				else if (c == "3") ret += 13;
				else if (c == "4") ret += 12;
				else if (c == "5") ret += 11;
				else if (c == "6") ret += 10;
				else if (c == "7") ret += 9;
				else if (c == "8") ret += 8;
				else if (c == "9") ret += 7;
				else if (c == "A") ret += 6;
				else if (c == "B") ret += 5;
				else if (c == "C") ret += 4;
				else if (c == "D") ret += 3;
				else if (c == "E") ret += 2;
				else if (c == "F") ret += 1;
				s = s.substr(0, s.length() - 1);
			}
			else
			{
				k = k * 16;
				string c = s.substr(s.length() - 1, 1);
				if (c == "0") { ret += 15 * k; }
				else if (c == "1") ret += 14 * k;
				else if (c == "2") ret += 13 * k;
				else if (c == "3") ret += 12 * k;
				else if (c == "4") ret += 11 * k;
				else if (c == "5") ret += 10 * k;
				else if (c == "6") ret += 9 * k;
				else if (c == "7") ret += 8 * k;
				else if (c == "8") ret += 7 * k;
				else if (c == "9") ret += 6 * k;
				else if (c == "A") ret += 5 * k;
				else if (c == "B") ret += 4 * k;
				else if (c == "C") ret += 3 * k;
				else if (c == "D") ret += 2 * k;
				else if (c == "E") ret += 1 * k;
				else if (c == "F") ret += 0 * k;
				s = s.substr(0, s.length() - 1);
			}
		}

		return -1 * ret;
	}



	while (s[0] == '0')
		s = s.substr(1);

	while(s.length() > 0)
	{
		ret = ret * 16;
		string c = s.substr(0, 1);
		if (c == "0") ret += 0;
		else if (c == "1") ret += 1;
		else if (c == "2") ret += 2;
		else if (c == "3") ret += 3;
		else if (c == "4") ret += 4;
		else if (c == "5") ret += 5;
		else if (c == "6") ret += 6;
		else if (c == "7") ret += 7;
		else if (c == "8") ret += 8;
		else if (c == "9") ret += 9;
		else if (c == "A") ret += 10;
		else if (c == "B") ret += 11;
		else if (c == "C") ret += 12;
		else if (c == "D") ret += 13;
		else if (c == "E") ret += 14;
		else if (c == "F") ret += 15;
		s = s.substr(1);
	}
	return ret;
}


int convert_to_int_from_hex_when_hex_is_positive(string s)	
{
	int ret = 0;
	while(s.length() > 0)
	{
		ret = ret * 16;
		string c = s.substr(0, 1);
		if (c == "0") ret += 0;
		else if (c == "1") ret += 1;
		else if (c == "2") ret += 2;
		else if (c == "3") ret += 3;
		else if (c == "4") ret += 4;
		else if (c == "5") ret += 5;
		else if (c == "6") ret += 6;
		else if (c == "7") ret += 7;
		else if (c == "8") ret += 8;
		else if (c == "9") ret += 9;
		else if (c == "A") ret += 10;
		else if (c == "B") ret += 11;
		else if (c == "C") ret += 12;
		else if (c == "D") ret += 13;
		else if (c == "E") ret += 14;
		else if (c == "F") ret += 15;
		s = s.substr(1);
	}
	return ret;
}

int eval_postfix(Line** postfix, int lineCounter)
{
	Line* stackHead = 0; Line* stackTail = 0;
	int rez = 0;
	Line* currPostfix = *postfix;
	while (currPostfix != 0)
	{
		string x = currPostfix->word;
		cout << x << "mmmmmmmmm\n";

		if ((x == "+") || (x == "-") || (x == "*") || (x == "/"))	// binarni operator
		{
			string ops2 = popWord(&stackHead, &stackTail)->word;
			string ops1 = popWord(&stackHead, &stackTail)->word;

			if (is_variable_correct(ops2))
			{

			}

			int op2 = 0;
			int op1 = 0;
			if (is_number(ops2))			
				op2 = convert_to_int(ops2);
			//else if (ops2.substr(2,1) == "0" || ops2.substr(2,1) == "1" || ops2.substr(2,1) == "2" || ops2.substr(2,1) == "3" 
			//      || ops2.substr(2,1) == "4" || ops2.substr(2,1) == "5" || ops2.substr(2,1) == "6" || ops2.substr(2,1) == "7" )
			//	op2 = convert_to_int_from_hex_when_hex_is_positive(ops2.substr(2));
			//else 
			//	op2 = convert_to_int_from_hex(ops2.substr(2));

 
			if (is_number(ops1))			
				op1 = convert_to_int(ops1);
			//else if (ops1.substr(2,1) == "0" || ops1.substr(2,1) == "1" || ops1.substr(2,1) == "2" || ops1.substr(2,1) == "3" 
			//      || ops1.substr(2,1) == "4" || ops1.substr(2,1) == "5" || ops1.substr(2,1) == "6" || ops1.substr(2,1) == "7" )
			//	op1 = convert_to_int_from_hex_when_hex_is_positive(ops2.substr(2));
			//else 
			//	op1 = convert_to_int_from_hex(ops2.substr(2));

			

			if (x == "+")
				rez = op1 + op2;
			else if (x == "-")
				rez = op1 - op2;
			else if (x == "*")
				rez = op1 * op2;
			else if (x == "/")
				rez = op1 / op2;

			cout << op1 << "&&&" << op2 << "&&&" << rez << "&&&\n";

			pushWord(&stackHead, &stackTail, convert_to_string(rez));
		}
		else														// operand
		{
			pushWord(&stackHead, &stackTail, x);
		}
		currPostfix = currPostfix->nextWord;
		//cout << "STACK";
		//if (stackHead != nullptr)
		//printLine(&stackHead);
		//cout << "\n";
	}

	string r = popWord(&stackHead, &stackTail)->word;
	//cout << r << " rez\n";
	//if (is_hex_number(r))
	//	rez = convert_to_int_from_hex(r);
	//else
		rez = convert_to_int(r);
	//cout << rez << " rezrez\n";
	//rez = convert_to_int((popWord(&stackHead, &stackTail))->word);


	if (stackHead == 0)
		return rez;
	else
	{
		throw new SyntaxError(lineCounter, "Error: assignemet in EQU directive is not correct.");
		return 0;
	}
}

void pushWord(Line** head, Line** tail, string word)
{
	Line* newWord = new Line(word);
	if ((*head) == 0)
		*head = newWord;
	else
		(*tail)->nextWord = newWord;
	*tail = newWord;
}

Line* popWord(Line** head, Line** tail)
{
	if (*head == 0)
		return 0;
	Line* ret;
	Line* currWord = *head; Line* prev = 0;
	while (currWord->nextWord != 0)
	{
		prev = currWord;
		currWord = currWord->nextWord;
	}
	if (prev == 0)
	{
		*head = 0;
		*tail = 0;
		ret = currWord;
		return ret;
	}
	*tail = prev;
	ret = currWord;
	prev->nextWord = 0;
	return ret;
}

Line* topWord(Line** head, Line** tail)
{
	if (*head == 0)
		return 0;
	Line* ret;
	Line* currWord = *head;
	while (currWord->nextWord != 0)
	{
		currWord = currWord->nextWord;
	}
	ret = currWord;
	return ret;
}


int ipr(string s)
{
	if ((s == "+") || (s == "-")) return 2;
	if ((s == "*") || (s == "/")) return 3;
	if (s == "(") return 6;
	if (s == ")") return 1;
	return -1;
}

int spr(string s)
{
	if ((s == "+") || (s == "-")) return 2;
	if ((s == "*") || (s == "/")) return 3;
	if (s == "(") return 0;
	return -1;
}

void printLine(Line** words)
{
	if ((*words) == 0) return;
	Line* currWrod = *words;
	while (currWrod != 0)
	{
		cout << currWrod->word << "_";
		currWrod = currWrod->nextWord;
	}
	cout << "\n";
}

void deleteLine(Line** words)
{
	if ((*words) == 0) return;
	Line* currWrod = *words;
	while (currWrod != 0)
	{
		Line* oldWord = currWrod;
		currWrod = currWrod->nextWord;
		delete oldWord;
	}
}

string delete_white_spaces_at_the_begging(string s)
{
	int i = 0;

	while ((s.length() > 0) && (s[i] == ' '))
	{
		s.erase(0, 1);
	}

	 return s;
}

string delete_white_spaces_at_the_end(string s)
{
	while ((s.length() > 0) && (s[s.length() - 1] == ' '))
	{
		s.erase(s.length() - 1, 1);
	}

	 return s;
}

string delete_white_spaces_between_words(string s)
{
	s = s + " ";
	unsigned int i = s.find_first_of(" ");
	unsigned int j = 0;
	string ret = "";
	string ret2 = "";
	while (i < s.length())
	{
		string s1 = s.substr(0, i);
		ret += s1 + " ";
		ret2 += s1;
		s = s.substr(i);
		j = s.find_first_not_of(" ");
		if (j < s.length())
		{
			s = s.substr(j);
			i =  s.find_first_of(" ");
		}
	}
	return ret;
}

string delete_all_white_spaces_between_words(string s)
{
	s = s + " ";
	unsigned int i = s.find_first_of(" ");
	unsigned int j = 0;
	string ret = "";
	string ret2 = "";
	while (i < s.length())
	{
		string s1 = s.substr(0, i);
		ret += s1;
		ret2 += s1;
		s = s.substr(i);
		j = s.find_first_not_of(" ");
		if (j < s.length())
		{
			s = s.substr(j);
			i =  s.find_first_of(" ");
		}
	}
	return ret;
}

string insert_white_spaces_between_operation(string s, string operation)
{
	s += operation;
	unsigned int i = s.find_first_of(operation);
	unsigned int j = 0;
	string ret = "";
	string ret2 = "";
	while (i < s.length())
	{
		string s1 = s.substr(0, i);
		ret += s1 + " " + operation + " ";
		ret2 += s1;
		s = s.substr(i);
		j = s.find_first_not_of(operation);
		if (j < s.length())
		{
			s = s.substr(j);
			i =  s.find_first_of(operation);
		}
	}

	unsigned int k = ret.find_last_of(operation);
	if (k < ret.length())
		ret = ret.substr(0, k);
	return ret;
}

string insert_white_spaces_between_operations(string s)
{
	string ret;

	ret = insert_white_spaces_between_operation(s, "+");
	ret = insert_white_spaces_between_operation(ret, "-");
	ret = insert_white_spaces_between_operation(ret, "*");
	ret = insert_white_spaces_between_operation(ret, "/");
	ret = insert_white_spaces_between_operation(ret, "(");
	ret = insert_white_spaces_between_operation(ret, ")");

	return ret;
}

void get_words_between_white_spaces(string s, int* numOfWords, Line** firstWord)
{
	*numOfWords = 0;
	*firstWord = 0;
	Line* lastWord = 0;
	s = s + " ";
	unsigned int i = s.find_first_of(" ");
	unsigned int j = 0;

	while (i < s.length())
	{
		string s1 = s.substr(0, i);
		Line* newWord = new Line(s1);

		if (*firstWord == 0)
		{
			*firstWord = newWord;
		}
		else
		{
			lastWord->nextWord = newWord;
		}
		lastWord = newWord;

		(*numOfWords) = (*numOfWords) + 1;
		s = s.substr(i);
		j = s.find_first_not_of(" ");
		if (j < s.length())
		{
			s = s.substr(j);
			i =  s.find_first_of(" ");
		}
	}
	/*Line* curr = *firstWord;
	while (curr != nullptr)
	{
		cout << curr->word << "$";
		curr = curr->nextWord;
	}*/
}

void get_words_between_commas(string s, int* numOfWords, Line** firstWord)
{
	*numOfWords = 0;
	*firstWord = 0;
	Line* lastWord = 0;

	unsigned int k = s.find_last_of(",");
	if (k != s.length() - 1)
		s = s + ",";

	unsigned int i = s.find_first_of(",");
	if (i == 0)
	{
		s = s.substr(1);
		i = s.find_first_of(",");
	}

	unsigned int j = 0;

	while (i < s.length())
	{
		string s1 = s.substr(0, i);
		Line* newWord = new Line(s1);

		if (s1 != ",")
		{
			if (*firstWord == 0)
			{
				*firstWord = newWord;
			}
			else
			{
				lastWord->nextWord = newWord;
			}
			lastWord = newWord;
		}

		(*numOfWords) = (*numOfWords) + 1;
		s = s.substr(i);
		j = s.find_first_not_of(",");
		if (j < s.length())
		{
			s = s.substr(j);
			i =  s.find_first_of(",");
		}
	}
	/*Line* curr = *firstWord;
	while (curr != nullptr)
	{
		cout << curr->word << "$";
		curr = curr->nextWord;
	}*/
}

void get_words_between_plus_and_minus(string s, int* numOfWords, Line** firstWord, Line** firstSign)
{
	*numOfWords = 0;
	*firstWord = 0;
	Line* lastWord = 0;
	*firstSign = 0;
	Line* lastSign = 0;
	Line* oneBeforeLastSign = 0;

	unsigned int k1 = s.find_last_of("+");
	unsigned int k2 = s.find_last_of("-");
	if ((k1 != s.length() - 1) && (k2 != s.length() - 1))
		s = s + "+";

	unsigned int i1 = s.find_first_of("+");
	unsigned int i2 = s.find_first_of("-");
	if (i1 == 0)
	{
		s = s.substr(1);
		i1 = s.find_first_of("+");
		i2 = s.find_first_of("-");
		Line* newSign = new Line("+");
		*firstSign = newSign;
		lastSign = newSign;
	}
	else if (i2 == 0)
	{
		s = s.substr(1);
		i1 = s.find_first_of("+");
		i2 = s.find_first_of("-");
		Line* newSign = new Line("-");
		*firstSign = newSign;
		lastSign = newSign;
	}
	else
	{
		i1 = s.find_first_of("+");
		i2 = s.find_first_of("-");
		Line* newSign = new Line("+");
		*firstSign = newSign;
		lastSign = newSign;
	}
	unsigned int j = 0;
	string s1;

	while ((i1 < s.length()) || (i2 < s.length()))
	{
		j = (i1 < i2) ? i1 : i2;
		if (j == 1)
			s1 = s.substr(0, j);
		else
			s1 = s.substr(0, j);

		Line* newWord = new Line(s1);
		Line* newSign = (j == i1) ? new Line("+") : new Line("-");

		if ((s1 != "+") && (s1 != "-"))
		{
			if (*firstWord == 0)
			{
				*firstWord = newWord;
			}
			else
			{
				lastWord->nextWord = newWord;
			}
			lastWord = newWord;

			if (*firstSign == 0)
			{
				*firstSign = newSign;
			}
			else
			{
				oneBeforeLastSign = lastSign;
				lastSign->nextWord = newSign;
			}
			lastSign = newSign;
		}

		(*numOfWords) = (*numOfWords) + 1;
		if (j == 0)
			s = s.substr(j + 1, s.length());
		else
			s = s.substr(j + 1, s.length());

		i1 =  s.find_first_of("+");
		i2 =  s.find_first_of("-");
	}

	oneBeforeLastSign->nextWord = 0;
	lastSign = oneBeforeLastSign;

	Line* curr1 = *firstWord; Line* curr2 = *firstSign;
	while (curr1 != 0)
	{
		cout << curr1->word << " ";
		curr1 = curr1->nextWord;
	}
	cout << "***********\n";
	while (curr2 != 0)
	{
		cout << curr2->word << " ";
		curr2 = curr2->nextWord;
	}
}

void add_words_at_the_beggining_of_first_line(Line** l1, Line** l2)
{
	Line* curr2 = (*l2);
	if ((*l1) == 0)
	{
		*l1 = *l2;
		return;
	}
	else if ((*l2) == 0)
	{
		return;
	}
	else
	{
		while (curr2->nextWord != 0)
		{
			curr2 = curr2->nextWord;
		}
		curr2->nextWord = (*l1);
		*l1 = *l2;
	}
}

void add_words_at_the_end_of_first_line(Line** l1, Line** l2)
{
	Line* curr1 = (*l1);
	if ((*l1) == 0)
	{
		*l1 = *l2;
		return;
	}
	else if ((*l2) == 0)
	{
		return;
	}
	else
	{
		while (curr1->nextWord != 0)
		{
			curr1 = curr1->nextWord;
		}
		curr1->nextWord = (*l2);
		//*l1 = *l2;
	}
}

bool check_if_two_commas_are_not_next_to_each_other(string s)
{
	//cout << "***" << s << "***";
	unsigned int i = s.find(",,");
	//cout << i << "\n";
	if (i < s.length())
		return true;
	else
		return false;
}

bool is_empty(string s)
{
	if (s == "")
		return true;
	else
		return false;
}

bool just_white_spaces(string s)				// blanko0blanko1\n  znaci \n je na poziciji 2. tj. s.length()-1
{
	unsigned int j = s.find_first_not_of(" ");
	return (j == s.length() - 1);
}

bool is_variable_correct(string s)
{
	if (!(((s[0] >= 'a') && (s[0] <= 'z')) || ((s[0] >= 'A') && (s[0] <= 'Z')))) return false;
	unsigned int i;
	for (i = 1; i < s.length() - 1; i++)
	{
		if (!(((s[i] >= 'a') && (s[i] <= 'z')) || ((s[i] >= 'A') && (s[i] <= 'Z')) || ((s[i] >= '0') && (s[i] <= '9')))) return false;
	}
	return true;
}

bool is_number(string s)
{
	unsigned int i;

	if ((s[0] == '0') && (s.length() >= 2)) return false;						// 01 0230 0034 it can't start with digit 0
	if ((s[0] != '-') && (!((s[0] >= '0') && (s[0] <= '9')))) return false;

	for (i = 1; i < s.length(); i++)
	{
		if (!((s[i] >= '0') && (s[i] <= '9'))) return false;
	}
	return true;
}

string nibble_to_16(int c1)
{
	string s1;

		 if (c1 == 0) s1 = "0";
	else if (c1 == 1) s1 = "1";
	else if (c1 == 2) s1 = "2";
	else if (c1 == 3) s1 = "3";
	else if (c1 == 4) s1 = "4";
	else if (c1 == 5) s1 = "5";
	else if (c1 == 6) s1 = "6";
	else if (c1 == 7) s1 = "7";

	else if (c1 == 8) s1 = "8";
	else if (c1 == 9) s1 = "9";
	else if (c1 == 10) s1 = "A";
	else if (c1 == 11) s1 = "B";
	else if (c1 == 12) s1 = "C";
	else if (c1 == 13) s1 = "D";
	else if (c1 == 14) s1 = "E";
	else if (c1 == 15) s1 = "F";

	return s1;
}

bool psw(string s)					// PSW
{
	return ((s == "psw") || (s == "PSW"));
}

bool absolute_immediate(string s)	// 20
{
	return (is_number(s) || is_hex_number(s));
}
bool symbol_immediate(string s)					// &x
{
	if (s[0] != '&')
		return false;
	string s1 = s.substr(1);
	return is_variable_correct(s1);
}
bool symbol_absolute(string s)							// x
{
	return (is_variable_correct(s) && !reg_dir(s) && !reg_dir_h(s) && !reg_dir_l(s) && !reg_indir_h_symbol_offset(s) && !reg_indir_l_symbol_offset(s));
}
bool location_from_the_address(string s)		// *20
{
	if (s[0] != '*')
		return false;
	string s1 = s.substr(1);
	return is_number(s1);
}
bool reg_dir(string s)					  		// r3
{
	if (s == "r0") return true;
	if (s == "r1") return true;
	if (s == "r2") return true;
	if (s == "r3") return true;
	if (s == "r4") return true;
	if (s == "r5") return true;
	if (s == "r6") return true;
	if (s == "r7") return true;
	if (s == "pc") return true;
	if (s == "sp") return true;

	return false;
}
bool reg_indir_absolute_offset(string s) 		// r4[32]  r4[0x32]
{
	string s1 = s.substr(0, 2);
	if (reg_dir(s1) == false)
		return false;
	if (s[s.length() - 1] != ']')
		return false;
	string s2 = s.substr(3, s.length() - 4);
	return (is_number(s2) || (is_hex_number(s2)));
}
bool reg_indir_symbol_offset(string s)	  		// r5[x]
{
	string s1 = s.substr(0, 2);
	if (reg_dir(s1) == false)
		return false;
	if (s[s.length() - 1] != ']')
		return false;
	string s2 = s.substr(3, s.length() - 4);
	return is_variable_correct(s2);
}
bool pc_rel_offset(string s)		      		// $x
{
	if (s[0] != '$')
		return false;
	string s1 = s.substr(1);
	return is_variable_correct(s1);
}
bool reg_dir_h(string s)
{
	return (reg_dir(s.substr(0, s.length() - 1)) && ((s[s.length() - 1] == 'h') || (s[s.length() - 1] == 'H')));
}
bool reg_dir_l(string s)
{
	return (reg_dir(s.substr(0, s.length() - 1)) && ((s[s.length() - 1] == 'l') || (s[s.length() - 1] == 'L')));
}
bool reg_indir_h_absolute_offset(string s) 		// r4h[32]
{
	string s1 = s.substr(0, 3);
	if (reg_dir_h(s1) == false)
		return false;
	if (s[s.length() - 1] != ']' || s[3] != '[')
		return false;
	string s2 = s.substr(4, s.length() - 5);
	return ((is_number(s2)) || (is_hex_number(s2)));
}
bool reg_indir_l_absolute_offset(string s) 		// r4l[32]
{
	string s1 = s.substr(0, 3);
	if (reg_dir_l(s1) == false)
		return false;
	if (s[s.length() - 1] != ']' || s[3] != '[')
		return false;
	string s2 = s.substr(4, s.length() - 5);
	return ((is_number(s2)) || (is_hex_number(s2)));
}
bool reg_indir_h_symbol_offset(string s)	  	// r5h[x]
{
	string s1 = s.substr(0, 3);
	if (reg_dir_h(s1) == false)
		return false;
	if (s[s.length() - 1] != ']')
		return false;
	string s2 = s.substr(4, s.length() - 5);
	return is_variable_correct(s2);
}
bool reg_indir_l_symbol_offset(string s)	  	// r5l[x]
{
	string s1 = s.substr(0, 3);
	if (reg_dir_l(s1) == false)
		return false;
	if (s[s.length() - 1] != ']')
		return false;
	string s2 = s.substr(4, s.length() - 5);
	return is_variable_correct(s2);
}
bool reg_indir(string s)					// [r4]
{	
	if (s[0] != '[' || s[s.length() - 1] != ']')
		return false;
	string ss = s.substr(1, s.length() - 2);
	return reg_dir(ss);
}
bool reg_indir_h(string s)				// [r4h]
{
	if (s[0] != '[' || s[s.length() - 1] != ']')
		return false;
	string ss = s.substr(1, s.length() - 2);
	return reg_dir_h(ss);
}
bool reg_indir_l(string s)				// [r4l]
{
	if (s[0] != '[' || s[s.length() - 1] != ']')
		return false;
	string ss = s.substr(1, s.length() - 2);
	return reg_dir_l(ss);
}
bool is_hex_number(string s)				// 0x1234
{
	if (s[0] != '0' || s[1] != 'x')
		return false;
	int i;
	for (i = 2; i < s.length(); i++)
	{
		if (!((s[i] >= '0') && (s[i] <= '9')) && (s[i] != 'A') && (s[i] != 'B') && (s[i] != 'C') && (s[i] != 'D') && (s[i] != 'E') && (s[i] != 'F')) return false;
	}
	return true;
}
int length_of_hex_number(string s)			// 0x1234 --> length = 4
{
	return s.substr(2).length();
}
int return_num_of_registar_from_string(string s, int i)	// r2 ---> 2		pc ---> 7	
{
	int r = 0;
	if (s.substr(i - 1, 2) == "sp")
	{	r = 6;			}
	else if (s.substr(i - 1, 2) == "pc")
	{	r = 7;			}
	else 
	{	r = (s)[i] - '0';	}
	return r;
}
string convert_from_int_to_Byte(int x)
{
	string ret = "";
	int i;
	i = x / 16;
	ret += nibble_to_16(i);
	i = x % 16;
	ret += nibble_to_16(i);
	return ret;
}
int number_of_digits(int n)
{
	if (n == 0) return 1;
	int k = 0;
	if (n < 0)
	{
		n = (-1)* n;
		k++;
	}
	while(n > 0)
	{
		n = n/10;
		k++;
	}
	return k;
}

