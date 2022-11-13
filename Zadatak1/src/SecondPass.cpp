/*
 * SecondPass.cpp
 *
 *  Created on: Mar 11, 2019
 *      Author: user
 */

using namespace std;
#include <iostream>
#include "SecondPass.h"
#include "FirstPass.h"

SecondPass::SecondPass(LineList* myLineListHead, int numOfSections, SymbolTable* symbolTable, string outputFilenName, int startAddress)
{
	this->myLineListHead = myLineListHead;
	lineCounter = 0;
	locationCounter = 0;
	currLine = myLineListHead;
	currentSection = new Section("UND", 0, false, 0, 0, 0, 0); 			// undefiendSection
	this->numOfSections = numOfSections;
	this->symbolTable = symbolTable;

	text_Head = 0;			text_Tail = 0;
	data_Head = 0;		  	data_Tail = 0;

	ret_text_Head = 0;		ret_text_Tail = 0;
	ret_data_Head = 0;		ret_data_Tail = 0;

	this->outputFileName = outputFileName;

	//myOfstream.open(outputFilenName.c_str());

	startSectionsAddress = startAddress;
}

SecondPass::~SecondPass()
{

}

ofstream& SecondPass::secondPass(ofstream& myOfstream)
{
	while (currLine != 0)
	{
		string s = currLine->line->word;

		try
		{
			if (s == ".global")
				executeGlobal(currLine->line->nextWord, "global");
			else if (s == ".extern")
				executeExtern(currLine->line->nextWord, "extern");
			else if (s == ".text")
				executeText();
			else if (s == ".data")
				executeData();
			else if (s.substr(0, 8) == ".section")
			{
				string sn = s.substr(9, s.length() - 9);
				C secC(sn, 0, 0, 0, 0);
				sectionC.push_back(secC);
				executeSection(sn);
			}
			else if (s == ".bss")
			{
				C secC(".bss", 0, 0, 0, 0);
				sectionC.push_back(secC);
				executeSection(".bss");
				executeBss();
			}
			else if (s == ".byte")
				executeByte(currLine->line->nextWord);
			else if (s == ".word")
				executeWord(currLine->line->nextWord);
			else if (s == ".align")
				executeAlign(currLine->line->nextWord);
			else if (s == ".skip")
				executeSkip(currLine->line->nextWord);
			else { //cout << "!!!LC!!!" << locationCounter << "PRE\n";
				executeInstruction(currLine->line);
				//cout << "!!!LC!!!" << locationCounter << "POSLE\n";
				}


		} catch (SyntaxError* s) { cout << "Syntax error at line " << s->getLineNo() << ". "<< s->getError() << "\n"; }


		currLine = currLine->nextLine;
		lineCounter++;
	}


	if (symbolTable->getSection(".text") != 0)
		print_ByteList(".text", &text_Head, myOfstream);
	if (symbolTable->getSection(".data") != 0)
		print_ByteList(".data", &data_Head, myOfstream);
	for (list<C>::iterator it = sectionC.begin(); it != sectionC.end(); it++)
   		print_ByteList(it->sectionName, myOfstream);					


	if (symbolTable->getSection(".text") != 0)
		print_Ret(".text", ret_text_Head, myOfstream);
	if (symbolTable->getSection(".data") != 0)
		print_Ret(".data", ret_data_Head, myOfstream);
	for (list<C>::iterator it = sectionC.begin(); it != sectionC.end(); it++)
   		print_Ret(it->sectionName, myOfstream);

	/*myOfstream << "\n\n LINKER LOADER \n\n";
	LinkerLoader* linkload = new LinkerLoader(symbolTable, text_Head, text_Tail, data_Head, data_Tail, rodata_Head, rodata_Tail, ret_text_Head, ret_text_Tail, ret_data_Head, ret_data_Tail, ret_rodata_Head, ret_rodata_Tail, outputFileName, startSectionsAddress);
	linkload->link(myOfstream);
	linkload->load();
	if (symbolTable->getSection(".text") != 0)
		print_ByteList(".text", &text_Head, myOfstream);
	if (symbolTable->getSection(".data") != 0)
		print_ByteList(".data", &data_Head, myOfstream);
	if (symbolTable->getSection(".rodata") != 0)
		print_ByteList(".rodata", &rodata_Head, myOfstream);*/

	//myOfstream.close();

	return myOfstream;
}

void SecondPass::executeGlobal(Line* words, string glob_extern)
{
	Line* currWord = words;
	Line* symbol = 0;
	while (currWord != 0)
	{
		Symbol* s = symbolTable->getSymbol(currWord->word);
		//if (s!=0)cout << s->getName();
		if ((s != 0) && (s->getSectionName() == "UND"))
		{
			Line* sym = new Line(s->getName());
			add_words_at_the_end_of_first_line(&symbol, &sym);
		}
		currWord = currWord->nextWord;
	}
	//cout << "***********\n";
	Line* curr = symbol;
	while (curr != 0)
	{
		cout << "Syntax error at line " << currLine->numberOfLineInFirstPass << ". Error: symbol " << curr->word << " is " << glob_extern << " but not defined.\n";
		Line* old = curr;
		curr = curr->nextWord;
		delete old;
	}
}

void SecondPass::executeExtern(Line* words, string glob_extern)
{
	//executeGlobal(words, glob_extern);
}

void SecondPass::executeLabel(Line* words)
{

}

void SecondPass::executeEQU(Line* words)
{

}

void SecondPass::executeText()
{
	locationCounter = 0;
	currentSection = symbolTable->getSection(".text");
	//cout << "........................\n" << currentSection->getID() << "\n..........................\n"; 
}

void SecondPass::executeData()
{
	locationCounter = 0;
	currentSection = symbolTable->getSection(".data");
}
void SecondPass::executeSection(string sectionName)
{
	locationCounter = 0;
	currentSection = symbolTable->getSection(sectionName);
	//cout << "aaaaaaaaaaa" << currentSection->getName() << "\n";
}

void SecondPass::executeBss()
{
	locationCounter = 0;
	currentSection = symbolTable->getSection(".bss");
}

