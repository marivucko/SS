/*
 * FirstPass.cpp
 *
 *  Created on: Feb 20, 2019
 *      Author: user
 */

#include "FirstPass.h"
#include "AuxiliaryClass.h"
#include "SecondPass.h"
#include <iostream>
#include <fstream>
#include <sstream>
#include <string>


using namespace std;


int TNS::glob_TNS_OrdinalNumber = 0;

FirstPass::FirstPass(string inputFileName, string outputFileName, int startAddress)
{
	this->inputFileName = inputFileName;
	this->outputFileName = outputFileName;

	myOfstream.open(this->outputFileName.c_str());

	locationCounter = 0;
	lineCounter = 1;

	end_Of_FirstPass = false;
	end_of_line = false;

	undefiendSection = new Section("UND", 0, false, 0, 0, 0, 0);
	currentSection = undefiendSection;
	//currentSection = new Section("TXT", 1, 0, false, 1);

	numOfSections = 1;


	my_TNS_head = 0;
	my_TNS_tail = 0;
	//my_TNS_counter = 0;

	myLineListHead = 0;
	myLineListTail = 0;

	roDataLabelsHead = 0;
	roDataLabelsTail = 0;

	startSectionsAddress = startAddress;

	symbolTable = new SymbolTable();
}

FirstPass::~FirstPass()
{
	delete symbolTable;
}

void FirstPass::firstPass()
{
	string line = "";

	ifstream myfile(inputFileName.c_str());

	while (getline(myfile, line) && !end_Of_FirstPass)
	{
		while (!end_of_line)
		{
			if (just_white_spaces(line))
			{
				cout << "prazna linija\n";
				end_of_line = true;
			}
			//else if (line.compare("\n"))
			//{
			//	cout << "PRAZNA LINIJA\n";
			//	end_of_line = true;
			//}
			//else if (line.compare(".rodata\n"))
			//{
			//	cout << "RODATA\n";
			//	end_of_line = true;
			//}
			else
			{
				line = line.substr(0, line.length() - 1);		// delete '\n' at the end of the line 
				line = delete_white_spaces_at_the_begging(line);
				line = delete_white_spaces_at_the_end(line);
				//line = insert_white_spaces_between_operations(line);	// ZA EQU SAMO!!!

				int n;
				Line* words = 0;

				get_words_between_white_spaces(line, &n, &words);

				Line* currWord = words;


				string firstWord = currWord->word;
			//	if (!isLabel(firstWord))
				//	insertLineToLineList(words);				// there is no need to execute labels in the second pass

				currWord = currWord->nextWord;
				string s = ".eNd";

				cout << "line " << lineCounter << " " << line << n << "\n";

				try
				{
					if (firstWord == ".end")
					{
						end_Of_FirstPass = true;
						end_of_line = true;
						currentSection->setSize(locationCounter);
					}
					else if (firstWord == ".global")
					{
						executeGlobal(&currWord, ".global");
						end_of_line = true;
					}
					else if (firstWord == ".extern")
					{
						executeExtern(&currWord, ".extern");
						end_of_line = true;
					}
					else if (isEQU(firstWord))		// .equ a,b
					{

						string line1 = line.substr(line.find(',') + 1);
						line1 = delete_white_spaces_at_the_begging(line1);
						if (line1[0] == '-')
							line1 = "0 " + line1;
						//cout << "|||||||||||\n" << line1 << "\n|||||||||||\n";
						line1 = insert_white_spaces_between_operations(line1);
						line1 = delete_white_spaces_at_the_begging(line1);
						line1 = delete_white_spaces_at_the_end(line1);
						line1 = insert_white_spaces_between_operations(line1);
						line1 = delete_white_spaces_at_the_end(line1);;


						get_words_between_white_spaces(line1, &n, &words);
						Line* currWord = words;
						string firstWord = line.substr(5, line.find(',') - 5);

						executeEQU(firstWord, &currWord, n);	// *****firsWord = a, currWord = {b}   .equ a,b

						end_of_line = true;
					}
					else if (isLabel(firstWord))	// labela
					{
						executeLabel(firstWord, &line);
						//cout << "222 " << line << "\n"; 
						end_of_line = false;
					}
					else if (isSection(firstWord, &currWord))
					{
						int r = 1; int w = 0; int e = 0;
						string ss = "";
						Line* curr = currWord;
						if (curr != 0)	curr = curr->nextWord;
						//if (curr != 0) { cout << "\n=========\n" << curr->word << "\n=========\n"; currWord=currWord->nextWord; }
						while (curr != 0) 
						{ 
							ss = ss + curr->word;
							curr = curr->nextWord;
						}
						if (ss.find('r') < ss.length()) { r = 1; }
						if (ss.find('w') < ss.length()) { w = 1; }
						if (ss.find('e') < ss.length()) { e = 1; }

						executeSection(firstWord, &currWord, r, w, e);
						end_of_line = true;
			
					}
					else if (isInstruction(firstWord))
					{
						if (currentSection->getName() == ".data")
						{
							//throw new SyntaxError(lineCounter, "Error: it is forbidden to execute instruction in .data section.");
							//end_of_line = true;
						}
						string l1 = insert_white_spaces_between_operation(line, ",");
						int k = l1.find(",");
						if (k < l1.length())
						{
							l1 = l1.substr(0, k) + l1.substr(k + 1);	
						}
						//cout << "\n\n======\n"<<l1<<"=====\n";
						//cout << "\n\n======\n"<<line<<"=====\n";
						int n1; Line* currWord1;
						l1 = delete_white_spaces_at_the_end(l1);
						get_words_between_white_spaces(l1, &n1, &currWord1);
						string firstWord1 = currWord1->word;
						currWord1 = currWord1->nextWord;
						//cout << "\n\n======\n"<<firstWord1<<"=====\n";
						executeInstruction(firstWord1, &currWord1);
						end_of_line = true;
						//cout << "LOCATION COUNTER " << locationCounter << "\n";
					}
					else if (is_Byte_Word_Align_Skip(firstWord))
					{
						string line2 = line.substr(firstWord.length(), line.length() - firstWord.length());
						line2 = delete_all_white_spaces_between_words(line2);
						//cout << "_" << line2 << "_\n";
						get_words_between_commas(line2, &n, &words);
						Line* currWord = words;
						execute_Byte_Word_Align_Skip(firstWord, &currWord, &line);
						end_of_line = true;
					}
					else
					{
						throw new SyntaxError(lineCounter, "Error: line is not recognized as any directive, instruction, section.");
						end_of_line = true;
					}
				} catch (SyntaxError* s) { end_of_line = true; cout << "Syntax error at line " << s->getLineNo() << ". "<< s->getError() << "\n"; }



			}
		}
		if (end_of_line)
			lineCounter++;
		end_of_line = false;

	}

	try {
	if (end_Of_FirstPass == false)
		throw new SyntaxError(lineCounter, "Error: directive .end is missing.");
	} catch (SyntaxError* s) { end_of_line = true; cout << "Syntax error at line " << s->getLineNo() << ". "<< s->getError() << "\n"; }

	//symbolTable->printSymbols(myOfstream);
	try
	{
		calculate_TNS();
	} catch (SyntaxError* s) { end_of_line = true; cout << "Syntax error at line " << s->getLineNo() << ". "<< s->getError() << "\n"; }

	symbolTable->printSymbols(myOfstream);
	cout << "\n\n";
	symbolTable->printSections(myOfstream);
	printLineList();
	print_RoDataLabels();

	SecondPass* sp = new SecondPass(myLineListHead, numOfSections, symbolTable, outputFileName, startSectionsAddress);
	sp->secondPass(myOfstream);

	myOfstream.close();
}

