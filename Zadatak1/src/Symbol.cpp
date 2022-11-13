/*
 * Symbol.cpp
 *
 *  Created on: Feb 19, 2019
 *      Author: user
 */

#include "Symbol.h"
#include "SymbolTable.h"


Symbol::Symbol(string name, int sectionNo, Section* mySection, int value, bool global, bool isDefinedByEQU)
{
	this->name = name;
	this->sectionNo = sectionNo;
	this->mySection = mySection;
	this->value = value;
	this->global = global;
	ID = SymbolTable::globID++;
	this->oldName = "";
	this->isDefinedByEQU = isDefinedByEQU;
}

Symbol::~Symbol()
{

}