void SecondPass::executeByteWord(Line* words, int num)
{
	Line* curr = words;
	int result = 0;
	while (curr != 0)
	{
		string s = curr->word;
		Line* labels;
		Line* signs;//cout << "OVDEEE" << s <<"\n";
		get_words_between_plus_and_minus(s, &numOfSections, &labels, &signs);
		while (labels != 0)
		{
			string s = labels->word;
			if (is_number(s))
			{
				if (signs->word == "+")
					result += convert_to_int(s);
				else
					result -= convert_to_int(s);
			}
			else if (is_hex_number(s))
			{//cout << "OVDEEE" << s <<"\n";
				int a = 0;
				if (s.substr(2,1) == "0" || s.substr(2,1) == "1" || s.substr(2,1) == "2" || s.substr(2,1) == "3" 
				 || s.substr(2,1) == "4" || s.substr(2,1) == "5" || s.substr(2,1) == "6" || s.substr(2,1) == "7" )
					a = convert_to_int_from_hex_when_hex_is_positive(s.substr(2));
				else 
					a = convert_to_int_from_hex(s.substr(2)); 
				if (signs->word == "+")
					result += a;
				else
					result -= a;
			}
			else
			{
				Symbol* symbol = symbolTable->getSymbol(s);
				if (symbol != 0)
				{
					if (symbol->isGlobal() == 0) 	// ovaj red je dodat
					{	if (signs->word == "+")
							result += symbol->getValue();
						else
							result -= symbol->getValue();
					}

					if (symbol->isGlobal() == true)
					{
						R_type r = absolute;
						char c = '+';
						if (signs->word != "+")
							c = '-';
						insert_in_CurrentSection_Ret(locationCounter, r, c, symbol->getID(), num, symbol->getName());
					}
					else
					{
						R_type r = absolute;
						char c = '+';
						if (signs->word != "+")
							c = '-';
						insert_in_CurrentSection_Ret(locationCounter, r, c, symbol->getSectionNo(), num, symbol->getSectionName());
					}
				}
				else
				{
					Section* sec = symbolTable->getSection(s);
					if (sec != 0)
					{
						R_type r = absolute;
						char c = '+';
						if (signs->word != "+")
							c = '-';
						insert_in_CurrentSection_Ret(locationCounter, r, c, sec->getID(), num, symbol->getSectionName());
					}
				}
			}

			labels = labels->nextWord;
			signs = signs->nextWord;
		}
		if (num == 4)
		{
			insert_int_to_4B_in_CurrentSection_ByteList(result);
			locationCounter += 4;
		}
		else if (num == 2)
		{
			insert_int_to_2B_in_CurrentSection_ByteList(result);
			locationCounter += 2;
		}
		else if (num == 1)
		{
			insert_int_to_1B_in_CurrentSection_ByteList(result);
			locationCounter += 1;
		}
		result = 0;
		curr = curr->nextWord;
	}
}

void SecondPass::executeByte(Line* words)
{
	executeByteWord(words, 1);
}

void SecondPass::executeWord(Line* words)
{
	executeByteWord(words, 2);
}

void SecondPass::executeAlign(Line* words)
{
	string s = words->word;
	int i = convert_to_int(s);
	i = 1 << i;
	while (locationCounter % i != 0)
	{
		locationCounter += 1;
		insert_1B_in_CurrentSection_ByteList("00");
	}
	//if (locationCounter + i > 0)				// (i > 0) ??
	//	locationCounter += i;
	//if (i > 0)
	//	insert_N_emptyBytes_in_CurrentSection_ByteList(i);
}

void SecondPass::executeSkip(Line* words)
{
	string s = words->word;
	int i = convert_to_int(s);
	if (locationCounter + i > 0)				// (i > 0) ??
		locationCounter += i;
	if (i > 0)
		insert_N_emptyBytes_in_CurrentSection_ByteList(i);
}

