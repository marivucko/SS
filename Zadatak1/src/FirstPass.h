 /* FirstPass.h
 *
 *  Created on: Feb 20, 2019
 *      Author: user
 */

#ifndef FIRSTPASS_H_
#define FIRSTPASS_H_

using namespace std;

#include "SymbolTable.h"
#include <iostream>
#include <fstream>
#include <sstream>
#include <string>



class Line
{

public:
	string word;					// list of words from text file
	Line* nextWord;
	Line(string s)
	{
		word = s;
		nextWord = 0;
	}
};

class LineList
{

public:
	Line* line;						// list of lines
	LineList* nextLine;
	int numberOfLineInFirstPass;
	LineList(Line* line, int n)
	{
		this->line = line;
		this->nextLine = 0;
		this->numberOfLineInFirstPass = n;
	}
};

class RoDataLabels
{

public:
	string label;
	RoDataLabels *next;
	RoDataLabels(string label)
	{
		this->label = label;
		this->next = 0;
	}
};


class SyntaxError
{
	int lineNo;
	string error;
public:
	SyntaxError(int lineNo, string error)
	{
		this->lineNo = lineNo;
		this->error = error;
	}
	int getLineNo()
	{
		return lineNo;
	}
	string getError()
	{
		return error;
	}
};


struct TNS
{
	int ordinalNumber;
	string label;
	Line* postfixHead;
	TNS* next;
public:
	TNS(string label, Line* postfixHead)
	{
		this->ordinalNumber = glob_TNS_OrdinalNumber++;
		this->label = label;
		this->postfixHead = postfixHead;
		next = 0;
	}

static int glob_TNS_OrdinalNumber;

bool is_Label_already_menthioned_in_previos_labels_of_TNS(string label, int ordinalNumber, TNS* tnsHead)
{
	bool ret = false;
	TNS* currTNS = tnsHead;
	int i = 0;
	while (currTNS != 0 && i <= ordinalNumber)
	{
		if (currTNS->label == label)
			return true;
		currTNS = currTNS->next;
		i++;
	}
	return ret;
}

};

class FirstPass
{

	int locationCounter;
	int lineCounter;

	string inputFileName;
	string outputFileName;

	ofstream myOfstream;

	bool end_Of_FirstPass, end_of_line;
	Section *undefiendSection;
	Section* currentSection;

	int numOfSections;

	TNS* my_TNS_head;
	TNS* my_TNS_tail;
	//int my_TNS_counter;

	LineList* myLineListHead;
	LineList* myLineListTail;

	RoDataLabels* roDataLabelsHead;
	RoDataLabels* roDataLabelsTail;

	int startSectionsAddress;

	SymbolTable* symbolTable;

public:

	friend class AuxiliaryClass;

	FirstPass(string inputFileName, string outputFileName, int startAddress);
	virtual ~FirstPass();

	void firstPass();

	bool isEnd(string s);

	bool isGlobal();
	bool isExtern();

	bool isEQU(string firstWord);
	bool isLabel();

	bool isSection();
	bool isTextSection();
	bool isDataSection();
	bool isRoDataSection();
	bool isBssSection();

	bool isByte();
	bool isWord();
	bool isAlign();
	bool isSkip();

	bool isInstruction(string firstWord);
	void executeInstruction(string mnemonic, Line** currWord);

	void executeGlobal(Line** words, string glob_extern);
	void executeExtern(Line** words, string glob_extern);

	bool isLabel(string s);
	void separate_label_from_the_rest_of_line(string s, string* label, string* rest_of_line);
	void executeLabel(string firstWord, string* line);

	bool isEQU(Line** words);								// a EQU b --- words = {EQU, b}
	void executeEQU(string firstWord, Line** words, int n); 		// firstWord = a; words = {EQU, b}; n -- number of all words + firstWord

	bool isSection(string s, Line** currWord);
	void executeSection(string s, Line** words, int r, int w, int e);

	bool is_Byte_Word_Align_Skip(string firstWord);
	void execute_Byte_Word_Align_Skip(string firstWord, Line** currWord, string* line);

	void try_to_add_new_symbol_to_symbolTable(string s, int value, bool isDefinedByEQU);


	void eval_postfix(string firstWord, Line** postfix, int lineCounter, bool* finished, int* result, bool afterFirstPass);

	bool is_symbol_in_label_part_of_TNS(string symbolName);
	void add_line_in_TNS(string label, Line** postfix);
	TNS* get_first_line_from_TNS();
	int number_of_lines_in_TNS();
	void print_TNS();
	void check_circular_dependencies_TNS(string ops2, string firstWord, bool* finished, int* rez, Line** stackHead, Line** stackTail, bool afterFirstPass);

	void calculate_TNS();

	void insertLineToLineList(Line* l, int n);
	void printLineList();

	bool two_lines_are_equal(Line* first, Line* second);

	void separte_commas_from_string(Line** currWord, Line** line, int* num);

	bool are_all_symbols_in_list_already_defined(Line** line);
	void add_label_in_RoDataLabels(string label);
	void print_RoDataLabels();

};

#endif /* FIRSTPASS_H_ */
