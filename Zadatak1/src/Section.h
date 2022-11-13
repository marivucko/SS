/*
 * Section.h
 *
 *  Created on: Feb 19, 2019
 *      Author: user
 */

#ifndef SECTION_H_
#define SECTION_H_
#include <iostream>
using namespace std;


class Section
{
private:
	string name;
	int value;
	bool global;
	int ID;
	int size;
	int startAddress;
	int r, w, e;
	string oldName;
public:
	Section(string name, int value, bool global, int size, int r, int w, int e);
	virtual ~Section();

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
	int getSection() const
	{
		return ID;
	}
	void setSection(int section)
	{
		this->ID = section;
	}
	int getSize() const
	{
		return size;
	}
	void setSize(int size)
	{
		this->size = size;
	}
	int getValue() const
	{
		return value;
	}
	void setValue(int value)
	{
		this->value = value;
	}
	void setStartAddress(int startAddress)
	{
		this->startAddress = startAddress;
	}
	int getStartAddress()
	{
		return this->startAddress;
	}
	int getR()
	{
		return r;
	}
	void setR(int r)
	{
		this->r = r;
	}
	int getW()
	{
		return w;
	}
	void setW(int w)
	{
		this->w = w;
	}
	int getE()
	{
		return e;
	}
	void setE(int e)
	{
		this->e = e;
	}
	void setOldName(string oldName)
	{
		this->oldName = oldName;
	}
	string getOldName()
	{
		return this->oldName;
	}
};

#endif /* SECTION_H_ */
