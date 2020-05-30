#!/bin/bash

truncate -s 0 testfile.txt

while :
do 
	ans=$(yad --center --form --field="Write your code here":TXT "$LINE" --button="Execute" --width=600 --height=400); 

	if [[ $ans == "|" ]]; then
	   yad --center --warning --text "\nWarning: Empty Code!" --text-align=center --width=400 --height=100 --button="Return:1" --button="Exit:2"
	   ret=$?
	   if [[ $ret -eq 1 ]]; then 
	   	LINE=""
		continue ; fi
	   if [[ $ret -eq 2 ]]; then break ; fi
#	   [[ $? -eq 1 ]] && break
	else
	   truncate -s 0 testfile.txt
	   LINE=${ans::-1}
	   new=$(echo -e ${ans::-1})
	   echo "$new"  >> testfile.txt
	   eval "yacc -d yacc.y"
	   eval "lex lex.l" 
	   eval "gcc arraylist.c symbolTable.c code_generator.c lex.yy.c y.tab.c -ll"
	   eval "./a.out"

	   err="`cat ErrorsOut.txt`"
	   yad --center --form --field="Errors:":TXT "$err" --text-align=left --width=500 --height=500 --button="View Quadrables:0" --button="Return:1" --button="Exit:2"
	   ret=$?
	   if [[ $ret -eq 1 ]]; then continue ; fi
	   if [[ $ret -eq 2 ]]; then break ; fi

	   quad="`cat codeOut.txt`"
	   yad --center --form --field="Quadrables:":TXT "$quad" --text-align=left --width=500 --height=500 --button="View Parsing" --button="Return" --button="Exit"
	   ret=$?
	   if [[ $ret -eq 1 ]]; then continue ; fi
	   if [[ $ret -eq 2 ]]; then break ; fi

	   pars="`cat parserOut.txt`"
	   yad --center --form --field="Parsing Result:":TXT "$pars" --text-align=left --editable=FALSE --width=500 --height=500 --button="View Symbol Table" --button="Return" --button="Exit"
	   ret=$?
	   if [[ $ret -eq 1 ]]; then continue ; fi
	   if [[ $ret -eq 2 ]]; then break ; fi

	   sym="`cat symbolTableOut.txt`"
	   yad --center --form --field="":TXT "$sym" --text-align=left --width=500 --height=500 --button="Return" --button="Exit"
	   [[ $? -eq 1 ]] && break
	fi
done
