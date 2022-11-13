//============================================================================
// Name        : 02.cpp
// Author      : 
// Version     :
// Copyright   : Your copyright notice
// Description : Hello World in C++, Ansi-style
//============================================================================


#include "FirstPass.h"
#include "AuxiliaryClass.h"
#include <iostream>
#include <fstream>
#include <sstream>
#include <string>
#include <stdio.h>
#include <string.h>

using namespace std;



int main(int argc, char* argv[])
{
    string s = string(argv[1]);
    if (s == "asembler")
    {   if (argc != 5)
	{
		cout << "The number of the arguments is incorrect.\n";
	}
	else
	{	//  ./prog asembler -o tekst1.o tekst1.s
		string asembler = string(argv[1]);
		string object = string(argv[2]);
		string oFile = string(argv[3]);
		string sFile = string(argv[4]);

		string oFileName = "";
		string sFileName = "";

		bool commandLineError = false;
		if (oFile.length() <= 2)
			commandLineError = true;
		else
		{
			oFileName = oFile.substr(0, oFile.length() - 2);
			if (oFile.substr(oFile.length() - 2, 2) != ".o")
				commandLineError = true;
		}

		if (sFile.length() <= 2)
			commandLineError = true;
		else
		{
			sFileName = sFile.substr(0, sFile.length() - 2);
			if (sFile.substr(sFile.length() - 2, 2) != ".s")
				commandLineError = true;
		}

		if (oFileName != sFileName)
			commandLineError = true;
		
		if ((asembler != "asembler") || (object != "-o") || (commandLineError))
			cout << "The format of asembling should be 'asembler -o ulaz1.o ulaz1.s'.\n";
		else
		{	
			string inputFile = sFile + ".txt";
			string outputFile = oFile + ".txt";
			cout << inputFile << "***\n";
			FirstPass f(inputFile, outputFile, 0);		// 0 is the start address for linking
			f.firstPass();

			//cout << argv[1] << " \n";
			//FirstPass f("1.s.txt", convert_to_int(argv[1])); 
			//f.firstPass();
			
		}
	
	}
      }

	return 0;
}
