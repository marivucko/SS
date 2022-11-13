/*
 * SymbolTable.cpp
 *
 *  Created on: Feb 19, 2019
 *      Author: user
 */

using namespace std;
#include <iostream>
#include "SymbolTable.h"
#include "AuxiliaryClass.h"

int SymbolTable::globID = 0;


SymbolTable::SymbolTable()
{
	symbolHead = 0;
	symbolTail = 0;

	sectionHead = 0;
	sectionTail = 0;

	currentSection = 0;
}

SymbolTable::~SymbolTable()
{
	SymbolElem* symbolCurr = symbolHead;

	while(symbolCurr != 0)
	{
		SymbolElem* old = symbolCurr;
		symbolCurr = symbolCurr->next;
		delete old;
	}

	SectionElem* sectionCurr = sectionHead;

	while(sectionCurr != 0)
	{
		SectionElem* old = sectionCurr;
		sectionCurr = sectionCurr->next;
		delete old;
	}

	delete currentSection;
}

Symbol* SymbolTable::getSymbol(string name)
{
	SymbolElem* currElem = symbolHead;
	while (currElem != 0 && currElem->symbol->getName() != name)
	{
		currElem = currElem->next;
	}
	if (currElem == 0)
		return 0;
	else
		return currElem->symbol;
}

Section* SymbolTable::getSection(string name)
{
	SectionElem* currElem = sectionHead;
	while (currElem != 0 && currElem->section->getName() != name)
	{
		currElem = currElem->next;
	}
	if (currElem == 0)
		return 0;
	else
		return currElem->section;
}

Symbol* SymbolTable::getSymbolByID(int id)
{
	SymbolElem* currElem = symbolHead;
	while (currElem != 0 && currElem->symbol->getID() != id)
	{
		currElem = currElem->next;
	}
	if (currElem == 0)
		return 0;
	else
		return currElem->symbol;
}

Section* SymbolTable::getSectionByID(int id)
{
	SectionElem* currElem = sectionHead;
	while (currElem != 0 && currElem->section->getID() != id)
	{
		currElem = currElem->next;
	}
	if (currElem == 0)
		return 0;
	else
		return currElem->section;
}

Symbol* SymbolTable::getSymbolByOldName(string name)
{
	SymbolElem* currElem = symbolHead;
	while (currElem != 0 && currElem->symbol->getOldName() != name)
	{
		currElem = currElem->next;
	}
	if (currElem == 0)
		return 0;
	else
		return currElem->symbol;
}

Section* SymbolTable::getSectionByOldName(string name)
{
	SectionElem* currElem = sectionHead;
	while (currElem != 0 && currElem->section->getOldName() != name)
	{
		currElem = currElem->next;
	}
	if (currElem == 0)
		return 0;
	else
		return currElem->section;
}



void SymbolTable::insertSymbol(Symbol* s)
{
	Symbol* e = getSymbol(s->getName());
	if (e != 0)
	{
		return;
	}

	SymbolElem* newElem = new SymbolElem(s);

	if (symbolHead == 0)
	{
		symbolHead = newElem;
	}
	else
	{
		symbolTail->next = newElem;
	}
	symbolTail = newElem;
}

void SymbolTable::insertSection(Section* s)
{
	Section* e = getSection(s->getName());
	if (e != 0)
	{
		return;
	}

	SectionElem* newElem = new SectionElem(s);

	if (sectionHead == 0)
	{
		sectionHead = newElem;
	}
	else
	{
		sectionTail->next = newElem;
	}
	sectionTail = newElem;
}


ofstream& SymbolTable::printSymbols(ofstream& myOutputFile)
{
	SymbolElem* currElem = symbolHead;
	myOutputFile << "NAME                                       SEC                    SecN       VAL       GLOB      ID\n";
	while (currElem != 0)
	{
		myOutputFile << currElem->symbol->getName();
		for(int i = 0; i < (45 - (signed)currElem->symbol->getName().length()); i++) myOutputFile << " ";
		myOutputFile << currElem->symbol->getSectionNo();
		for(int i = 0; i < (24 - (signed)currElem->symbol->getSectionName().length()); i++) myOutputFile << " ";
		myOutputFile << currElem->symbol->getSectionName();
		for(int i = 0; i < (10 - (signed)number_of_digits(currElem->symbol->getValue())); i++) myOutputFile << " ";
		myOutputFile << currElem->symbol->getValue()<<"          ";
		myOutputFile << currElem->symbol->isGlobal();
		for(int i = 0; i < (8 - number_of_digits(currElem->symbol->getID())); i++) myOutputFile << " ";
		myOutputFile << currElem->symbol->getID() << "\n";
		currElem = currElem->next;
	}
	myOutputFile << "\n";
	return myOutputFile;
}

ofstream& SymbolTable::printSections(ofstream& myOutputFile)
{
	SectionElem* currElem = sectionHead;
	myOutputFile << "NAME               SEC     VAL     GLOB     ID           SIZE           startAddress     RWE\n";
	while (currElem != 0)
	{
		string s = currElem->section->getName();
		//if ((s != ".text") && (s != ".data") && (s != ".rodata") && (s != ".bss"))
		//	s = ".section_" + s;
		myOutputFile << s;
		for(int i = 0; i < (20 - (signed)s.length()); i++) myOutputFile << " ";
		myOutputFile << currElem->section->getSection() << "       " << currElem->section->getValue() << "        ";
		myOutputFile << currElem->section->isGlobal() << "       " << currElem->section->getID();;
		for(int i = 0; i < (15 - number_of_digits(currElem->section->getSize())); i++) myOutputFile << " ";
		myOutputFile << currElem->section->getSize();
		for(int i = 0; i < (23 - number_of_digits(currElem->section->getStartAddress())); i++) myOutputFile << " ";
		myOutputFile << currElem->section->getStartAddress() << "     "; 
		myOutputFile << currElem->section->getR() << currElem->section->getW() << currElem->section->getE() << "\n";
		currElem = currElem->next;
	}
	myOutputFile << "\n";
	return myOutputFile;
}

void SymbolTable::increment_ID_of_all_symbols()
{
	SymbolElem* currElem = symbolHead;
	while (currElem != 0)
	{
		currElem->symbol->setID(currElem->symbol->getID() + 1);
		currElem = currElem->next;
	}
	//SymbolTable::globID++;
}

int SymbolTable::getValueById(int id)
{
	SectionElem* currSec = sectionHead;
	while ((currSec != 0) && (currSec->section->getID()) != id)
		currSec = currSec->next;
	if (currSec != 0)
		return currSec->section->getStartAddress();
	SymbolElem* currSym = symbolHead;
	while ((currSym != 0) && (currSym->symbol->getID()) != id)
		currSym = currSym->next;
	if (currSym != 0)
		return currSym->symbol->getValue();
	return -1;
}