void SecondPass::executeInstruction(Line* words)
{
	int permutated = 1;
	int instructionSize = 4;
	bool pc_rel_dst = false;
	//if (currentSection->getName() == ".data")
	//{
	//	throw new SyntaxError(lineCounter, "Error: only .text section can be executed (can have instructions).");
	//	return;
	//}
	Line* curr = words;
	int byte[8]; bool byte_E[8]; int i;
	for(i = 1; i < 8; i++)
	{
		byte[i] = 0;
		byte_E[i] = false;
	}
	
	int _3_4_Byte = 0;
	bool _3_4_ByteE = false;
	bool dest_operand_1B = false;	string dst_operand_1B_value = "";		// int r2h[12]
	string mnemonic = curr->word;
	int w = 1;
	if (mnemonic[mnemonic.length() - 1 ] == 'b' && mnemonic != "sub") w = 0;
	int jmp_add = 0; int jmp_mov = 0;

	if (curr->word == "halt") { byte[1] += 1 << 3; insert_1B_in_CurrentSection_ByteList(convert_from_int_to_Byte(byte[1])); locationCounter += 1; return; }
	else if (curr->word == "xchg" || curr->word == "xchgw" || curr->word == "xchgb")  byte[1] += 2 << 3;
	else if (curr->word == "int"  || curr->word == "intw"  || curr->word == "intb" )  byte[1] += 3 << 3;
	else if (curr->word == "mov"  || curr->word == "movw"  || curr->word == "movb" )  byte[1] += 4 << 3;
	else if (curr->word == "add"  || curr->word == "addw"  || curr->word == "addb" )  byte[1] += 5 << 3;
	else if (curr->word == "sub"  || curr->word == "subw"  || curr->word == "subb" )  byte[1] += 6 << 3;
	else if (curr->word == "mul"  || curr->word == "mulw"  || curr->word == "mulb" )  byte[1] += 7 << 3;
	else if (curr->word == "div"  || curr->word == "divw"  || curr->word == "divb" )  byte[1] += 8 << 3;
	else if (curr->word == "cmp"  || curr->word == "cmpw"  || curr->word == "cmpb" )  byte[1] += 9 << 3;
	else if (curr->word == "not"  || curr->word == "notw"  || curr->word == "notb" )  byte[1] += 10 << 3;
	else if (curr->word == "and"  || curr->word == "andw"  || curr->word == "andb" )  byte[1] += 11 << 3;
	else if (curr->word == "or"   || curr->word == "orw"   || curr->word == "orb"  )  byte[1] += 12 << 3;
	else if (curr->word == "xor"  || curr->word == "xorw"  || curr->word == "xorb" )  byte[1] += 13 << 3;
	else if (curr->word == "test" || curr->word == "testw" || curr->word == "testb")  byte[1] += 14 << 3;
	else if (curr->word == "shl"  || curr->word == "shlw"  || curr->word == "shlb" )  byte[1] += 15 << 3;
	else if (curr->word == "shr"  || curr->word == "shlw"  || curr->word == "shlb" )  byte[1] += 16 << 3;
	else if (curr->word == "push" || curr->word == "pushw" || curr->word == "pushb")  byte[1] += 17 << 3;
	else if (curr->word == "pop"  || curr->word == "popw"  || curr->word == "popb" )  byte[1] += 18 << 3;
	else if (curr->word == "jmp"  || curr->word == "jmpw"  || curr->word == "jmpb" )  byte[1] += 19 << 3;
	else if (curr->word == "jeq"  || curr->word == "jeqw"  || curr->word == "jeqb" )  byte[1] += 20 << 3;
	else if (curr->word == "jne"  || curr->word == "jnew"  || curr->word == "jneb" )  byte[1] += 21 << 3;
	else if (curr->word == "jgt"  || curr->word == "jgtw"  || curr->word == "jgtb" )  byte[1] += 22 << 3;
	else if (curr->word == "call" || curr->word == "callw" || curr->word == "callb")  byte[1] += 23 << 3;
	else if (curr->word == "ret")  { byte[1] += 24 << 3; insert_1B_in_CurrentSection_ByteList(convert_from_int_to_Byte(byte[1])); locationCounter += 1; return; }
	else if (curr->word == "iret") { byte[1] += 25 << 3; insert_1B_in_CurrentSection_ByteList(convert_from_int_to_Byte(byte[1])); locationCounter += 1; return; }

	curr = curr->nextWord;			// curr->word = dst

	int r = 0;
	//cout << "_______" << reg_indir_h_absolute_offset(curr->word) << "_______\n";
	
	if (absolute_immediate(curr->word))	// 20
	{
		byte[1] += 0;			// FirstPass message : "Error: the destination of instruction can not be immediate operand."
		//cout << "\n\n\n\n_7" << " " << "_\n\n\n\n";
		if (mnemonic == "push" || mnemonic == "pushb" || mnemonic == "pushw")
		{
			if (is_hex_number(curr->word))
			{
				string s = (curr->word).substr(2);
				int a = convert_to_int_from_hex_when_hex_is_positive(s);
				if (a >= 256)
				{
					_3_4_Byte = a;
					_3_4_ByteE = true;
				}
				else
				{
					dst_operand_1B_value = s;
					dest_operand_1B = true;
				}
				
			}
			else if (is_number(curr->word))
			{
				if (convert_to_int(curr->word) >= 256)
				{
					_3_4_Byte = convert_to_int(curr->word);
					_3_4_ByteE = true;
				}
				else
				{					//cout << "OVVVVVVVVVVDE\n" << convert_to_int(curr->word)<<"---\n";
					dst_operand_1B_value = convert_from_int_to_Byte(convert_to_int(curr->word));//cout << "OVVgfsgfsgfdsE\n" << dst_operand_1B_value<<"---\n";
					dest_operand_1B = true;
				}
			}
		}
	}
	else if (symbol_immediate(curr->word))	// &x
	{
		byte[1] += 0;			// FirstPass message : "Error: the destination of instruction can not be immediate operand."
		//cout << "\n\n\n\n_8" << " " << "_\n\n\n\n";
		if (mnemonic == "push" || mnemonic == "pushb" || mnemonic == "pushw")
		{
			string sName = (curr->word).substr(1);
			Symbol *s = symbolTable->getSymbol(sName);
			if (s == 0)
			{
				throw new SyntaxError(lineCounter, "Error in the second pass: the symbol of source of instruction is not defined.");
				return;
			}
			else if (s->isGlobal() == 1)
			{
				_3_4_ByteE = true;
				_3_4_Byte = 0;
				insert_in_CurrentSection_Ret(locationCounter + instructionSize - 2, absolute, '+', s->getID(), 2, s->getName());
			}
			else
			{	
				_3_4_ByteE  = true;
				_3_4_Byte = s->getValue();
				insert_in_CurrentSection_Ret(locationCounter + instructionSize - 2, absolute, '+', s->getSectionNo(), 2, s->getSectionName());
			}
		}		
	}
	else if (symbol_absolute(curr->word))	// x
	{
		if (w == 1) byte[1] += 1 << 2; byte_E[3] = true; byte_E[4] = true;		// size of the operand is 2B
		byte[2] += 5 << 5; 						// apsolutno adresiranje simbola je zapravo memorijsko direktno
		Symbol* s = symbolTable->getSymbol(curr->word);
		if (s == 0)
		{
			throw new SyntaxError(lineCounter, "Error in the second pass: the destination of instruction is memory direct symbol that does not exsit in symbol table.");
			return;
		}
		else if (s->isGlobal() == 1)
		{
			_3_4_ByteE = true;
			_3_4_Byte = 0;
			insert_in_CurrentSection_Ret(locationCounter + 2, absolute, '+', s->getID(), 2, s->getName());
		}
		else
		{	
			if (s->getSectionName() != currentSection->getName())
			{
				_3_4_ByteE = true;
				_3_4_Byte = s->getValue();
				insert_in_CurrentSection_Ret(locationCounter + 2, absolute, '+', s->getSectionNo(), 2, s->getSectionName());
			}
			else
			{
				_3_4_ByteE = true;
				_3_4_Byte = s->getValue();
				insert_in_CurrentSection_Ret(locationCounter + 2, absolute, '+', s->getSectionNo(), 2, s->getSectionName());					////////////////////////
			}
		}//cout << "\n\n\n\n_1" <<  " "<< "_\n\n\n\n";
	}
	else if (location_from_the_address(curr->word))	// *20
	{
		if (w == 1) byte[1] += 1 << 2;	// size of the operand is 2B
		byte[2] += 5 << 5;
		_3_4_ByteE = true;
		_3_4_Byte = convert_to_int(curr->word.substr(1));
		permutated = 1; //cout << "\n\n\n\n_2" << " " << "_\n\n\n\n";
	}
	else if (reg_dir(curr->word))					// r0..r5
	{
		if (w == 1) byte[1] += 1 << 2; 	// size of the operand
		byte[2] += 1 << 5;	// regdir
		//r = (curr->word)[1] - '0';
		r = return_num_of_registar_from_string(curr->word, 1);
		byte[2] += r << 1;	// number of registar
		//cout << "\n\n_3  " <<  mnemonic[mnemonic.length() - 1] << w<< "  _\n\n";
	}
	else if (reg_dir_h(curr->word))
	{
		byte[1] += 0 << 2; 	// size of the operand is 1B
		byte[2] += 1 << 5;	// regdir
		if (w == 0) byte[2] += 1;		// H
		r = return_num_of_registar_from_string(curr->word, 1);
		byte[2] += r << 1;	// number of registar
		//cout << "\n\n_4" << " " << "_\n\n";

	}
	else if (reg_dir_l(curr->word))
	{
		if (mnemonic[mnemonic.length() - 1] == 'w')  byte[1] += 1 << 2; 	// size of the operand
		byte[2] += 1 << 5;	// regdir
		byte[2] += 0;		// L
		r = return_num_of_registar_from_string(curr->word, 1);
		byte[2] += r << 1;	// number of registar
		//cout << "\n\n_5" << " " << "_\n\n";
	}
	else if (reg_indir(curr->word))	// [r2]
	{
		if (w == 1) byte[1] += 1 << 2;	// size of the operand is 2B
		byte[2] += 2 << 5;	// reg indir
		int r = return_num_of_registar_from_string(curr->word, 2);
		byte[2] += r << 1;
	}
	else if (reg_indir_h(curr->word))	// [r2h]
	{
		if (w == 1) byte[1] += 1 << 2; 	// size of the operand is 1B
		byte[2] += 2 << 5;	// reg indir
		int r = return_num_of_registar_from_string(curr->word, 2);
		byte[2] += r << 1;
		//byte[2] += 1;		// H
	}
	else if (reg_indir_l(curr->word))	// [r2l]
	{
		if (w == 1) byte[1] += 1 << 2;	// size of the operand is 1B
		byte[2] += 2 << 5;	// reg indir
		int r = return_num_of_registar_from_string(curr->word, 2);
		byte[2] += r << 1;
	}
	else if (reg_indir_absolute_offset(curr->word) || (reg_indir_h_absolute_offset(curr->word)) || (reg_indir_l_absolute_offset(curr->word)))	// r4[32]   r4h[32]
	{	//cout << "\n\n_5" << " " << "_\n\n";
		r = return_num_of_registar_from_string(curr->word, 1);
		byte[2] += r << 1;	// number of registar
		string c = "";
		if (reg_indir_absolute_offset(curr->word))
			c = (curr->word).substr(3, (curr->word).length() - 4);
		else
			c = (curr->word).substr(4, (curr->word).length() - 5);	
		//cout << "\n\n_*** " << c << " ***_\n\n";
		if (is_hex_number(c))
		{
			c = c.substr(2);//cout << "\n\n_*    " << c << "_\n\n";
			int a = convert_to_int_from_hex_when_hex_is_positive(c);
			if (a >= 256)
			{
				if (w == 1) byte[1] += 1 << 2; 	// size of the operand is 2B
				byte[2] += 4 << 5;	// reg indir 16b offset
				_3_4_Byte = convert_to_int_from_hex_when_hex_is_positive(c);
				_3_4_ByteE = true;
			}
			else
			{
				if (w == 1) byte[1] += 1 << 2; 	// size of the operand is 1B
				byte[2] += 3 << 5;	// reg indir 8b offset
				dst_operand_1B_value = c;
				dest_operand_1B = true;
			}
		}
		else if (is_number(c))
		{//cout << "\n\n\n\n_*" << " " << "_\n\n\n\n";
			int a = convert_to_int(c);
			if (a >= 256)
			{
				if (w == 1) byte[1] += 1 << 2;	// size of the operand is 2B
				byte[2] += 4 << 5;	// reg indir 16b offset
				_3_4_Byte = convert_to_int(c);
				_3_4_ByteE = true;
			}
			else
			{
				if (w == 1) byte[1] += 1 << 2;	// size of the operand is 1B
				byte[2] += 3 << 5;	// reg indir 8b offset
				int t = convert_to_int(c);	
				string tt = convert_from_int_to_Byte(t); 
				dst_operand_1B_value = tt;
				dest_operand_1B = true;
			}
		}
	}
	else if ((reg_indir_symbol_offset(curr->word)) || (reg_indir_h_symbol_offset(curr->word)) || (reg_indir_l_symbol_offset(curr->word)))	// r5[x] r5h[x] r5l[x]
	{
		if (w == 1) byte[1] += 1 << 2; 	// size of the operand is 2B
		byte[2] += 4 << 5;	// regdind 16b offset		
		r = return_num_of_registar_from_string(curr->word, 1);
		byte[2] += r << 1;	// number of registar
		if (reg_indir_h_symbol_offset(curr->word))	// H
			byte[2] += 1;
		string sName = "";
		if (reg_indir_symbol_offset(curr->word))
			sName = (curr->word).substr(3, (curr->word).length() - 4);
		else
			sName = (curr->word).substr(4, (curr->word).length() - 5);
		Symbol* s = symbolTable->getSymbol(sName);
		if (s == 0)
		{
			throw new SyntaxError(lineCounter, "Error in the second pass: the symbol of destination of instruction is not defined.");
			return;
		}
		else if (s->isGlobal() == 1)
		{
			if (r != 7)
			{
				_3_4_ByteE = true;
				_3_4_Byte = 0;
				insert_in_CurrentSection_Ret(locationCounter + 2, absolute, '+', s->getID(), 2, s->getName());
			}
			else		// PC_REL
			{
				pc_rel_dst = true; _3_4_ByteE = true; _3_4_Byte = 0; insert_in_CurrentSection_Ret(locationCounter + 2, relative, '+', s->getID(), 2, s->getName());
			}
		}
		else
		{	
			if (s->getSectionName() != currentSection->getName())
			{	if (r != 7)
				{	
					_3_4_ByteE = true;
					_3_4_Byte = s->getValue();
					insert_in_CurrentSection_Ret(locationCounter + 2, absolute, '+', s->getSectionNo(), 2, s->getSectionName());
				}
				else 
				{
					pc_rel_dst = true;
					_3_4_ByteE = true;
					_3_4_Byte = s->getValue();
					insert_in_CurrentSection_Ret(locationCounter + 2, relative, '+', s->getSectionNo(), 2, s->getSectionName());
				}
			}
			else
			{	if (r != 7)
				{
					_3_4_ByteE = true;
					_3_4_Byte = s->getValue();
					insert_in_CurrentSection_Ret(locationCounter + 2, absolute, '+', s->getSectionNo(), 2, s->getSectionName());				//////////////////////
				}
				else 
				{
					pc_rel_dst = true;
					_3_4_ByteE = true;
					_3_4_Byte = s->getValue();							// PCREL u odnosu na istu sekciju, nema potrebe
				}
			}
		}//cout << "\n\n\n\n_6" << " " << "_\n\n\n\n";
	}
	else if (pc_rel_offset(curr->word))			// $x
	{
		if (w == 1) byte[1] += 1 << 2; 	// size of the operand is 2B
		byte[2] += 4 << 5;		// regdind 16b offset
		byte[2] += 7 << 1;		// pc = r7	
		string sName = (curr->word).substr(1);
		Symbol *s = symbolTable->getSymbol(sName);
		if (s == 0)
		{
			throw new SyntaxError(lineCounter, "Error in the second pass: the symbol of destination of instruction is not defined.");
			return;
		}
		else if (s->isGlobal() == 1)
		{
			pc_rel_dst = true;
			_3_4_ByteE = true;
			_3_4_Byte = 0;
			insert_in_CurrentSection_Ret(locationCounter + 2, relative, '+', s->getID(), 2, s->getName());
		}
		else
		{	
			if (s->getSectionName() != currentSection->getName())
			{//cout << "\n\n\n\n_6 " << s->getName() << "_\n\n\n\n";
				pc_rel_dst = true;
				_3_4_ByteE = true;
				_3_4_Byte = s->getValue();
				insert_in_CurrentSection_Ret(locationCounter + 2, relative, '+', s->getSectionNo(), 2, s->getSectionName());
			}
			else
			{
				pc_rel_dst = true;
				_3_4_ByteE = true;
				_3_4_Byte = s->getValue();
			}
		}
	}
	string byteString[8];

	byteString[1] = convert_from_int_to_Byte(byte[1]);
	insert_1B_in_CurrentSection_ByteList(byteString[1]);

	byteString[2] = convert_from_int_to_Byte(byte[2]);
	insert_1B_in_CurrentSection_ByteList(byteString[2]);

	if (dest_operand_1B)
	{	insert_1B_in_CurrentSection_ByteList(dst_operand_1B_value);		}			//cout << "OVVVVVVVVVVDE" <<dst_operand_1B_value<< "\n";

	bool kraj = false;
	curr = curr->nextWord;
	if ((mnemonic == "int") || (mnemonic == "not")  || (mnemonic == "push")  || (mnemonic == "pop")  || (mnemonic == "jmp")
				|| (mnemonic == "jeq")  || (mnemonic == "jne")   || (mnemonic == "jgt")  || (mnemonic == "call")
				|| (mnemonic == "notb") || (mnemonic == "pushb") || (mnemonic == "popb") || (mnemonic == "jmpb")
				|| (mnemonic == "jeqb") || (mnemonic == "jneb")  || (mnemonic == "jgtb") || (mnemonic == "callb")
				|| (mnemonic == "notw") || (mnemonic == "pushw") || (mnemonic == "popw") || (mnemonic == "jmpw")
				|| (mnemonic == "jeqw") || (mnemonic == "jnew")  || (mnemonic == "jgtw") || (mnemonic == "callw")
				|| (mnemonic == "intb") || (mnemonic == "intw"))

		{	instructionSize = convert_to_int(curr->word);	kraj = true;}

	if (pc_rel_dst && _3_4_ByteE) 
	{ 
		if (!kraj)
		{
			Line* cc = curr;
			while (cc->nextWord) cc = cc->nextWord;
			instructionSize = convert_to_int(cc->word);
		}
		//cout << "--------------\n" << _3_4_Byte << " " << instructionSize << "\n---------------\n";
		_3_4_Byte = _3_4_Byte - instructionSize + 2;	//cout << "--------------\n" << _3_4_Byte << "\n---------------\n";
	}
	if (_3_4_ByteE == true)
	{					
		string thirdByte;
		string forthByte;
		string third_forth;

		if (_3_4_Byte >= 0)
		{
			if (permutated == 1)
			{
				thirdByte = convert_from_int_to_Byte(_3_4_Byte % 256);
				forthByte = convert_from_int_to_Byte(_3_4_Byte / 256);
			}
			else
			{
				thirdByte = convert_from_int_to_Byte(_3_4_Byte / 256);
				forthByte = convert_from_int_to_Byte(_3_4_Byte % 256);
			}
		}
		else
		{
			third_forth = convert_negative_to_hex(_3_4_Byte, 4);
			//cout << "&&&&&&&&&&&&&&&&&&&&&&&&&&& " << third_forth << "\n";
			if (permutated == 1)
			{	thirdByte = third_forth.substr(2, 2);
				forthByte = third_forth.substr(0, 2);
			}
			else
			{
				thirdByte = third_forth.substr(0, 2);
				forthByte = third_forth.substr(2, 2);
			}
		}
		insert_1B_in_CurrentSection_ByteList(thirdByte);
		insert_1B_in_CurrentSection_ByteList(forthByte);
		//locationCounter += 4;
	}
	else
	{
		//locationCounter += 2;
	}


	r = 0;

	if (kraj == true)
	{	locationCounter += instructionSize; return; }	///OVO DODATO
	int permutated2 = 1;
	bool pc_rel_src = false;

	int op2Descr = 0;
	int src_1B_2B_ = 0;		int src_1B_ = 0;
	bool src_1B_2B_E = false;	bool src_1B_E = false;
	bool src_operand_1B = false;	string src_operand_1B_value = "";		// int r2h[12]


	//cout << "_______" << curr->word << "_______\n";
	Line* t = curr; t = t->nextWord; instructionSize = convert_to_int(t->word);
	
	if (absolute_immediate(curr->word))	// 20
	{//cout << "\n\n\n\n_" << curr->word << "_\n\n\n\n";
		op2Descr = 0 << 5;
		if (is_hex_number(curr->word))
		{
			string s = (curr->word).substr(2);//cout << "\n\n\n\n_normal" << s << "_\n\n\n\n";
			int a = convert_to_int_from_hex_when_hex_is_positive(s);
			if (a >= 256)
			{
				src_1B_2B_ = a;
				src_1B_2B_E = true;
			}
			else
			{
				src_1B_ = a;
				src_1B_E = true;
			}
		}
		else if (is_number(curr->word))
		{
			if (convert_to_int(curr->word) >= 256)
			{
				src_1B_2B_ = convert_to_int(curr->word);
				src_1B_2B_E = true;
			}
			else
			{
				src_1B_ = convert_to_int(curr->word);
				src_1B_E = true;
			}
		}
	}
	else if (symbol_immediate(curr->word))	// &x
	{
		op2Descr = 0 << 5;	
		string sName = (curr->word).substr(1);
		Symbol *s = symbolTable->getSymbol(sName);
		if (s == 0)
		{
			throw new SyntaxError(lineCounter, "Error in the second pass: the symbol of source of instruction is not defined.");
			return;
		}
		else if (s->isGlobal() == 1)
		{
			pc_rel_src = true;
			src_1B_2B_E = true;
			src_1B_2B_ = 0;
			insert_in_CurrentSection_Ret(locationCounter + instructionSize - 2, absolute, '+', s->getID(), 2, s->getName());
		}
		else
		{	
			if (s->getSectionName() != currentSection->getName())
			{//cout << "\n\n\n\n_6 " << s->getName() << "_\n\n\n\n";
				pc_rel_src = true;
				src_1B_2B_E  = true;
				src_1B_2B_ = s->getValue();
				insert_in_CurrentSection_Ret(locationCounter + instructionSize - 2, absolute, '+', s->getSectionNo(), 2, s->getSectionName());
			}
			else
			{
				//pc_rel_dst = true;
				src_1B_2B_E  = true;
				src_1B_2B_ = s->getValue();
				insert_in_CurrentSection_Ret(locationCounter + instructionSize - 2, absolute, '+', s->getSectionNo(), 2, s->getSectionName());		//////////////////////////////////
			}
		}		

	}
	else if (symbol_absolute(curr->word))	// x
	{
		op2Descr += 5 << 5; 						// apsolutno adresiranje simbola je zapravo memorijsko direktno
		Symbol* s = symbolTable->getSymbol(curr->word);
		if (s == 0)
		{
			throw new SyntaxError(lineCounter, "Error in the second pass: the destination of instruction is memory direct symbol that does not exsit in symbol table.");
			return;
		}
		else if (s->isGlobal() == 1)
		{
			src_1B_2B_E = true;
			src_1B_2B_ = 0;
			insert_in_CurrentSection_Ret(locationCounter + instructionSize - 2, absolute, '+', s->getID(), 2, s->getName());
		}
		else
		{	
			if (s->getSectionName() != currentSection->getName())
			{
				src_1B_2B_E = true;
				src_1B_2B_ = s->getValue();
				insert_in_CurrentSection_Ret(locationCounter + instructionSize - 2, absolute, '+', s->getSectionNo(), 2, s->getSectionName());
			}
			else
			{
				src_1B_2B_E = true;
				src_1B_2B_ = s->getValue();
				insert_in_CurrentSection_Ret(locationCounter + instructionSize - 2, absolute, '+', s->getSectionNo(), 2, s->getSectionName());			//////////////////////////
			}
		}//cout << "\n\n\n\n_1" <<  " "<< "_\n\n\n\n";
	}
	else if (location_from_the_address(curr->word))	// *20
	{
		op2Descr += 5 << 5;
		src_1B_2B_E = true;
		src_1B_2B_ = convert_to_int(curr->word.substr(1));
		permutated = 0;//cout << "\n\n\n\n_2" << " " << "_\n\n\n\n";
	}
	else if (reg_dir(curr->word))					// r0..r5
	{
		op2Descr += 1 << 5;		// regdir
		r = (curr->word)[1] - '0';
		op2Descr += r << 1;	// number of registar
		//cout << "\n\n_3  " <<  mnemonic[mnemonic.length() - 1] << w<< "  _\n\n";
	}
	else if (reg_dir_h(curr->word))
	{
		op2Descr += 1 << 5;		// regdir
		r = (curr->word)[1] - '0';
		op2Descr += r << 1;		// number of registar
		op2Descr += 1;			// H bit

	}
	else if (reg_dir_l(curr->word))
	{
		op2Descr += 1 << 5;		// regdir
		r = (curr->word)[1] - '0';
		op2Descr += r << 1;	// number of registar
	}
	else if (reg_indir(curr->word))	// [r2]
	{
		op2Descr += 2 << 5;	// reg indir
		int r = return_num_of_registar_from_string(curr->word, 2);
		op2Descr += r << 1;
	}
	else if (reg_indir_h(curr->word))	// [r2h]
	{
		op2Descr += 2 << 5;	// reg indir
		int r = return_num_of_registar_from_string(curr->word, 2);
		op2Descr += r << 1;	
	}
	else if (reg_indir_l(curr->word))	// [r2l]
	{
		op2Descr += 2 << 5;	// reg indir
		int r = return_num_of_registar_from_string(curr->word, 2);
		op2Descr += r << 1;
	}
	else if (reg_indir_absolute_offset(curr->word) || (reg_indir_h_absolute_offset(curr->word)) || (reg_indir_l_absolute_offset(curr->word)))	// r4[32]   r4h[32]
	{	
		int r = return_num_of_registar_from_string(curr->word, 1);	//cout << "\n\n_5   " << r << "_\n\n";
		op2Descr += r << 1;
		string c = "";
		if (reg_indir_absolute_offset(curr->word))
			c = (curr->word).substr(3, (curr->word).length() - 4);
		else
			c = (curr->word).substr(4, (curr->word).length() - 5);	
		//cout << "\n\n_*** " << c << " ***_\n\n";
		if (is_hex_number(c))
		{
			c = c.substr(2);//cout << "\n\n_*    " << c << "_\n\n";
			int a = convert_to_int_from_hex_when_hex_is_positive(c);
			if (a >= 256)
			{
				op2Descr += 4 << 5;	// reg indir 16b offset
				src_1B_2B_ = a;
				src_1B_2B_E = true;
			}
			else
			{
				op2Descr += 3 << 5;	// reg indir 8b offset
				src_1B_ = a;
				src_1B_E = true;
			}
		}
		else if (is_number(c))
		{//cout << "\n\n\n\n_*" << " " << "_\n\n\n\n";
			int a = convert_to_int(c);
			if (a >= 256)
			{
				op2Descr += 4 << 5;	// reg indir 16b offset
				src_1B_2B_ = a;
				src_1B_2B_E = true;
			}
			else
			{
				op2Descr += 3 << 5;	// reg indir 8b offset 
				src_1B_ = a;
				src_1B_E = true;
			}
		}
	}
	else if ((reg_indir_symbol_offset(curr->word)) || (reg_indir_h_symbol_offset(curr->word)) || (reg_indir_l_symbol_offset(curr->word)))	// r5[x] r5h[x] r5l[x]
	{
		op2Descr += 4 << 5;	// regdind 16b offset		
		r = (curr->word)[1] - '0';
		byte[2] += r << 1;	// number of registar
		op2Descr += r << 1;
		//if (reg_indir_h_symbol_offset(curr->word))	// H
		//	byte[2] += 1;
		string sName = "";
		if (reg_indir_symbol_offset(curr->word))
			sName = (curr->word).substr(3, (curr->word).length() - 4);
		else
			sName = (curr->word).substr(4, (curr->word).length() - 5);
		Symbol* s = symbolTable->getSymbol(sName);
		if (s == 0)
		{
			throw new SyntaxError(lineCounter, "Error in the second pass: the symbol of destination of instruction is not defined.");
			return;
		}
		else if (s->isGlobal() == 1)
		{
			if (r != 7)
			{
				src_1B_2B_E = true;
				src_1B_2B_ = 0;
				insert_in_CurrentSection_Ret(locationCounter + instructionSize - 2, absolute, '+', s->getID(), 2, s->getName());
			}
			else 
			{
				pc_rel_src = true;
				src_1B_2B_E = true;
				src_1B_2B_ = -2;
				insert_in_CurrentSection_Ret(locationCounter + instructionSize - 2, relative, '+', s->getID(), 2, s->getName());
			}
		}
		else
		{	
			if (s->getSectionName() != currentSection->getName())
			{
				if (r != 7)
				{
					src_1B_2B_E = true;
					src_1B_2B_ = s->getValue();
					insert_in_CurrentSection_Ret(locationCounter + instructionSize - 2, absolute, '+', s->getSectionNo(), 2, s->getSectionName());
				}
				else 
				{
					pc_rel_src = true;
					src_1B_2B_E = true;
					src_1B_2B_ = s->getValue() - 2;
					insert_in_CurrentSection_Ret(locationCounter + instructionSize - 2, relative, '+', s->getSectionNo(), 2, s->getSectionName());
				}
			}
			else
			{
				if (r != 7)
				{
					src_1B_2B_E = true;
					src_1B_2B_ = s->getValue();				
					insert_in_CurrentSection_Ret(locationCounter + instructionSize - 2, absolute, '+', s->getSectionNo(), 2, s->getSectionName());			/////////////
				}
				else 
				{
					pc_rel_src = true;
					src_1B_2B_E = true;
					src_1B_2B_ = s->getValue() - 2;								// ista sekcija nema potrebe za pc_rel	
				}
			}
		}//cout << "\n\n\n\n_6" << " " << "_\n\n\n\n";
	}
	else if (pc_rel_offset(curr->word))			// $x
	{
		op2Descr += 4 << 5;		// regdind 16b offset
		op2Descr += 7 << 1;		// pc = r7	
		string sName = (curr->word).substr(1);
		Symbol *s = symbolTable->getSymbol(sName);
		if (s == 0)
		{
			throw new SyntaxError(lineCounter, "Error in the second pass: the symbol of destination of instruction is not defined.");
			return;
		}
		else if (s->isGlobal() == 1)
		{
			pc_rel_src = true;
			src_1B_2B_E = true;
			src_1B_2B_ = -2;
			insert_in_CurrentSection_Ret(locationCounter + instructionSize - 2, relative, '+', s->getID(), 2, s->getName());
		}
		else
		{	
			if (s->getSectionName() != currentSection->getName())
			{//cout << "\n\n\n\n_6 " << s->getName() << "_\n\n\n\n";
				pc_rel_src = true;
				src_1B_2B_E = true;
				src_1B_2B_ = s->getValue() - 2;
				insert_in_CurrentSection_Ret(locationCounter + instructionSize - 2, relative, '+', s->getSectionNo(), 2, s->getSectionName());
			}
			else
			{
				pc_rel_src = true;
				src_1B_2B_E = true;
				src_1B_2B_ = s->getValue() - 2;
			}
		}
	}


	/*int op2Descr = 0;
	int src_1B_2B_ = 0;		int src_1B_ = 0;
	bool src_1B_2B_E = false;	bool src_1B_E = false;
	bool src_operand_1B = false;	string src_operand_1B_value = "";*/		


	string op2DescrString = convert_from_int_to_Byte(op2Descr);
	insert_1B_in_CurrentSection_ByteList(op2DescrString);
	
	curr = curr->nextWord;
	
	instructionSize = convert_to_int(curr->word); //cout << "INSTRUCTION SIZE" << instructionSize << "\n";

	//if (pc_rel_dst && _3_4_ByteE) 
	//{ 
	//	//instructionSize = 4;
	//	_3_4_Byte = _3_4_Byte - instructionSize;
	//}

	//if (pc_rel_src && src_1B_2B_E) 
	//{ 
	//	src_1B_2B_ = src_1B_2B_ - instructionSize + 2;	
	//}
	if (src_1B_2B_E == true)
	{//cout << "\n\n\n\n_ubbaci" << src_1B_2B_ << "_\n\n\n\n";
		string src_1_Byte;
		string src_2_Byte;
		string src_1_2_Byte;

		if (src_1B_2B_ >= 0)
		{
			if (permutated == 1)
			{
				src_1_Byte = convert_from_int_to_Byte(src_1B_2B_ % 256);
				src_2_Byte = convert_from_int_to_Byte(src_1B_2B_ / 256);
			}
			else
			{
				src_1_Byte = convert_from_int_to_Byte(src_1B_2B_ / 256);
				src_2_Byte = convert_from_int_to_Byte(src_1B_2B_ % 256);
			}
		}
		else
		{
			src_1_2_Byte = convert_negative_to_hex(src_1B_2B_, 4);
			//cout << "&&&&&&&&&&&&&&&&&&&&&&&&&&& " << src_1_2_Byte << "\n";
			if (permutated == 1)
			{	src_1_Byte = src_1_2_Byte.substr(2, 2);
				src_2_Byte = src_1_2_Byte.substr(0, 2);
			}
			else
			{
				src_1_Byte = src_1_2_Byte.substr(0, 2);
				src_2_Byte = src_1_2_Byte.substr(2, 2);
			}
		}
		insert_1B_in_CurrentSection_ByteList(src_1_Byte);
		insert_1B_in_CurrentSection_ByteList(src_2_Byte);
	}
	else if (src_1B_E == true)
	{
		string src = convert_from_int_to_Byte(src_1B_);
		insert_1B_in_CurrentSection_ByteList(src);
	}
	else
	{
	
	}

	locationCounter += instructionSize;
	
}