bool FirstPass::isEnd(string s)
{
	return false;
}

bool FirstPass::isInstruction(string firstWord)
{
	if (firstWord == "halt") return true;	
	if (firstWord == "sub"  || firstWord == "subw"  || firstWord == "subb" )  return true;
	if (firstWord == "xchg" || firstWord == "xchgw" || firstWord == "xchgb")  return true;	
	if (firstWord == "mul"  || firstWord == "mulw"  || firstWord == "mulb" )  return true;
	if (firstWord == "int"  || firstWord == "intw"  || firstWord == "intb" )  return true;	
	if (firstWord == "div"  || firstWord == "divw"  || firstWord == "divb" )  return true;
	if (firstWord == "mov"  || firstWord == "movw"  || firstWord == "movb" )  return true;	
	if (firstWord == "cmp"  || firstWord == "cmpw"  || firstWord == "cmpb" )  return true;
	if (firstWord == "add"  || firstWord == "addw"  || firstWord == "addb" )  return true;	
	if (firstWord == "not"  || firstWord == "notw"  || firstWord == "notb" )  return true;

	if (firstWord == "and"  || firstWord == "andw"  || firstWord == "andb" )  return true;	
	if (firstWord == "shr"  || firstWord == "shrw"  || firstWord == "shrb" )  return true;
	if (firstWord == "or"   || firstWord == "orw"   || firstWord == "orb"  )  return true;	
	if (firstWord == "push" || firstWord == "pushw" || firstWord == "pushb")  return true;
	if (firstWord == "xor"  || firstWord == "xorw"  || firstWord == "xorb" )  return true;	
	if (firstWord == "pop"  || firstWord == "popw"  || firstWord == "popb" )  return true;
	if (firstWord == "test" || firstWord == "testw" || firstWord == "testb")  return true;	
	if (firstWord == "jmp"  || firstWord == "jmpw"  || firstWord == "jmpb" )  return true;
	if (firstWord == "shl"  || firstWord == "shlw"  || firstWord == "shlb" )  return true;	
	if (firstWord == "jeq"  || firstWord == "jeqw"  || firstWord == "jeqb" )  return true;

	if (firstWord == "jne"  || firstWord == "jnew"  || firstWord == "jneb" )  return true;
	if (firstWord == "jgt"  || firstWord == "jgtw"  || firstWord == "jgtb" )  return true;
	if (firstWord == "call" || firstWord == "callw" || firstWord == "callb")  return true;
	if (firstWord == "ret") return true;
	if (firstWord == "iret") return true;
}

