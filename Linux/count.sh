#!/bin/bash
#Declare values.
#totalSize for total size of all the files.
#subDirNum for the number of all the sub directorise.
#filesNum for the number of all the normal files.
#binNum for the number of all the binary files.
declare -i totalSize=0 subDirNum=0 filesNum=0 binNum=0
#Decalre the color.
declare white='\e[00m' green='\e[01;32m'
#The name of the shell script and the path to process
declare name=$0 path
#If no arguments, then the path is current working directory.
#Otherwise, the first argument is the path to process
if [ $# -eq 0 ]
then
	path=.
else
	path=$1
fi
#List all the file in the derectory using ls command.
#Error redirected to standrad output, which will output later after some process.
files=$(ls -A $path 2>&1)
if [ ! $? -eq 0 ]
then
	echo -n $0 >&2
	#Replace the error message 'ls' by the name of the script.
	echo ${files} | sed "s/ls//" >&2
	#Exit the script with error number 1.
	exit 1
fi
#Begin counting.
for i in ${files}
do
	#If the name refers to a directory.
	if [ -d $path/${i} ]
	then
		#Plus the counting value.
		((subDirNum++))
	#If the name refers to a normal file, which also includes binary files.
	elif [ -f $path/${i} ]
	then
		#Get its size by wc -c.
		#Redirect the error message.
		file=(`wc -c $path/${i} 2>/dev/null`)
		#If successed.
		if [ $? -eq 0 ]
		then
			#Plus the counting value.
			((totalSize+=file[0],filesNum++))
		fi
		if [ -x $path/${i} ]
		then
			#Also the counting value of binary files, if the file is a binary file.
			((binNum++))
		fi
	fi
done
#Output the result with some colors.
echo -e "${white}"The number of normal file\(s\) is "${green}${filesNum}"
echo -e "${white}"The number of subdictory\(-ies\) is "${green}${subDirNum}"
echo -e "${white}"The number of executive file\(s\) is "${green}${binNum}"
echo -e "${white}"The total size of all file\(s\) is "${green}${totalSize}"
#Reset the color.
echo -ne "${white}"