void SecondPass::insert_Byte_in_ByteList(string Byte, ByteList** head, ByteList** tail)
{
	ByteList* newElem = new ByteList(Byte);
	if (*head == 0)
		*head = newElem;
	else
		(*tail)->next = newElem;
	*tail = newElem;
}

void SecondPass::insert_Byte_in_ByteList(string Byte, string sectionName)
{
	ByteList* newElem = new ByteList(Byte);
	list<C>::iterator it;
	for (it = sectionC.begin(); it != sectionC.end() && it->sectionName != sectionName; it++);
	if (it != sectionC.end())   	
	{	
		if (it->byte_List_Head == 0)
			it->byte_List_Head = newElem;
		else
			it->byte_List_Tail->next = newElem;
		it->byte_List_Tail = newElem;
	}
}
ofstream& SecondPass::print_ByteList(string sectionName, ByteList** head, ofstream& myOfstream)
{
	myOfstream << "\n#" << sectionName << "\n";
	ByteList* curr = *head;
	int i = 0;
	while (curr != 0)
	{
		myOfstream << curr->byte << " ";
		if (++i == 12)
		{
			myOfstream << "\n";
			i = 0;
		}
		curr = curr->next;
	}
	myOfstream << "\n";
	return myOfstream;
}

ofstream& SecondPass::print_ByteList(string sectionName, ofstream& myOfstream)
{
	myOfstream << "\n#" << sectionName << "\n";
	list<C>::iterator it;
	for (it = sectionC.begin(); it != sectionC.end() && it->sectionName != sectionName; it++);
	if (it != sectionC.end())   	
	{
		ByteList* curr = it->byte_List_Head;	
		int i = 0;
		while (curr != 0)
		{	
			myOfstream << curr->byte << " ";
			if (++i == 12)
			{
				myOfstream << "\n";
				i = 0;
			}
			curr = curr->next;
		}
		myOfstream << "\n";
	}
	return myOfstream;
}