void FirstPass::executeInstruction(string mnemonic, Line** currWords)
{
	Line** currWord = currWords;
	Line* lineToBeAdded = new Line(mnemonic);
	if (currentSection->getE() == 0)
	{
		//cout << "\n"<<currentSection->getE()<<"\n";
		throw new SyntaxError(lineCounter, "Error: it is forbidden to execute instructions in " + currentSection->getName() +" section because E bit is 0.");
		return;
	}
	if ((mnemonic == "halt") || (mnemonic == "ret") || (mnemonic == "iret"))						
	{
		insertLineToLineList(lineToBeAdded, lineCounter);
		locationCounter += 1;
		return;
	}
	bool b = (mnemonic[mnemonic.length() - 1 ] == 'b' && mnemonic != "sub");		// size of operand is 1B										
	string dst = (*currWord)->word; Line* dstLine = new Line(dst);				// currWord = dst
	add_words_at_the_end_of_first_line(&lineToBeAdded, &dstLine);
	//cout << "_" << dst << "_\n";
	int locationCounterIncrement = 2;
	if (absolute_immediate(dst) || symbol_immediate(dst))
	{
		if ((mnemonic != "push") && (mnemonic != "pushb") && (mnemonic != "pushw"))
		{
			throw new SyntaxError(lineCounter, "Error: the destination of instruction can not be immediate operand.");
			return;
		}
		else
		{
			if (symbol_immediate(dst))
				locationCounterIncrement += 2;
			else if (is_hex_number(dst))
			{
				string c = dst.substr(2);
				int a = convert_to_int_from_hex_when_hex_is_positive(c);
				if (a >= 256) 
					locationCounterIncrement += 2;
				else
					locationCounterIncrement += 1;
			}
			else if (is_number(dst))
			{
				int a = convert_to_int(dst);
				if (a >= 256)
					locationCounterIncrement += 2;
				else
					locationCounterIncrement += 1;
			}
		}
	}
	else if ((reg_indir_h_absolute_offset(dst)) || (reg_indir_l_absolute_offset(dst)) || (reg_indir_absolute_offset(dst)))	// r4[32]  r4[0x32]    r4h[32]  r4h[0x32]
	{
		string c = "";
		if (reg_indir_absolute_offset(dst))
			c = (dst).substr(3, (dst).length() - 4);
		else
			c = (dst).substr(4, (dst).length() - 5);	
		if (is_hex_number(c))
		{
			c = c.substr(2);//cout << "\n\n_*    " << c << "_\n\n";
			int a = convert_to_int_from_hex_when_hex_is_positive(c);
			if (a >= 256) 
				locationCounterIncrement += 2;
			else
				locationCounterIncrement += 1;
		}
		else if (is_number(c))
		{
			int a = convert_to_int(c);
			if (a >= 256)
				locationCounterIncrement += 2;
			else
				locationCounterIncrement += 1;
			
		}
	}
	else if ((symbol_absolute(dst)) || (location_from_the_address(dst)) || (reg_indir_symbol_offset(dst))     || (pc_rel_offset(dst))
				   || (reg_indir_h_symbol_offset(dst)) || (reg_indir_l_symbol_offset(dst)))
	{
		//if (b)
		//	locationCounterIncrement += 1;
		//else
			locationCounterIncrement += 2;
	}
	else if (reg_indir(dst))						// [r4]
	{
		
	}
	else if (reg_indir_h(dst) || reg_indir_l(dst))				// [r4h]   [r4l]
	{
		
	}
	else if (reg_dir(dst) || reg_dir_h(dst) || reg_dir_l(dst))		// r3   r3h   r3l
	{
	}
	else
	{
		throw new SyntaxError(lineCounter, "Error: the destination operand does not have appropriate form.");
		return;
	}
	*currWord = (*currWord)->nextWord;											// currWord = src
	if ((mnemonic == "int") || (mnemonic == "not")  || (mnemonic == "push")  || (mnemonic == "pop")  || (mnemonic == "jmp")
				|| (mnemonic == "jeq")  || (mnemonic == "jne")   || (mnemonic == "jgt")  || (mnemonic == "call")
				|| (mnemonic == "notb") || (mnemonic == "pushb") || (mnemonic == "popb") || (mnemonic == "jmpb")
				|| (mnemonic == "jeqb") || (mnemonic == "jneb")  || (mnemonic == "jgtb") || (mnemonic == "callb")
				|| (mnemonic == "notw") || (mnemonic == "pushw") || (mnemonic == "popw") || (mnemonic == "jmpw")
				|| (mnemonic == "jeqw") || (mnemonic == "jnew")  || (mnemonic == "jgtw") || (mnemonic == "callw")
				|| (mnemonic == "intb") || (mnemonic == "intw"))
	{
		if (*currWord != 0)
		{//cout << "******\n***********\n"<<(*currWord)->word<<"**********\n**********\n";
			throw new SyntaxError(lineCounter, "Error: the instruction should have just one argument.");
			return;
		}
		else
		{
			string lci = convert_to_string(locationCounterIncrement); Line* isize = new Line(lci);				
			add_words_at_the_end_of_first_line(&lineToBeAdded, &isize);
			insertLineToLineList(lineToBeAdded, lineCounter);
			locationCounter += locationCounterIncrement;
			return;
		}
	}
	string src = (*currWord)->word; Line* srcLine = new Line(src);
	add_words_at_the_end_of_first_line(&lineToBeAdded, &srcLine);
	locationCounterIncrement += 1;

	if (absolute_immediate(src))
	{//cout << "AAAA\n" << src<<"BBB\n";
		if (is_hex_number(src))
		{
			src = src.substr(2);//cout << "\n\n_*    " << src << "_\n\n";
			int a = convert_to_int_from_hex_when_hex_is_positive(src);
			if (a >= 256) 
				locationCounterIncrement += 2;
			else
				locationCounterIncrement += 1;
		}
		else if (is_number(src))
		{
			int a = convert_to_int(src);
			if (a >= 256)
				locationCounterIncrement += 2;
			else
				locationCounterIncrement += 1;
			
		}
	}
	else if (symbol_immediate(src) || location_from_the_address(src) || pc_rel_offset(src) || symbol_absolute(src)
				       || reg_indir_symbol_offset(src) || reg_indir_h_symbol_offset(src) || reg_indir_l_symbol_offset(src))
	{
		locationCounterIncrement += 2;
	}
	else if ((reg_indir_absolute_offset(src))|| (reg_indir_h_absolute_offset(src)) || (reg_indir_l_absolute_offset(src)))
	{//cout << "^^^^^^^^^^\n" << "lalal" << "^^^^^^^^^^^^\n";
		string c = "";
		if (reg_indir_absolute_offset(src))
			c = src.substr(3, src.length() - 4);
		else
			c = src.substr(4, src.length() - 5);	
		if (is_hex_number(c))
		{
			c = c.substr(2);
			int a = convert_to_int_from_hex_when_hex_is_positive(c);	//cout << "^^^^^^^^^^\n" << a << "^^^^^^^^^^^^\n";
			if (a >= 256)
			{
				locationCounterIncrement += 2;
			}
			else
			{
				locationCounterIncrement += 1;
			}
		}
		else if (is_number(c))
		{
			int a = convert_to_int(c);
			if (a >= 256)
			{
				locationCounterIncrement += 2;
			}
			else
			{
				locationCounterIncrement += 1;
			}
		}
	}
	else if (reg_indir(src) || reg_indir_h(src) || reg_indir_l(src) || reg_dir(src) || reg_dir_h(src) || reg_dir_l(src))
	{
	}
	else
	{
		throw new SyntaxError(lineCounter, "Error: the source operand does not have appropriate form.");
		return;
	}

	*currWord = (*currWord)->nextWord;
	//if ((*currWord) != 0)
	//{
		//throw new SyntaxError(lineCounter, "Error: instruction has too many words.");
		//return;
	//}

	locationCounter += locationCounterIncrement;

	string lcii = convert_to_string(locationCounterIncrement); Line* iisize = new Line(lcii);				
			add_words_at_the_end_of_first_line(&lineToBeAdded, &iisize);
	insertLineToLineList(lineToBeAdded, lineCounter);
}



