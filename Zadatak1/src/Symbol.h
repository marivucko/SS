/*
 * Symbol.h
 *
 *  Created on: Feb 19, 2019
 *      Author: user
 */

#ifndef SYMBOL_H_
#define SYMBOL_H_
#include "Section.h"
#include <iostream>
using namespace std;

class Symbol
{

private:
	string name;
	int sectionNo;
	Section *mySection;
	int value;
	bool global;
	int ID;
	string oldName;
	bool isDefinedByEQU;

public:


	Symbol(string name, int sectionNo, Section* mySection, int value, bool global, bool isDefinedByEQU);
	virtual ~Symbol();

	bool isGlobal() const
	{
		return global;
	}

	void setGlobal(bool global)
	{
		this->global = global;
	}

	int getID() const
	{
		return ID;
	}

	void setID(int id)
	{
		ID = id;
	}

	string getName() const
	{
		return name;
	}

	void setName(string name)
	{
		this->name = name;
	}

	int getSectionNo() const
	{
		return sectionNo;
	}

	void setSectionNo(int sectionNo)
	{
		this->sectionNo = sectionNo;
	}

	string getSectionName()
	{
		return this->mySection->getName();
	}

	void setSection(Section* mySection)
	{
		this->mySection = mySection;
	}
	int getValue() const
	{
		return value;
	}

	void setValue(int value)
	{
		this->value = value;
	}

	void setIsDefinedByEQU(bool isDefinedByEQU)
	{
		this->isDefinedByEQU = isDefinedByEQU;
	}

	Section* getMySection()
	{
		return mySection;
	}

	bool getIsDefinedByEQU()
	{
		return isDefinedByEQU;
	}

	int getSectionStartAddress()
	{
		return this->mySection->getStartAddress();
	}
	string getOldName()
	{
		return this->oldName;
	}
	void setOldName(string oldName)
	{
		this->oldName = oldName;
	}
};


#endif /* SYMBOL_H_ */