void SecondPass::insert_in_Ret(int offset, R_type r_type, char sign, int value, Ret** head, Ret** tail, int numOfBytes, string reference)
{
	Ret* newElem = new Ret(offset, r_type, sign, value, numOfBytes, reference);
	if (*head == 0)
		*head = newElem;
	else
		(*tail)->next = newElem;
	*tail = newElem;
}

void SecondPass::insert_in_Ret(int offset, R_type r_type, char sign, int value, int numOfBytes, string sectionName, string reference)
{
	Ret* newElem = new Ret(offset, r_type, sign, value, numOfBytes, reference);
	list<C>::iterator it;
	for (it = sectionC.begin(); it != sectionC.end() && it->sectionName != sectionName; it++);
	if (it != sectionC.end())   	
	{
		if (it->ret_Head == 0)
			it->ret_Head = newElem;
		else
			it->ret_Tail->next = newElem;
		it->ret_Tail = newElem;
	}
}

ofstream& SecondPass::print_Ret(string sectionName, Ret* head, ofstream& myOfstream)
{
	myOfstream << "\n#.ret" << sectionName << "\n";
	myOfstream << "Offset             " << "Type           " << "Sign   " << "Value                                    Reference\n";
	Ret* curr = head;
	while (curr != 0)
	{
		myOfstream << curr->offset;
		for(int i = 0; i < (19 - (signed)(number_of_digits(curr->offset))); i++) myOfstream << " ";
		if (curr->r_type == 0 )
			myOfstream << "R_386_32       ";
		else if (curr->r_type == 1 )
			myOfstream << "R_386_32_PC32  ";
		myOfstream << curr->sign;
		for(int i = 0; i < (11 - (signed)(number_of_digits(curr->value))); i++) myOfstream << " ";
		myOfstream << curr->value;
		for(int i = 0; i < (45 - (signed)(curr->reference.length())); i++) myOfstream << " ";
		myOfstream << curr->reference << "\n";
		curr = curr->next;
	}
	myOfstream << "\n";
	return myOfstream;
}