void FirstPass::executeGlobal(Line** currWord, string glob_extern)
{
	Line* line = 0;
	if (*currWord == 0)
	{
		throw new SyntaxError(lineCounter, "Error: there are no symbols after declaration.");
		return;
	}

	Line* prevWord = 0;

	while (*currWord != 0)
	{
		string s  = (*currWord)->word;

		unsigned int i = s.find_first_of(",");
		if ((i < s.length()) && (s != ","))
		{
			Line* newWords = 0;
			int n;
			prevWord = *currWord;
			*currWord = (*currWord)->nextWord;							// delete a,b,c from currWord and add it to the list as 3 elements (a, b, c)

			if (check_if_two_commas_are_not_next_to_each_other(s)) throw new SyntaxError(lineCounter, "Error: there are 2 or more commas next to each other.");

			delete_all_white_spaces_between_words(s);
			get_words_between_commas(s, &n, &newWords);
			add_words_at_the_beggining_of_first_line(currWord, &newWords);

			Line* m = 0;
			get_words_between_commas(s, &n, &m);
			Line* c = m;
			while (c != 0)
			{
				if (c->word != ",")
				{
					Line* b = c;
					b->nextWord = 0;
					add_words_at_the_end_of_first_line(&line, &b);
				}
				c = c->nextWord;
			}

			if ((*currWord) != 0)
				s  = (*currWord)->word;

			Line* curr = *currWord;
			while (curr != 0)
			{
				//cout << curr->word << "$";
				curr = curr->nextWord;
			}
			//cout << "\n";
		}
		else
		{
			Line* m = new Line(s);
			if (s != ",")
				add_words_at_the_end_of_first_line(&line, &m);
		}


		if ((prevWord != 0) && (prevWord->word == ",") && (s == ","))
		{
			throw new SyntaxError(lineCounter, "Error: there are 2 or more commas next to each other.");
		}
		if (s != ",")
		{
			if (is_variable_correct(s) == false) throw new SyntaxError(lineCounter, s+ " " + (*currWord)->word + "Error: variable does not have appropriate form - must start with a letter, and contain only letters and digits.");
		}
		if (((*currWord)->nextWord == 0) && (s == ","))
		{
			throw new SyntaxError(lineCounter, "Error: missing variable after last comma.");
		}

		if (s != ",")
		{
			Symbol* sym = symbolTable->getSymbol(s);
			if (sym == 0)
			{
				symbolTable->insertSymbol(new Symbol(s, 0, undefiendSection, 0, true, false));
			}
			else
			{
				if (sym->isGlobal() == true)
				{
					throw new SyntaxError(lineCounter, "Error: symbol " + sym->getName() + " already defined as global.");
				}
				else
				{
					sym->setGlobal(true);
				}
			}
		}

		prevWord = *currWord;
		*currWord = (*currWord)->nextWord;
	}

	Line* m = new Line(glob_extern);
	add_words_at_the_beggining_of_first_line(&line, &m);
	insertLineToLineList(line, lineCounter);
	Line* c = line;
	while (c != 0)
	{
		//cout << c->word << "%";
		c = c->nextWord;
	}
	//cout << "\n";

}

void FirstPass::executeExtern(Line** words, string glob_extern)
{
	executeGlobal(words, glob_extern);
}

bool FirstPass::isLabel(string s)
{
	//cout << "LABELLABEL\n";
	unsigned int i = s.find_first_of(":");
	if (i < s.length())
	{
		string s1 = s.substr(0, i);
		//cout << "LABEL " << s1 << "\n";
		if (is_variable_correct(s1))
			return true;
		else
			return false;
	}
	else
	{
		return false;
	}
}

void FirstPass::separate_label_from_the_rest_of_line(string s, string* label, string* rest_of_line)
{
	unsigned int i = s.find_first_of(":");
	if (i < s.length())
	{
		if (is_variable_correct(s.substr(0, i)))
		{
			*label = s.substr(0, i);
			if (i == s.length() - 1)
			{
				*rest_of_line = s;
			}
			else
			{
				*rest_of_line = s.substr(i);
			}
			//cout << *label << "***" << *rest_of_line;
		}
	}
}

void FirstPass::executeLabel(string firstWord, string* line)
{
	end_of_line = false;
	end_of_line = false;

	string label;

	unsigned int i = firstWord.find_first_of(":");
	label = firstWord.substr(0, i);
	*line = *line + "\n";
	*line = (*line).substr(i + 1);

	//cout << "+++++++++\n" << *line << "+++++++++\n";

	if (currentSection->getName().compare(".rodata"))
		add_label_in_RoDataLabels(label);

	Symbol* symbol = symbolTable->getSymbol(label);
	if (symbol == 0)
	{
		symbolTable->insertSymbol(new Symbol(label, currentSection->getID(), currentSection, locationCounter, false, false));
	}
	else
	{
		if (symbol->isGlobal() == true)
		{
			if (symbol->getSectionName() == "UND")
			{
				symbol->setSectionNo(currentSection->getID());
				symbol->setSection(currentSection);
				symbol->setValue(locationCounter);
			}
			else
			{
				//throw new SyntaxError(lineCounter, "Error: symbol " + label + " is defined more than once.");
				//end_of_line = true;
				symbol->setValue(locationCounter);
				symbol->setSectionNo(currentSection->getID());
				symbol->setSection(currentSection);
			}
		}
		else
		{
			//throw new SyntaxError(lineCounter, "Error: symbol " + label + " is defined more than once.");
			//end_of_line = true;
			symbol->setValue(locationCounter);
			symbol->setSectionNo(currentSection->getID());
			symbol->setSection(currentSection);
		}
	}
}

bool FirstPass::isEQU(string firstWord)
{
	return (firstWord == ".equ");
}

bool FirstPass::isSection(string s, Line** currWord)
{
	int i;
	if (symbolTable->getSection(s) != 0 && s!= ".section")
	{
		throw new SyntaxError(lineCounter, "Error: section " + s + " is already defined.");
		end_of_line = true;
	}
	if (((*currWord) != 0) && (s == ".section") && (symbolTable->getSection((*currWord)->word) != 0))
	{
		throw new SyntaxError(lineCounter, "Error: section " + s + " is already defined.");
		end_of_line = true;
	}
	if ((s == ".text") || (s == ".data") || (s == ".rodata") || (s == ".bss") || (s == ".section"))
		return true;
	else
		return false;
}

void FirstPass::executeSection(string s, Line** currWord, int r, int w, int e)
{
	if (((*currWord) != 0) && ((s == ".text") || (s == ".data") || (s == ".rodata") || (s == ".bss")))
		throw new SyntaxError(lineCounter, "Error: the declaration of section is followed by arguments.");

	if (currentSection != undefiendSection)
	{
		currentSection->setSize(locationCounter);
		locationCounter = 0;
	}
	if (((s == ".text") || (s == ".data") || (s == ".rodata") || (s == ".bss")))
	{
		if (s == ".text") { w = 1; e = 1; }
		if (s == ".data") { w = 1; e = 1; }
		if (s == ".bss")  { w = 1; }
		currentSection = new Section(s, 0, false, 0, r, w, e);
		insertLineToLineList(new Line(s), lineCounter);
	}
	else
	{
		string sectionName = (*currWord)->word;
		currentSection = new Section(sectionName, 0, false, 0, r, w, e);
		insertLineToLineList(new Line(".section " + (*currWord)->word), lineCounter);
	}
	currentSection->setID(numOfSections);
	symbolTable->increment_ID_of_all_symbols();
	symbolTable->insertSection(currentSection);
	numOfSections++;
}

