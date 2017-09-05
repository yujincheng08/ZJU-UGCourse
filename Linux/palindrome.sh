#!/bin/bash
declare -i len flag=1 i j
read -p "Enter a line of string: " str
#Delete non-alphabetic charactor
str=${str//[!a-zA-Z]/}
#Get its length
len=${#str}
for((i=0;i<len/2;i++))
do
	#loop and check one by one
	((j=len-i-1))
	if [ ${str:i:1} != ${str:j:1} ]
	then
		#Not success.
		flag=0
		break
	fi
done
if [ $flag -eq 0 ]
then
echo No. It is not palindrome.
else
echo Yes. It is palindrome.
fi