ofstream& SecondPass::print_Ret(string sectionName, ofstream& myOfstream)
{
	if (sectionName == ".bss") return myOfstream;
	myOfstream << "\n#.ret." << sectionName << "\n";
	myOfstream << "Offset             " << "Type           " << "Sign   " << "Value                                    Reference\n";
	list<C>::iterator it;
	for (it = sectionC.begin(); it != sectionC.end() && it->sectionName != sectionName; it++)
	if (it == sectionC.end())  
		return myOfstream; 
	Ret* curr = it->ret_Head;	
	while (curr != 0)
	{
		myOfstream << curr->offset;
		for(int i = 0; i < (19 - (signed)(number_of_digits(curr->offset))); i++) myOfstream << " ";
		if (curr->r_type == 0 )
			myOfstream << "R_386_32       ";
		else if (curr->r_type == 1 )
			myOfstream << "R_386_32_PC32  ";
		myOfstream << curr->sign;
		for(int i = 0; i < (11 - (signed)(number_of_digits(curr->value))); i++) myOfstream << " ";
		myOfstream << curr->value;
		for(int i = 0; i < (45 - (signed)(curr->reference.length())); i++) myOfstream << " ";
		myOfstream << curr->reference << "\n";
		curr = curr->next;
	}
	myOfstream << "\n";
	return myOfstream;
}