bool FirstPass::is_Byte_Word_Align_Skip(string firstWord)
{
	if (firstWord == ".byte") return true;
	if (firstWord == ".word") return true;
	if (firstWord == ".align") return true;
	if (firstWord == ".skip") return true;
	return false;
}
void FirstPass::execute_Byte_Word_Align_Skip(string firstWord, Line** currWord, string* line)
{
	if (currentSection->getW() == 0)
	{
		//throw new SyntaxError(lineCounter, "Error: it is forbidden to write in " + currentSection->getName() +" section because W bit is 0.");
		//return;
	}
	if ((*currWord) == 0)
	{
		throw new SyntaxError(lineCounter, "Error: there are no parameters after " + firstWord + ". declaration");
	}
	//else if (!is_number((*currWord)->word) && (firstWord == ".skip"))
	//{
	//	throw new SyntaxError(lineCounter, "Error: the type of the parameter after " + firstWord + ". declaration should be integer.");
	//}
	else if (currentSection->getName() == "UND")
	{
		throw new SyntaxError(lineCounter, "Error: it is forbidden to use " + firstWord + " declaration in " + currentSection->getName() + " section");
	}
	/*else if (currentSection->getName() == ".bss")
	{
		if (firstWord == ".skip")
		{
				locationCounter += convert_to_int(((*currWord)->word));
				insertLineToLineList(line, lineCounter);
		}
		else
		{
			throw new SyntaxError(lineCounter, "Error: it is forbidden to use " + firstWord + " declaration in " + currentSection->getName() + " section");
			return;
		}
	}*/
	else if (firstWord == ".byte")
	{
		Line* line = 0; int num = 0;
		separte_commas_from_string(currWord, &line, &num);

		Line* charLine = new Line(".byte");
		add_words_at_the_beggining_of_first_line(&line, &charLine);
		insertLineToLineList(line, lineCounter);
		locationCounter += 1 * num;
		//cout << "CHAR NUMBER OF WORDS " << num << " locationCoutner " << locationCounter << "\n";

	}
	else if (firstWord == ".word")
	{
		Line* line = 0; int num = 0;
		separte_commas_from_string(currWord, &line, &num);

		Line* charLine = new Line(".word");
		add_words_at_the_beggining_of_first_line(&line, &charLine);
		insertLineToLineList(line, lineCounter);
		locationCounter += 2 * num;				// long bi bio sve isto samo se dodaje 4*num umesto 2*num
		//cout << "WORD NUMBER OF WORDS " << num << " locationCoutner " << locationCounter << "\n";
	}
	else if (firstWord == ".align")
	{
		(*line) = insert_white_spaces_between_operations(*line);
		(*line) = delete_white_spaces_at_the_begging(*line);
		(*line) = delete_white_spaces_at_the_end(*line);
		(*line) = insert_white_spaces_between_operations(*line);
		(*line) = delete_white_spaces_at_the_end(*line);

		(*line) = (*line).substr(6);
		//cout << (*line) << " FFFFFFFFFFFFFFFF\n";
		Line* line1 = 0; int n;
		get_words_between_commas(*line, &n, &line1);

		while (line1 != 0)
		{
			line1->word = insert_white_spaces_between_operations(line1->word);
			line1->word = delete_white_spaces_at_the_begging(line1->word);
			line1->word = delete_white_spaces_at_the_end(line1->word);
			line1->word = insert_white_spaces_between_operations(line1->word);
			line1->word = delete_white_spaces_at_the_end(line1->word);

			//cout << line1->word << " mm\n";
			Line* words = 0; int n;
			get_words_between_white_spaces(line1->word, &n, &words);
			Line* currWords = words;
			string firstWord = currWords->word;
			printLine(&currWords);

			if (are_all_symbols_in_list_already_defined(&currWords))
			{
				Line* postfix = 0;
				infix_to_postfix(&currWords, &postfix, lineCounter);
				printLine(&postfix);

				bool finished;
				int value; bool a = false;
				eval_postfix(firstWord, &postfix, lineCounter, &finished, &value, a);

				if (finished)
				{
					//cout << "nn " << value << " nn\n";
					//cout << "LC " << locationCounter << "\n";
					int value1 = value;
					value = 1 << value;
					while (locationCounter % value != 0)
						locationCounter += 1;
					//locationCounter += value;
					//cout << "LC " << locationCounter << "\n";

					Line* newLine = new Line(".align");
					Line* resultLine = new Line(convert_to_string(value1)); // BILO JE SAMO to_string!!!
					add_words_at_the_end_of_first_line(&newLine, &resultLine);
					insertLineToLineList(newLine, lineCounter);
				}
			}
			else
				throw new SyntaxError(lineCounter, "Error: the parameter after .align declaration can not be calculated at this point!");


			line1 = line1->nextWord;
		}

	}
	else if (firstWord == ".skip")
	{
		(*line) = insert_white_spaces_between_operations(*line);
		(*line) = delete_white_spaces_at_the_begging(*line);
		(*line) = delete_white_spaces_at_the_end(*line);
		(*line) = insert_white_spaces_between_operations(*line);
		(*line) = delete_white_spaces_at_the_end(*line);

		(*line) = (*line).substr(5);
		//cout << (*line) << " FFFFFFFFFFFFFFFF\n";
		Line* line1 = 0; int n;
		get_words_between_commas(*line, &n, &line1);

		while (line1 != 0)
		{
			line1->word = insert_white_spaces_between_operations(line1->word);
			line1->word = delete_white_spaces_at_the_begging(line1->word);
			line1->word = delete_white_spaces_at_the_end(line1->word);
			line1->word = insert_white_spaces_between_operations(line1->word);
			line1->word = delete_white_spaces_at_the_end(line1->word);

			Line* words = 0; int n;
			get_words_between_white_spaces(line1->word, &n, &words);
			Line* currWords = words;
			string firstWord = currWords->word;
			printLine(&currWords);

			if (are_all_symbols_in_list_already_defined(&currWords))
			{
				Line* postfix = 0;
				infix_to_postfix(&currWords, &postfix, lineCounter);
				printLine(&postfix);

				bool finished;
				int value; bool a = false;
				eval_postfix(firstWord, &postfix, lineCounter, &finished, &value, a);

				if (finished)
				{
					//cout << "nn " << value << " nn\n";
					//cout << "LC " << locationCounter << "\n";
					locationCounter += value;
					//cout << "LC " << locationCounter << "\n";

					Line* newLine = new Line(".skip");
					Line* resultLine = new Line(convert_to_string(value));	// BILO JE SAMO to_string
					add_words_at_the_end_of_first_line(&newLine, &resultLine);
					insertLineToLineList(newLine, lineCounter);
				}

			}
			else
				throw new SyntaxError(lineCounter, "Error: the parameter after .skip declaration is can not be calculated at this point!");

		line1 = line1->nextWord;

	}
	}

}


