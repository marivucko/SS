all: prog

prog: main.o AuxiliaryClass.o FirstPass.o SecondPass.o Section.o Symbol.o SymbolTable.o
	g++ main.o AuxiliaryClass.o FirstPass.o SecondPass.o Section.o Symbol.o SymbolTable.o -o prog

main.o: main.cpp
	g++ -c main.cpp

AuxiliaryClass.o: AuxiliaryClass.cpp AuxiliaryClass.h
	g++ -c AuxiliaryClass.cpp

FirstPass.o: FirstPass.cpp FirstPass.h
	g++ -c FirstPass.cpp

SecondPass.o: SecondPass.cpp SecondPass.h
	g++ -c SecondPass.cpp

Section.o: Section.cpp Section.h
	g++ -c Section.cpp

Symbol.o: Symbol.cpp Symbol.h
	g++ -c Symbol.cpp

SymbolTable.o: SymbolTable.cpp SymbolTable.h
	g++ -c SymbolTable.cpp

clean:
	rm -rf *o prog

##


#OBJS = file1.o file2.o file3.o
#
#prog1:	file1.o file2.o file3.o
#	gcc -o prog1 file1.o file2.o file3.o
#
#file1.o: file1.c mydefs.h
#	gcc -c file1.c
#
#file2.o: file2.c mydefs.h
#	gcc -c file2.c
#
#file3.o: file3.c
#	gcc -c file3.c
#
#clean:
#	rm file1.o file2.o file3.o




#OBJS = file1.o file2.o file3.o
#
#prog1: ${OBJS}
#	${CC} -o $@ ${OBJS}
#
#file1.o file2.o: mydefs.h
#
#clean:
#	rm ${OBJS}


#OBJS = file1.o file2.o file3.o 
#
#prog1: ${OBJS} 
#	${CC} -o $@ ${OBJS} 
#
#file1.o file2.o: mydefs.h 
#
#clean: 
#	rm ${OBJS} 

#%.o: %.c
#$(CC) $(CPPFLAGS) $(CFLAGS) -c 