void SecondPass::insert_1B_in_CurrentSection_ByteList(string Byte)
{
	if (currentSection->getName() == ".text")
		insert_Byte_in_ByteList(Byte, &text_Head, &text_Tail);
	else if (currentSection->getName() == ".data")
		insert_Byte_in_ByteList(Byte, &data_Head, &data_Tail);
	else 
		insert_Byte_in_ByteList(Byte, currentSection->getName());
}

void SecondPass::insert_int_to_4B_in_CurrentSection_ByteList(int i)
{
	if (i < 0)
	{
		string s = convert_negative_to_hex_permutated(i, 8);
	//	insert_1B_in_CurrentSection_ByteList(s.substr(2, 3) + s.substr(3, 4));
		insert_1B_in_CurrentSection_ByteList(s.substr(1, 1) + s.substr(0, 1));
		insert_1B_in_CurrentSection_ByteList(s.substr(3, 1) + s.substr(2, 1));
		insert_1B_in_CurrentSection_ByteList(s.substr(5, 1) + s.substr(4, 1));
		insert_1B_in_CurrentSection_ByteList(s.substr(7, 1) + s.substr(6, 1));
		//insert_1B_in_CurrentSection_ByteList(s.substr(0, 2));
		return;
	}
	string word[8];
	for (int k = 0; k < 8; k++)
	{
		word[k] = nibble_to_16(i % 16);
		i = i / 16;
	}
	insert_1B_in_CurrentSection_ByteList(word[1] + word[0]);
	insert_1B_in_CurrentSection_ByteList(word[3] + word[2]);
	insert_1B_in_CurrentSection_ByteList(word[5] + word[4]);
	insert_1B_in_CurrentSection_ByteList(word[7] + word[6]);
}

