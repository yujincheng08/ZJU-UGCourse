#!/bin/bash
#Declare value. max/min for the max/min number.
#avg for the average value.
#i for loop and num for inputing.
declare -i max min avg i num
#Make a temperoray file.
tmp=$(mktemp)
#Loop to read 100 number from standard input.
for((i=0;i<100;i++))
do
	read num	#Read it.
	((avg+=num))	#Add it into ave.
	echo ${num} >>${tmp}	#Add it to the tmp file.
done
#Sort the file in numeric sort. 
sort -no ${tmp} ${tmp}
#Calculate the avg by dividing it by 100.
((avg/=100))
#Use head and tail to output the first and the last number.
echo The minimum number is `head -1 ${tmp}`
echo The maximum number is `tail -1 ${tmp}`
echo The average of them is ${avg}
echo Result after sorting:
#Output the content in the tmp file, which was sorted before.
cat ${tmp}
#Remove the tmp file.
rm -f ${tmp}
