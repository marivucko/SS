/*
 * SecondPass.h
 *
 *  Created on: Mar 11, 2019
 *      Author: user
 */

#ifndef SECONDPASS_H_
#define SECONDPASS_H_

#include "FirstPass.h"
#include "SymbolTable.h"
#include "AuxiliaryClass.h"
#include <iostream>
#include <istream>
#include <fstream>
#include <sstream>
#include <string>
#include <list>
#include <vector>

class ByteList
{

public:
	string byte;
	ByteList* next;

	ByteList(string b)
	{
		byte = b;
		next = 0;
	}
};

enum R_type {absolute, relative};

class Ret
{

public:
	int offset;
	R_type r_type;
	char sign;
	int value;
	Ret* next;
	int numOfBytes;
	string reference;

	Ret(int offset, R_type r_type, char sign, int value, int numOfBytes, string reference)
	{
		this->sign = sign;
		this->offset = offset;
		this->r_type = r_type;
		this->value = value;
		next = 0;
		this->numOfBytes = numOfBytes;
		this->reference = reference;
	}

	void setReference(string reference)
	{
		this->reference = reference;
	}
};

class C
{
public:
	string sectionName;
	ByteList* byte_List_Head;
	ByteList* byte_List_Tail;
	Ret*	  ret_Head;
	Ret*      ret_Tail;
	
	C(string sectionName, ByteList* byte_List_Head, ByteList* byte_List_Tail, Ret* ret_Head, Ret* ret_Tail)
	{
		this->sectionName = sectionName;
		this->byte_List_Head = byte_List_Head;
		this->byte_List_Tail = byte_List_Tail;
		this->ret_Head = ret_Head;
		this->ret_Tail = ret_Tail;
	}
};

class SecondPass
{
	LineList* myLineListHead;
	int locationCounter;
	int lineCounter;
	LineList* currLine;
	int numOfSections;
	Section* currentSection;
	SymbolTable* symbolTable;

	ByteList* text_Head; 	ByteList* text_Tail;
	ByteList* data_Head; 	ByteList* data_Tail;

	Ret* ret_text_Head;	Ret* ret_text_Tail;
	Ret* ret_data_Head;	Ret* ret_data_Tail;

	string outputFileName;

	list<C> sectionC;

	//ofstream myOfstream;
	int startSectionsAddress;

public:

	friend class FirstPass;

	SecondPass(LineList* myLineListHead, int numOfSections, SymbolTable* symbolTable, string outputFilenName, int startAddress);
	virtual ~SecondPass();

	ofstream& secondPass(ofstream& myOfstream);

	void executeGlobal(Line* words, string glob_extern);							//
	void executeExtern(Line* words, string glob_extern);							//
	void executeLabel(Line* words);		// doesn't do anytihng in the second pass	//
	void executeEQU(Line* words);

	void executeText();																//
	void executeData();																//
	void executeSection(string sectionName);															//
	void executeBss();																//

	void executeByteWord(Line* words, int num);	// i = 1 write 1B, i = 2 write 2B, i = 4, write 4B												//
	void executeByte(Line* words);	
	void executeWord(Line* words);													//
	void executeAlign(Line* words);													//
	void executeSkip(Line* words);													//
	void executeInstruction(Line* words);



	void insert_Byte_in_ByteList(string Byte, ByteList** head, ByteList** tail);
	void insert_Byte_in_ByteList(string Byte, string sectionName);
	ofstream& print_ByteList(string sectionName, ByteList** head, ofstream& myOfstream);
	ofstream& print_ByteList(string sectionName, ofstream& myOfstream);
	void insert_in_Ret(int offset, R_type r_type, char sign, int value, Ret** head, Ret** tail, int numOfBytes, string reference);
	void insert_in_Ret(int offset, R_type r_type, char sign, int value, int numOfBytes, string sectionName, string reference);
	ofstream& print_Ret(string sectionName, Ret* head, ofstream& myOfstream);
	ofstream& print_Ret(string sectionName, ofstream& myOfstream);

	void insert_1B_in_CurrentSection_ByteList(string Byte);
	void insert_int_to_4B_in_CurrentSection_ByteList(int number);
	void insert_int_to_2B_in_CurrentSection_ByteList(int number);
	void insert_int_to_1B_in_CurrentSection_ByteList(int number);
	void insert_in_CurrentSection_Ret(int offset, R_type r_type, char sign, int value, int numOfBytes, string reference);
	void insert_N_emptyBytes_in_CurrentSection_ByteList(int N);

};

#endif /* SECONDPASS_H_ */