void SecondPass::insert_int_to_2B_in_CurrentSection_ByteList(int i)
{
	if (i < 0)
	{
		string s = convert_negative_to_hex_permutated(i, 4);
		insert_1B_in_CurrentSection_ByteList(s.substr(1, 1) + s.substr(0, 1));
		insert_1B_in_CurrentSection_ByteList(s.substr(3, 1) + s.substr(2, 1));
		return;
	}
	string word[4];
	for (int k = 0; k < 4; k++)
	{
		word[k] = nibble_to_16(i % 16);
		i = i / 16;
	}
	insert_1B_in_CurrentSection_ByteList(word[1] + word[0]);
	insert_1B_in_CurrentSection_ByteList(word[3] + word[2]);
}

void SecondPass::insert_int_to_1B_in_CurrentSection_ByteList(int i)
{
	if (i < 0)
	{
		string s = convert_negative_to_hex_permutated(i, 2);
		insert_1B_in_CurrentSection_ByteList(s.substr(1, 1) + s.substr(0, 1));
		return;
	}
	string word[2];
	for (int k = 0; k < 2; k++)
	{
		word[k] = nibble_to_16(i % 16);
		i = i / 16;
	}
	insert_1B_in_CurrentSection_ByteList(word[1] + word[0]);
}


void SecondPass::insert_in_CurrentSection_Ret(int offset, R_type r_type, char sign, int value, int numOfBytes, string reference)
{
	if (currentSection->getName() == ".text")
		insert_in_Ret(offset, r_type, sign, value, &ret_text_Head, &ret_text_Tail, numOfBytes, reference);
	else if (currentSection->getName() == ".data")
		insert_in_Ret(offset, r_type, sign, value, &ret_data_Head, &ret_data_Tail, numOfBytes, reference);
	else 
		insert_in_Ret(offset, r_type, sign, value, numOfBytes, currentSection->getName(), reference);
}

void SecondPass::insert_N_emptyBytes_in_CurrentSection_ByteList(int N)
{
	int i;
	for (i = 0; i < N; i++)
	{
		if (currentSection->getName() == ".text")
			insert_Byte_in_ByteList("00", &text_Head, &text_Tail);
		else if (currentSection->getName() == ".data")
			insert_Byte_in_ByteList("00", &data_Head, &data_Tail);
		else
			insert_Byte_in_ByteList("00", currentSection->getName());
	}
}
