/*
 * AuxiliaryClass.h
 *
 *  Created on: Mar 1, 2019
 *      Author: user
 */

#ifndef AUXILIARYCLASS_H_
#define AUXILIARYCLASS_H_

#include "FirstPass.h"
#include <iostream>
#include <fstream>
#include <sstream>
#include <string>

struct Line;

using namespace std;



	string delete_white_spaces_at_the_begging(string s);
	string delete_white_spaces_at_the_end(string s);
	string delete_white_spaces_between_words(string s);
	string delete_all_white_spaces_between_words(string s);
	string insert_white_spaces_between_operation(string s, string operation);
	string insert_white_spaces_between_operations(string s);
	void get_words_between_white_spaces(string s, int* numOfWords, Line** firstWord);
	void get_words_between_commas(string s, int* numOfWords, Line** firstWord);
	void get_words_between_plus_and_minus(string s, int* numOfWords, Line** firstWord, Line** firstSign);
	void add_words_at_the_beggining_of_first_line(Line** l1, Line** l2);
	void add_words_at_the_end_of_first_line(Line** l1, Line** l2);
	bool check_if_two_commas_are_not_next_to_each_other(string s);
	bool is_empty(string s);
	bool just_white_spaces(string s);
	bool is_variable_correct(string s); 					// starts with letter and contain letters and digitis?
	bool is_number(string s);						// "20" --> 20
	int convert_to_int(string s);
	string convert_to_string(int n);
	string convert_negative_to_hex(int n, int num_of_Bytes);
	string convert_negative_to_hex_permutated(int n, int num_of_Bytes); // F324 --> 423F
	string convert_negative_to_hex_permutated2(int n, int num_of_Bytes); // F324 --> 24F3
	string convert_positive_to_hex_permutated(int n, int num_of_Bytes);
	int convert_to_int_from_hex(string s);
	int convert_to_int_from_hex_when_hex_is_positive(string s);

	void infix_to_postfix(Line** infix, Line** postfix, int lineCounter);
	int eval_postfix(Line** postfix, int lineCounter);

	void pushWord(Line** head, Line** tail, string word);
	Line* popWord(Line** head, Line** tail);
	Line* topWord(Line** head, Line** tail);
	int ipr(string s);
	int spr(string s);

	void printLine(Line** words);
	void deleteLine(Line** words);

	string nibble_to_16(int c1);

	bool psw(string s);					// PSW
	bool absolute_immediate(string s); 			// 20
	bool symbol_immediate(string s);			// &x
	bool symbol_absolute(string s);				// x 
	bool location_from_the_address(string s);		// *20
	bool reg_dir(string s);					// r3
	bool reg_indir_absolute_offset(string s); 		// r4[32]  r4[0x32]
	bool reg_indir_symbol_offset(string s);	  		// r5[x]
	bool pc_rel_offset(string s);		      		// $x

	bool reg_dir_h(string s);				// r3h
	bool reg_dir_l(string s);				// r3l

	bool reg_indir_h_absolute_offset(string s); 		// r4h[32]
	bool reg_indir_l_absolute_offset(string s); 		// r4l[32]

	bool reg_indir_h_symbol_offset(string s);	  	// r5h[x]
	bool reg_indir_l_symbol_offset(string s);	  	// r5l[x]

	bool reg_indir(string s);				// [r4]
	bool reg_indir_h(string s);				// [r4h]
	bool reg_indir_l(string s);				// [r4l]

	bool is_hex_number(string s);				// 0x1234
	int length_of_hex_number(string s);			// 0x1234 --> length = 4

	int return_num_of_registar_from_string(string s, int i);

	string convert_from_int_to_Byte(int x);
	int number_of_digits(int n);


#endif /* AUXILIARYCLASS_H_ */




