/*
 * Section.cpp
 *
 *  Created on: Feb 19, 2019
 *      Author: user
 */

#include "Section.h"
#include "SymbolTable.h"

Section::Section(string name, int value, bool global, int size, int r, int w, int e)
{
	this->name = name;
	this->value = value;
	this->global = global;
	ID = SymbolTable::globID++;
	this->size = size;
	this->startAddress = 0;
	this->r = r;
	this->w = w;
	this->e = e;
}

Section::~Section() {

}