void FirstPass::try_to_add_new_symbol_to_symbolTable(string s, int value, bool isDefinedByEQU)
{
	Symbol* symbol = symbolTable->getSymbol(s);
	if (symbol == 0)													// symbol does not exsit in symbolTable --> insert to table
	{
		if (currentSection->getID() == 0)
			throw new SyntaxError(lineCounter, "Error: a definition of symbol appeared before naming any section.");
		else
			symbolTable->insertSymbol(new Symbol(s, currentSection->getID(), currentSection, value, false, isDefinedByEQU));
	}
	else
	{
		if ((symbol->isGlobal()) && (symbol->getSectionNo() != 0))			// symbol exists in symbolTable, it's global and defined --> error
		{
			throw new SyntaxError(lineCounter, "Error: symbol " + s + " is already defined.");
		}
		else if ((symbol->isGlobal()) && (symbol->getSectionNo() == 0))		// symbol exists in symbolTable, it's global and but NOT defined --> set value and section
		{
			symbol->setSectionNo(currentSection->getID());
			symbol->setSection(currentSection);
			symbol->setValue(value);
		}
		else if (!(symbol->isGlobal()) && (symbol->getSectionNo() == 0))	// it shoudn't enter this branch
		{

		}
		else if (!(symbol->isGlobal()) && (symbol->getSectionNo() != 0))	// symbol exists in symbolTable, it is not global, but is already defined --> error
		{
			throw new SyntaxError(lineCounter, "Error: symbol " + s + " is already defined.");
		}
	}
}


void FirstPass::executeEQU(string firstWord, Line** currWord, int n)						// *****firsWord = a, currWord = {b}
{														// a EQU b --- firstWord = a, currWord = {EQU, b}
	//*currWord = (*currWord)->nextWord;					// currWord = EQU
	//*currWord = (*currWord)->nextWord;					// currWord = b
	if (!is_variable_correct(firstWord)) throw new SyntaxError(lineCounter, "Error: variable before EQU is not correct.");
	if (*currWord == 0)
	{	throw new SyntaxError(lineCounter, "Error: there are no parameters after EQU."); return; }
	
	Line* postfix;
	Line* currWord2 = *currWord;
	while (currWord2 != 0)
	{
		string s = (currWord2)->word;
		if (is_hex_number(s))
		{
			int a = 0;
			if (s.substr(2,1) == "0" || s.substr(2,1) == "1" || s.substr(2,1) == "2" || s.substr(2,1) == "3" 
			 || s.substr(2,1) == "4" || s.substr(2,1) == "5" || s.substr(2,1) == "6" || s.substr(2,1) == "7" )
				a = convert_to_int_from_hex_when_hex_is_positive(s.substr(2));
			else 
				a = convert_to_int_from_hex(s.substr(2)); 
			s =  convert_to_string(a);
			(currWord2)->word = s;
		}
		currWord2 = (currWord2)->nextWord;
	}
	//currWord = currWord2;
	infix_to_postfix(currWord, &postfix, lineCounter);
	
	

	bool finished;
	int value;
	eval_postfix(firstWord, &postfix, lineCounter, &finished, &value, false);


	if (finished == true)
	{
		try_to_add_new_symbol_to_symbolTable(firstWord, value, true);
	}
	else
	{
		//cout << "fjdkaljfdksaljfdklajfsdkl\n";
		add_line_in_TNS(firstWord, &postfix);
		print_TNS();

	}
	//symbolTable->printSymbols();
	//		cout << "====================================\n";
}

