/*
 * SymbolTable.h
 *
 *  Created on: Feb 19, 2019
 *      Author: user
 */


#ifndef SYMBOLTABLE_H_
#define SYMBOLTABLE_H_

#include "Symbol.h"
#include "Section.h"
#include <iostream>
#include <fstream>
#include <sstream>
#include <string>


struct SymbolElem
{
	Symbol *symbol;
	SymbolElem *next;
public:
	SymbolElem(Symbol *s)
	{
		symbol = s;
		next = 0;
	}
};

struct SectionElem
{
	Section *section;
	SectionElem *next;
	SectionElem(Section *s)
	{
		section = s;
		next = 0;
	}
};

class SymbolTable
{

public:

	SymbolElem *symbolHead, *symbolTail;
	SectionElem *sectionHead, *sectionTail;
	SectionElem *currentSection;

	static int globID;

	SymbolTable();
	virtual ~SymbolTable();

	Symbol* getSymbol(string name);
	Section* getSection(string name);

	Symbol* getSymbolByID(int id);
	Section* getSectionByID(int id);

	Symbol* getSymbolByOldName(string name);
	Section* getSectionByOldName(string name);

	void insertSymbol(Symbol* s);
	void insertSection(Section* s);

	ofstream& printSymbols(ofstream& myOutputFile);
	ofstream& printSections(ofstream& myOutputFile);

	void increment_ID_of_all_symbols();

	SectionElem* getFirstSection() { return this->sectionHead; }
	SymbolElem* getFirstSymbol() { return this->symbolHead; }

	int getValueById(int id);
};

#endif /* SYMBOLTABLE_H_ */