void FirstPass::eval_postfix(string firstWord, Line** postfix, int lineCounter, bool* finished, int* result, bool afterFirstPass)
{
	Line* stackHead = 0; Line* stackTail = 0;
	int rez = 0;
	Line* currPostfix = *postfix;
	while (currPostfix != 0)
	{
		string x = currPostfix->word;
		//cout << x << "mmmmmmmmm\n";

		if ((x == "+") || (x == "-") || (x == "*") || (x == "/"))	// binarni operator
		{
			Line* l2 = popWord(&stackHead, &stackTail);
			Line* l1 = popWord(&stackHead, &stackTail);

			string ops2;
			string ops1;

			if (l2 != 0)
				ops2 = l2->word;
			if (l1 != 0)
				ops1 = l1->word;

			if (l1 == 0)	// unarni operator
			{
				if (is_number(ops2))
				{
					rez = convert_to_int(ops2);
				}
				else
				{
					Symbol* s2 = symbolTable->getSymbol(ops2);
					if (s2 == 0)
					{
						//cout << "N0-\n";
						if  (firstWord == ops2)
						{
							throw new SyntaxError(lineCounter, "Error: there are circular dependencies in EQU directive.");
							*finished = false;
							return;
						}
						TNS* currTNS = my_TNS_head;
						while ((currTNS != 0) && (!afterFirstPass))
						{
							//cout << ops2 << " 000000000000000000000\n";
							if ((ops2 == currTNS->label) or (firstWord == ops2))
							{
								throw new SyntaxError(lineCounter, "Error: there are circular dependencies in EQU directive.");
								*finished = false;
								return;
							}
							currTNS = currTNS->next;
						}
						*finished = false;
						return;
					}
					else
					{
						if (s2->getSectionName() == "UND")
						{
							*finished = false;
							return;
						}
						else
						{
							rez = s2->getValue();
						}
					}
				}
				if (x == "-")
					rez = -1 * rez;
				pushWord(&stackHead, &stackTail, convert_to_string(rez));

			}
			else {	// begin of binary operation
			//cout << "     " << ops2  << "    " << ops1 << "\n";
			int op2, op1;



			if (is_number(ops2))
			{
				op2 = convert_to_int(ops2);
			}
			else
			{
				Symbol* s2 = symbolTable->getSymbol(ops2);

				if (s2 == 0)
				{
					//cout << "N2\n";
					if  (firstWord == ops2)
					{
						throw new SyntaxError(lineCounter, "Error: there are circular dependencies in EQU directive.");
						*finished = false;
						return;
					}
					TNS* currTNS = my_TNS_head;
					while ((currTNS != 0) && (!afterFirstPass))
					{
						//cout << ops2 << " 2222222222222222\n";
						if ((ops2 == currTNS->label) or (firstWord == ops2))
						{
							throw new SyntaxError(lineCounter, "Error: there are circular dependencies in EQU directive.");
							*finished = false;
							return;
						}
						currTNS = currTNS->next;
					}
					*finished = false;
					return;
				}
				else
				{
					if (s2->getSectionName() == "UND")
					{
						*finished = false;
						return;
					}
					else
					{
						op2 = s2->getValue();
					}
				}
			}

			if (is_number(ops1))
			{
				op1 = convert_to_int(ops1);
			}
			else
			{
				Symbol* s1 = symbolTable->getSymbol(ops1);

				if (s1 == 0)
				{
					//cout << "N1\n";


					if  (firstWord == ops1)
					{
						throw new SyntaxError(lineCounter, "Error: there are circular dependencies in EQU directive.");
						*finished = false;
						return;
					}

					TNS* currTNS = my_TNS_head;

					while ((currTNS != 0) && (!afterFirstPass))
					{
						//cout << ops1 << " 11111111111111111\n";
						if ((ops1 == currTNS->label) or (firstWord == ops1))
						{
							throw new SyntaxError(lineCounter, "Error: there are circular dependencies in EQU directive.");
							*finished = false;
							return;
						}
						currTNS = currTNS->next;
					}
					*finished = false;
					return;
					*finished = false;
					return;
				}
				else
				{
					if (s1->getSectionName() == "UND")
					{
						*finished = false;
						return;
					}
					else
					{
						op1 = s1->getValue();
					}
				}
			}
			//if (op2 == 0)
			//	cout << "NULAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAA\n";

			if ((x == "/") && (op2 == 0))
			{
				throw new SyntaxError(lineCounter, "Error: trying division with 0 in EQU directive.");
				cout << "Error: trying division with 0 in EQU directive.";
				*finished = false;
				return;
			}
			if (x == "+")
				rez = op1 + op2;
			else if (x == "-")
				rez = op1 - op2;
			else if (x == "*")
				rez = op1 * op2;
			else if (x == "/")
				rez = op1 / op2;

			//cout << op1 << "&&&" << op2 << "&&&" << rez << "&&&\n";

			pushWord(&stackHead, &stackTail, convert_to_string(rez));
			printLine(&stackHead);
		}} // end of binary operation
		else														// operand
		{
			pushWord(&stackHead, &stackTail, x);
		}
		currPostfix = currPostfix->nextWord;
	}
	string r = popWord(&stackHead, &stackTail)->word;
	if (is_number(r))
	{
		rez = convert_to_int(r);
	}
	else
	{
		Symbol* s2 = symbolTable->getSymbol(r);
		if (s2 == 0)
		{
			//cout << "NULLPTR\n";

			if (firstWord == r)
			{
				throw new SyntaxError(lineCounter, "Error: there are circular dependencies in EQU directive.");
				*finished = false;
				return;
			}
			TNS* currTNS = my_TNS_head;
			while ((currTNS != 0) && (!afterFirstPass))
			{
				if ((r == currTNS->label))
				{
					throw new SyntaxError(lineCounter, "Error: there are circular dependencies in EQU directive.");
					*finished = false;
					return;
				}
				currTNS = currTNS->next;
			}
			//cout << "BLLLLLLLLLLLLLAH\n";
			(*finished) = false;
			return;
		}
		else
		{
			rez = s2->getValue();
		}
	}
	//cout << rez << " rezrez\n";
	if (stackHead == 0)
	{
		(*result) = rez;
		(*finished) = true;
	}
	else
	{
		throw new SyntaxError(lineCounter, "Error: assignemet in EQU directive is not correct.");
		(*result) = 0;
		(*finished) = false;
	}
}

bool FirstPass::is_symbol_in_label_part_of_TNS(string symbolName)
{
	TNS* currTNS = my_TNS_head;
	while (currTNS != 0)
	{
		if (currTNS->label == symbolName)
			return true;
		currTNS = currTNS->next;
	}
	return false;
}

void FirstPass::add_line_in_TNS(string label, Line** postfix)
{
	TNS* newTNS = new TNS(label, (*postfix));
	if (my_TNS_head == 0)
		my_TNS_head = newTNS;
	else
		my_TNS_tail->next = newTNS;
	my_TNS_tail = newTNS;
}

int FirstPass::number_of_lines_in_TNS()
{
	int ret = 0;
	TNS* currTNS = my_TNS_head;
	while (currTNS != 0)
	{
		ret++;
		currTNS = currTNS->next;
	}
	return ret;
}

void FirstPass::print_TNS()
{
	//cout << "TNS\n";
	TNS* currTNS = my_TNS_head;
	while (currTNS != 0)
	{
		//cout << currTNS->ordinalNumber << " " << currTNS->label << " ";
		printLine(&currTNS->postfixHead);
		//cout << "\n";
		currTNS = currTNS->next;
	}
}

void FirstPass::check_circular_dependencies_TNS(string ops2, string firstWord, bool* finished, int* rez, Line** stackHead, Line** stackTail, bool afterFirstPass)
{
	Symbol* s2 = symbolTable->getSymbol(ops2);

	if (s2 == 0)
	{
		//cout << "N0-\n";
		if  (firstWord == ops2)
		{
			throw new SyntaxError(lineCounter, "Error: there are circular dependencies in EQU directive.");
			*finished = false;
			return;
		}
		TNS* currTNS = my_TNS_head;
		while ((currTNS != 0) && (!afterFirstPass))
		{
			//cout << ops2 << " 000000000000000000000\n";
			if ((ops2 == currTNS->label) or (firstWord == ops2))
			{
				throw new SyntaxError(lineCounter, "Error: there are circular dependencies in EQU directive.");
				*finished = false;
				return;
			}
			currTNS = currTNS->next;
		}
		*finished = false;
		return;
	}
	else
	{
		if (s2->getSectionName() == "UND")
		{
			*finished = false;
			return;
		}
		else
		{
			*rez = s2->getValue();
		}
	}
}

TNS* FirstPass::get_first_line_from_TNS()
{
	if (my_TNS_head == 0)
	{
		return 0;
	}
	TNS* ret = my_TNS_head;
	my_TNS_head = my_TNS_head->next;
	if (my_TNS_head == 0)
	{
		my_TNS_tail = 0;
	}
	return ret;
}

void FirstPass::calculate_TNS()
{
	int a = number_of_lines_in_TNS();
	int i = 0;
	//cout << "BEFORE\n";
	print_TNS();
	TNS* currTNS = get_first_line_from_TNS();
	bool finished = false;
	int result = 0;
	while (currTNS != 0)
	{
		eval_postfix(currTNS->label, &(currTNS->postfixHead), lineCounter, &finished, &result, true);
		if (finished == false)
		{
			add_line_in_TNS(currTNS->label, &currTNS->postfixHead);
		}
		else
		{
			//cout << result << "result\n";
			try_to_add_new_symbol_to_symbolTable(currTNS->label, result, true);
		}
		//cout << "AFTER\n";
		print_TNS();
		currTNS = get_first_line_from_TNS();
		i++;
		if (i > a * (a + 1) / 2)
		{
			throw new SyntaxError(lineCounter, "Error: dependencies in EQU directives can not be resolved.");
			return;
		}
	}
}

void FirstPass::insertLineToLineList(Line* l, int n)
{
	if (l == 0)
		return;
	LineList* newLineList = new LineList(l, n);
	if (myLineListHead == 0)
		myLineListHead = newLineList;
	else
		myLineListTail->nextLine = newLineList;
	myLineListTail = newLineList;
}

void FirstPass::printLineList()
{
	cout << "List of lines\n";
	LineList* currLine = myLineListHead;
	while (currLine != 0)
	{
		printLine(&currLine->line);
		currLine = currLine->nextLine;
	}
}

bool FirstPass::two_lines_are_equal(Line* first, Line* second)
{
	Line* currFirst = first;
	Line* currSecond = second;
	while ((currFirst != 0) && (currSecond != 0))
	{
		if (currFirst->word != currSecond->word)
			return false;
		currFirst = currFirst->nextWord;
		currSecond = currSecond->nextWord;
	}
	if ((currFirst != 0) || (currSecond != 0))
		return false;
	return true;
}

void FirstPass::separte_commas_from_string(Line** currWord, Line** line, int* num)
{
	*num = 0;

	Line* prevWord = 0;
	(*line) = 0;

	while (*currWord != 0)
	{
		string s  = (*currWord)->word;

		unsigned int i = s.find_first_of(",");
		if ((i < s.length()) && (s != ","))
		{
			Line* newWords = 0;
			int n;
			prevWord = *currWord;
			*currWord = (*currWord)->nextWord;							// delete a,b,c from currWord and add it to the list as 3 elements (a, b, c)

			if (check_if_two_commas_are_not_next_to_each_other(s)) throw new SyntaxError(lineCounter, "Error: there are 2 or more commas next to each other.");

			get_words_between_commas(s, &n, &newWords);
			add_words_at_the_beggining_of_first_line(currWord, &newWords);

			Line* m = 0;
			get_words_between_commas(s, &n, &m);
			Line* currLine = m;
			while (currLine != 0)
			{
				//cout << currLine->word << " aaaaaaaaaaaaaaaaaaa\n";
				Line* newLine = currLine;
				newLine->nextWord = 0;
				if (newLine->word != ",")
				{
					add_words_at_the_end_of_first_line(line, &newLine);
					(*num) += 1;
				}
				//else
				//	cout << ",,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,\n";
				currLine = currLine->nextWord;
			}

			if ((*currWord) != 0)
				s  = (*currWord)->word;

			Line* curr = *currWord;
			while (curr != 0)
			{
				//cout << curr->word << "$";
				curr = curr->nextWord;
			}
			//cout << "\n";
		}
		else
		{
			Line* m = new Line(s);
			if (s != ",")
			{
				add_words_at_the_end_of_first_line(line, &m);
				(*num) += 1;
			}
		}

		if ((prevWord != 0) && (prevWord->word == ",") && (s == ","))
		{
			throw new SyntaxError(lineCounter, "Error: there are 2 or more commas next to each other.");
		}
		//if (s != ",")
		//{
		//	if (is_variable_correct(s) == false) throw new SyntaxError(lineCounter, s+ " " + (*currWord)->word + "Error: variable does not have appropriate form - must start with a letter, and contain only letters and digits.");
		//}
		if (((*currWord)->nextWord == 0) && (s == ","))
		{
			throw new SyntaxError(lineCounter, "Error: missing variable after last comma.");
		}

		prevWord = *currWord;
		*currWord = (*currWord)->nextWord;
	}

	//cout << "Number of words in .char " << (*num) << "\n";
	printLine(line);

}

bool FirstPass::are_all_symbols_in_list_already_defined(Line** line)
{
	Line* currWord = (*line);
	while (currWord != 0)
	{
		Symbol* s = symbolTable->getSymbol(currWord->word);
		//cout << " " << currWord->word << " ";
		//if (s != 0)
		//	cout << s->isGlobal();
		string a = currWord->word;
		if (((s != 0) && s->isGlobal()) && (!is_number(a)) && (a != "+") && (a != "-") && (a != "*") && (a != "/"))
			return false;
		currWord = currWord->nextWord;
	}
	return true;
}

void FirstPass::add_label_in_RoDataLabels(string label)
{
	RoDataLabels* newLabel = new RoDataLabels(label);
	if (roDataLabelsTail == 0)
		roDataLabelsHead = newLabel;
	else
		roDataLabelsTail->next = newLabel;
	roDataLabelsTail = newLabel;
}

void FirstPass::print_RoDataLabels()
{
	//cout << "Labels in .rodata\n";
	RoDataLabels* currLabel = roDataLabelsHead;
	while (currLabel != 0)
	{
		//cout << currLabel->label << " ";
		currLabel = currLabel->next;
	}
	//cout << "\n";
}

