#!/bin/bash

declare mgrprofile=.mgr_profile
declare crse=Courses
declare crseprofile=${crse}/.crse_profile
declare crsebind=.crse_bind
declare crseinfo=.crse_info
declare -i logintype=-1 currentid=-1 currentid=-1
declare currentuser

#Function to get the return string using function emit.
function func()
{
	echo -n $($@ 3>&1 1>/dev/tty)
	#Redirect stdout to /dev/tty and 3 to stdout.
	#Then it can get the string from 3 and do not influent the standard output.
	#The first argument ($1) is used as command.
	#The rest of them are used as arguments for the command.
}

#Function to output string to file discription 3.
#Used to return a string.
#To get the string returned, use the function func.
function emit()
{
	echo -n 1>&3 $@
}

#It require the user to enter a new password and confirm it for a new account
#If successed, it will emit the new encrypted password.
#Usage: newPassword username
function newPassword()
{
	local pw pwc
	while true
	#Loop until the two password inputed are the same.
	do
		read -s -p "Input the password of ""$*"": " pw;echo
		#The frist time to input.
		if [ -z ${pw} ]
		then
			break #If an empty password entered, break the loop.
		fi
		read -s -p "Please confirm the password again: " pwc;echo
		#The second time to input.
		if [ ${pw} = ${pwc} ] #These two inputs must be the same
		then
			break
		fi
		echo >&2 "Sorry, please input again." #Well, input again
	done
	emit $(func encryptPasswd ${pw})
}

#Encrypt a password
function encryptPasswd()
{
	emit $(echo -n ${1} | md5sum | cut -d ' ' -f1)
}

#Check the last run status. If failed, then output error test and exit.
#Usage: checkstat $? errormsg
function checkstat() 
{
	if [ ! $1 -eq 0 ] #not equals to 0, which means fail
	then
		error $2 #output error message and exit
		exit -1
	fi
}

#Check if the account already exists.
#If so return 0, otherwise return 1
#Usage: accExit id [usrtype]
function accExit()
{
	#Try to get information
	local info=$(func accInfo $1)
	if [ -z "${info}" ]
	then
		#Fail to get. It means the account does not exist.
		return 1
	else
		#The account does exist.
		if [ $# -gt 1 ]
		then
		#If the second argument is set, we have to check its type.
			test $(func getUsrtype "${info}") -eq $2
			return $?
		else
			return 0
		fi
	fi
}

#Output error message to error steam
#Usage: error errormsg
function error()
{
	echo >&1 "$@"
}

#Get the account info from the profile.
function accInfo()
{
	emit $(cat ${mgrprofile}|egrep ":${1}\$")
}

#Get the username/password/usertype from the info string
function getUsrname()
{
	emit $(echo -n ${@}|cut -d ':' -f1)
	#Cut from the info string.
}

function getPasswd()
{
	emit $(echo -n ${@}|cut -d ':' -f2)
}

function getUsrtype()
{
	emit $(echo -n ${@}|cut -d ':' -f3)
}

#Input a password and then encrypt it.
function inputPasswd()
{
	local passwd
	read -sp "Please input the password of ""${*}"": " passwd;echo
	#Input password.
	emit $(func encryptPasswd ${passwd})
	#Encrypt it.
}

#Add a new user when logined as admin
#Usage newUser username password usertype userid
function newUser()
{
	if accExit $4
	then
		error "Sorry! There is already a user with the same id."
		error "Please check the id again."
		return 1
	fi
	echo >>$mgrprofile $1:$2:$3:$4
	return 0
}

#Delete an exist user by id when logined as amdin
#Usage: delUser userid
function delUser()
{
	if ! accExit $1
	then
		error "Sorry, cannot find and user with id :$1."
		error "Please check the id again."
		return 1
	fi
	sed -i '/'":$1\$"'/d' ${mgrprofile}
#echo "Deleted successfully."
	return 0
}

#Initialize the program when first logined
function initialize()
{
	#Try to make a directory and enter into it.
	mkdir hwmgr 2>/dev/null
	cd hwmgr
	checkstat $? "Fail to enter hwmgr directory!"
	mkdir ${crse} 2>/dev/null
	#Check the profile to identify if it's the first time to lunch.
	if [ -f ${mgrprofile} ] 
	then
		return 0
	fi
	echo It\'s the first time to use this system.
	echo Initializing the system...
	#New this two improtant profile.
	touch ${mgrprofile} ${crseprofile}
	echo Creating the administrator account...
	#Enter the name of the Administrator account
	local admin=$(func enterName 0)
	logintype=0 #temporatory set it to admin user
	local newpw=$(func newPassword ${admin})
	newUser ${admin} ${newpw} 0 0
	logintype=-1 #set it back
}

#Enter a user id/name.
#Usage: enterId/enterName usertype
function enterId()
{
	local -i id
	#If the second argument is set, there is no need to enter.
	if [ $# -eq 2 ]
	then
		emit $2
		return 0
	fi
	read -p "Enter the id of the $(printUsrtype $1): " id
	emit ${id}
	return 0
}

function enterName()
{
	local name
	read -p "Enter the name of the $(printUsrtype $1): " name
	emit ${name}
}

#Login by userid. It sets logintype and currentuser
function login()
{
	echo "You are about to login!"
	local -i userid=$(func enterId -1 $1)
	#Get info from id.
	local info=$(func accInfo ${userid})
	local username=$(func getUsrname "${info}")
	#Check whether the account exist. No using accExist for speed.
	if [ -z "${info}" ]
	then
		error "Sorry, cannot find this user. Please check your id."
		#Try to login again. But without id.
		login
		return $?
	fi
	local pwc=$(func getPasswd "${info}")
	local passwd=$(func inputPasswd "${username}")
	#Enter password and check it.
	if [ ${passwd} = ${pwc} ]
	then
		#Set the global values
		logintype=$(func getUsrtype "${info}")
		currentuser=${username}
		currentid=${userid}
		echo "Welcome ${username}!"
	else
		echo "Sorry! You entered a wrong password!"
		#Login again.
		login
		return $?
	fi
	#Clear the screen and print help information.
	clear
	printHelp
	return 0
}

#Print current user's logintype.
#if usertype is 0, then print Administrator
#if 1, then print Teacher
#if 2, then print Student
function printUsrtype()
{
	if [ $# -eq 0 ]
	then
		#If no argument, set it to logintype.
		printUsrtype ${logintype}
		return $?
	fi
	case $1 in
	0) echo -n Administrator;;
	1) echo -n Teacher;;
	2) echo -n Student;;
	3) echo -n Course;;
	4) echo -n Assignment;;
	-1) echo -n account you are going to login;;
	esac
}

#Print help information
function printHelp()
{
	echo "Welcome $(printUsrtype) ${currentuser} to Homework Manager!"
	echo Before your usage, you can enter help to read the user manual.
}

#A loop that get command
function getcmd()
{
	local cmd argv
	while true
	do
		read -p "> " cmd argv
		#Check wheather the command inputed avaliable
		case ${cmd} in
		"login"|"chadmin"|"chcrse"|\
		"addtch"|"deltch"|"chtch"|"lstch"|\
		"addstu"|"delstu"|"chstu"|"lsstu"|\
		"addcrse"|"mycrse"|"lscrse"|"delcrse"|\
		"bindtch"|"bindstu"|"unbindtch"|\
		"unbindstu"|"lsbind"|"editinfo"|\
		"crseinfo"|"arrasg"|"delasg"|\
		"lsasg"|"printasg"|"editasg"|"checkasg"|\
		"passwd"|"asginfo"|"clear") ${cmd} ${argv};;
		#Help information.
		"help") less ../readme;;
		"exit") exit 0;;
		"");;
		*) error "Unsupported command."
			error "Enter help for assistance."
		esac
	done
}

#Judge if the current user is an administrator/teacher/student account.
function isAdmin()
{
	if [ $# -eq 0 ]
	then
		#If no arguments, default check logintype
		if isAdmin ${logintype}
		then
			return 0
		else
			#In this term, print error information for convenience.
			error "Sorry, you are not an administrator."
			return 1
		fi
	fi
	#Check its type.
	if [ ${1} -eq 0 ]
	then
		return 0
	else
		return 1
	fi
}

#Similar with isAdmin
function isTch()
{
	if [ $# -eq 0 ]
	then
		if isTch ${logintype}
		then
			return 0
		else
			error "Sorry, you are not an teacher."
			return 1
		fi
	fi
	if [ ${1} -eq 1 ]
	then
		return 0
	else
		return 1
	fi
}

#Similar with isAdmin
function isStu()
{
	if [ $# -eq 0 ]
	then
		if isStu ${logintype}
		then
			return 0
		else
			error "Sorry, you are not a student."
			return 1
		fi
	fi
	if [ ${1} -eq 2 ]
	then
		return 0
	else
		return 1
	fi
}

#Add a new user by usertype
function addUsrByType()
{
	local -i id
	local name newpw
	#Loop for adding more user one time.
	while true
	do
		id=$(func enterId $1)
		#If id==0, then break the loop.
		if [ $id -eq 0 ]
		then
			break
		fi
		#Only when account does not exist.
		if ! accExit ${usrid} $1
		then
			error "Sorry, $(printUsrtype $1) account with $usrid does not exist."
			#Not a fatal error, continue to add user
			continue
		fi
		#Enter name and password, and add it to profile.
		name=$(func enterName $1)
		newpw=$(func newPassword "${name}")
		if newUser "${name}" ${newpw} $1 ${id}
		then
			echo "$(printUsrtype $1) ${name} with id ${id} added."
		fi
	done
}

#Change a existing user by usertype.
function chUsrByType()
{
	local -i id=$(func enterId $1 $2)
	local info=$(func accInfo $id)
	#Try to delete it first.
	if ! delUsrByType $1 $id
	then
		return 1
	fi
	#Get its old information form $info.
	local oname=$(func getUsrname "${info}")
	local opw=$(func getPasswd "${info}")
	echo "Now you are going to enter the new information for this account."
	echo "Enter empty string to preserve the old infromation."
	#Enter new name.
	local nname=$(func enterName $1)
	if [ -z $nname ]
	then
		nname=$oname
	fi
	local npw=$(func newPassword "$nname")
	#Empty password entered.
	if [ $npw = d41d8cd98f00b204e9800998ecf8427e ]
	then
		npw=$opw
	fi
	#Add it to profile.
	newUser "${nname}" ${npw} $1 ${id}
}

#Delete an existing user by usertype.
function delUsrByType()
{
	local -i usrtype id=$(func enterId $1 $2)
	local info=$(func accInfo $id)
	usrtype=$(func getUsrtype "${info}")
	#Check its usertype first.
	if [ ! ${usrtype} -eq $1 ]
	then
		error "Sorry, it's not a $(printUsrtype) account!"
		return 1
	fi
	#Now we can delete from profile.
	if delUser ${id}
	then
		echo "$(printUsrtype $1) ${id} successfully deleted."
		return 0
	else
		return 1
	fi
}

function lsUsrByType()
{
	local id
	if [ $# -gt 1 ]
	then
		#If id privided, list it only.
		id=$2
	else
		#If not, list them all.
		id='[0-9]*'
		echo "All $(printUsrtype) accounts are as follows: "
	fi
	#'/^.*:[0-9a-f]*:type:id$/'
	awk -F: '/^.*\:[0-9a-f]*:'"$1"':'"$id"'$/{print "\tUsername: "$1"\tUserid: "$4}' ${mgrprofile}
}

#Change the administrator account.
function chadmin()
{
	#Check the authority first.
	if ! isAdmin;
	then
		return 1
	fi
	#Ok, change it now.
	chUsrByType 0 0
}

#Similar with chadmin.
function chtch()
{
	if ! isAdmin
	then
		return 1
	fi
	chUsrByType 1 $1
}

#Similar with chadmin
function chstu()
{
	if ! isTch
	then
		return 1
	fi
	chUsrByType 2 $1
}

#Similar with chadmin
function addtch()
{
	if ! isAdmin;
	then
		return 1
	fi
	addUsrByType 1
}

#Similar with chadmin
function deltch()
{
	if ! isAdmin;
	then
		return 1
	fi
	delUsrByType 1 $1
}

#Similar with chadmin
function lstch()
{
	if ! isAdmin;
	then
		return 1
	fi
	lsUsrByType 1 $1
}

#Get course info by id
function crseInfo()
{
	emit $(cat ${crseprofile} | egrep ":$1\$")
}

#Check whether the course with id already exists.
function crseExist()
{
	#Try to get its info.
	local info=$(func crseInfo $1)
	#Check weather it is empty.
	if [ -z "${info}" ]
	then return 1
	else return 0
	fi
}

#Get the name of the course by info.
function crseName()
{
	emit $(echo -n ${@} | cut -d ':' -f1 )
}

#Change an existing course by id.
function chcrse()
{
	if ! isAdmin;
	then
		return 1
	fi
	#Get the id first.
	local -i id=$(func enterId 3 $1)
	if [ $id -eq 0 ]
	then
		#Wrong id.
		error "Error id inputed."
		return 1
	fi
	if ! crseExist $id
	then
		#Course does not exist.
		error "Sorry! Course$id does not exist."
		return 1
	fi
	#Get old and enter new information.
	local info=$(func crseInfo $id)
	local oname=$(func crseName $info)
	echo "Enter the new information. Empty for no changing."
	local nname=$(func enterName 3)
	#If empty, new name will be set by the old one.
	if [ -z "$nname" ]
	then
		nname=$oname
	fi
	#Replace information.
	info=$(echo -n "${info}" | sed 's/'"${oname}"'/'"${nname}"'/')
	#Delete and add it.
	sed -i '/'":$id\$"'/d' ${crseprofile}
	echo >>${crseprofile} "${info}"
}

function addcrse()
{
	#Check the authority first.
	if ! isAdmin;
	then
		return 1
	fi
	local name
	#Get the id.
	local -i id=$(func enterId 3 $1)
	if [ $id -eq 0 ]
	then
		error "Error id inputed."
		return 1
	fi
	#Check if the id conflicts.
	if crseExist $id
	then
		error "Sorry! There is already a course with the same id."
		return 1
	fi
	#Enter the name.
	name=$(func enterName 3)
	local crsedir=${crse}/Course${id}
	#Add to the the profile and new related directories and files.
	echo "${name}:${id}" >>${crseprofile}
	mkdir ${crsedir} 2>/dev/null
	touch ${crsedir}/${crsebind}
	#Add an empty line to the course info file.
	echo >>${crsedir}/${crseinfo}
	echo "Course ${name} added!"
	return 0
}

#List all the course current user is in.
function mycrse()
{
	#Get all course numbers.
	local crses=$(cat ${crseprofile} | cut -d: -f2)
	for i in $crses
	do
		#If in the course, then print its infromation.
		if incrse $i
		then
			lscrse $i
		fi
	done
	return 0
}

#List all the course
function lscrse()
{
	#No arguments privied, print all courses.
	if [ $# -eq 0 ]
	then 
		echo "All courses are as follows: "
		lscrse '[0-9]*'
	#One arguments privied, print it.
	elif [ $# -eq 1 ]
	then
		local id=$1
		awk -F: '/^.*\:'"$id"'$/{print "\tCourse name: "$1"\tCourse id: "$2}' ${crseprofile}
	#Loop to print all course whose id was privided.
	else
		for i in $@
		do
			if ! crseExist $i
			then
				error "Sorry, Course$i does not exit."
				continue
			fi
			lscrse $i
		done
	fi
}

#Delete an existing course.
function delcrse()
{
	#Check authority, get id and check the course's existence.
	if ! isAdmin;
	then
		return 1
	fi
	local -i id=$(func enterId 3 $1)
	if ! crseExist $id
	then
		error "Sorry, cannot find a course with id: $id."
		error "Please check the id again."
		return 1
	fi
	#Delete it from profile.
	sed -i '/'":${id}\$"'/d' ${crseprofile}
	#Delete its directorty.
	rm -rf ${crse}/Course${id}
	echo "Deleted Successfully."
	return $?
}

#Bind a user into a course by usertype.
#Usage bindUsr usrtype [crseid]
function bindUsr()
{
	#Get id and check it.
	local -i usrid id=$(func enterId 3 $2)
	if ! chkcrse $id
	then
		return 1
	fi
	local cbind=${crse}/Course${id}/${crsebind}
	#Loop to bind more at one time.
	while true
	do
		#Get account id and check it.
		usrid=$(func enterId $1)
		if [ ${usrid} -eq 0 ]
		then
			break
		fi
		if ! accExit ${usrid} $1
		then
			error "Sorry, $(printUsrtype $1) account with $usrid does not exist."
			continue
		fi
		if incrse ${id} ${usrid}
		then
			error "$(printUsrtype $1) $usrid already binded."
			continue
		fi
		#Add it to profile.
		echo >>${cbind} ${1}:${usrid}
	done
	echo "Done."
	return 0
}

#Unbind a user in the course by usertype.
#Usage unbindUsr usrtype [crseid]
function unbindUsr()
{
	#Get course id and check it.
	local -i usrid id=$(func enterId 3 $2)
	if ! chkcrse $id
	then
		return 1
	fi
	local cbind=${crse}/Course${id}/${crsebind}
	while true
	do
		#Get user id and check it.
		usrid=$(func enterId $1)
		if [ ${usrid} -eq 0 ]
		then
			break
		fi
		if ! accExit ${usrid} $1
		then
			error "Sorry, $(printUsrtype $1) account with $usrid does not exist."
			continue
		fi
		if ! incrse ${id} ${usrid}
		then
			error "User $usrid was not binded."
			continue
		fi
		#Delete it form bind file.
		sed -i '/^'"$1"':'"${usrid}\$"'/d' ${cbind}
	done
	echo "Done."
	return 0
}

#List all the bounded user.
function lsbind()
{
	#Get course id and check it.
	local -i id=$(func enterId 3 $1)
	local usrtype
	if ! chkcrse $id
	then
		return 1
	fi
	#Administrator account can list teachers and teacher can list students.
	if isAdmin ${logintype}
	then
		usrtype='1'
	elif isTch ${logintype}
	then
		usrtype='2'
	else
		error "Sorry, you cannot list binded users."
		return 1
	fi
	local cbind=${crse}/Course${id}/${crsebind}
	#Find all the ids can print their information.
	local usrs=$(cat ${cbind} | egrep "^${usrtype}:" | cut -d ':' -f2)
	local info name
	for i in $usrs
	do
		info=$(func accInfo $i)
		name=$(func getUsrname "$info")
		echo -e "\t$(printUsrtype $usrtype)\tname: $name\tid: $i"
	done
}

function bindtch()
{
	#Check authority and bind them.
	if ! isAdmin;
	then
		return 1
	fi
	bindUsr 1 $1
}

#Similar with bindtch.
function unbindtch()
{
	if ! isAdmin;
	then
		return 1
	fi
	unbindUsr 1 $1
}

#Similar with bindtch.
function addstu()
{
	if ! isTch;
	then
		return 1
	fi
	addUsrByType 2
}

#Similar with addtch.
function delstu()
{
	if ! isTch;
	then
		return 1
	fi
	delUsrByType 2 $1
}

#Similar with addtch.
function lsstu()
{
	if ! isTch;
	then
		return 1
	fi
	lsUsrByType 2 $1
}

#Similar with bindtch.
function bindstu()
{
	if ! isTch;
	then
		return 1
	fi
	bindUsr 2 $1
}

#Similar with bindtch.
function unbindstu()
{
	if ! isTch;
	then
		return 1
	fi
	unbindUsr 2 $1
}

#Check weather the user is in the course.
function incrse()
{
	local -i id
	#If the second id not privided, then check the current user.
	if [ $# -gt 1 ]
	then
		id=$2
	else
		id=${currentid}
	fi
	CourseBind=${crse}/Course$1/${crsebind}
	#Check if the querying result is empty.
	test -n "$(cat ${CourseBind} 2>/dev/null |egrep ":$id\$")"
	return $?
}

#Check the course's existence and weather the user is in course.
function chkcrse()
{
	if ! crseExist $1
	then
		error "Sorry, Course $1 does not exist."
		return 1
	fi
	if isAdmin ${logintype}
	then
		return 0
	fi
	if ! incrse $1
	then
		error "Sorry, you are not in this course."
		return 1
	fi
	return 0
}

#Edit course information.
function editinfo()
{
	#Check the authority, get course id and check it.
	if ! isTch
	then
		return 1
	fi
	local -i crseid
	crseid=$(func enterId 3 $1)
	if ! chkcrse $crseid; then return 1; fi
	#Get course info file and edit it by vim.
	CourseInfo=${crse}/Course${crseid}/${crseinfo}
	vim ${CourseInfo}
	echo "Done."
}

#List information of the course current user is in.
function crseinfo()
{
	#No arguments privided, list all the course.
	if [ $# -eq 0 ]
	then
		local crses=$(cat ${crseprofile} | cut -d: -f2)
		for i in $crses
		do
			if incrse $i
			then
				echo Course$i:
				crseinfo $i
			fi
		done
		return 0
	#Only one argument, list it only.
	elif [ $# -eq 1 ]
	then
		local -i crseid
		crseid=$(func enterId 3 $1)
		if ! chkcrse $crseid; then return 1; fi
		CourseInfo=${crse}/Course${crseid}/${crseinfo}
		nl -bt ${CourseInfo} 2>/dev/null
	#List all the argument one by one in loop.
	else
		for i in $@
		do
			if incrse $i
			then
				echo Course$i:
				crseinfo $i
			else
				error "Sorry, you are not in Course$i"
				continue
			fi
		done
	fi
}

#Arrange an assigment.
function arrasg()
{
	if ! isTch
	then
		return 1
	fi
	local -i crseid asgid
	crseid=$(func enterId 3 $1)
	if ! chkcrse $crseid; then return 1; fi
	asgid=$(func enterId 4 $2)
	asgdir=${crse}/Course${crseid}/Assignment${asgid}
	mkdir ${asgdir} 2>/dev/null
	#Get the assignment's directory and edit its detail file.
	echo >>${asgfir}/.detail
	vim ${asgdir}/.detail
	echo "Done."
	return
}

#Delete an assignment.
function delasg()
{
	if ! isTch
	then
		return 1
	fi
	local -i crseid asgid
	crseid=$(func enterId 3 $1)
	if ! chkcrse $crseid; then return 1; fi
	asgid=$(func enterId 4 $2)
	#Get its directory and delete it.
	asgdir=${crse}/Course${crseid}/Assignment${asgid}
	rm -rf ${asgdir}
	echo "Done."
}

#List the all the assignment in the course current user in.
function lsasg()
{
	if ! isTch
	then
		return 1
	fi
	local -i crseid asgid
	crseid=$(func enterId 3 $1)
	if ! chkcrse $crseid; then return 1; fi
	asg=$(ls -d ${crse}/Course${crseid}/Assignment* 2>/dev/null )
	for i in $asg
	do
		#Print its name.
		echo $i | cut -d '/' -f3
	done
	return
}

#Print the assignment status of all students.
function printasg()
{
	if ! isTch
	then
		return 1
	fi
	local -i crseid asgid stuid
	crseid=$(func enterId 3 $1)
	if ! chkcrse $crseid; then return 1; fi
	asgid=$(func enterId 4 $2)
	asgdir=${crse}/Course${crseid}/Assignment${asgid}
	if [ ! -d ${asgdir} ]
	then
		error "Sorry, assignment $asgid does not exist."
		return 1
	fi
	#Current student is privided.
	if [ $# -eq 3 ]
	then
		if ! incrse $crseid $3
		then
			error "Sorry! Student$3 is not in Course$crseid."
			return 1
		fi
		if [ -f ${asgdir}/$3 ]
		then
			echo Student $3:
			nl -bt ${asgdir}/$3
		else
			echo Student $3 did not hand in yet.
			return 1
		fi
	else
		#List all the student and print one by one.
		local cbind=${crse}/Course${crseid}/${crsebind}
		local usrs=$(cat ${cbind}|egrep "^2:"|cut -d: -f2)
		for i in $usrs
		do
			printasg $crseid $asgid $i
		done
	fi
	return
}

#Edit the assignement to finish it.
function editasg()
{
	#Check authority, get ids, check them and edit the assignment file.
	if ! isStu
	then
		return 1
	fi
	local -i crseid asgid
	crseid=$(func enterId 3 $1)
	if ! chkcrse ${crseid}; then return 1; fi
	asgid=$(func enterId 4 $2)
	asgdir=${crse}/Course${crseid}/Assignment${asgid}
	if [ ! -d ${asgdir} ]
	then
		error "Sorry, no this Assignment."
		return 1
	fi
	vim ${asgdir}/${currentid}
	echo "Done."
}

#Check all the status of the assignment current student have to finish.
function checkasg()
{
	if ! isStu
	then
		return 1
	fi
	#List all the courses.
	local crses=$(cat $crseprofile | cut -d ':' -f2 )
	for crseid in $crses
	do
		if ! incrse ${crseid}; then continue; fi
		crsedir=${crse}/Course${crseid}
		#List all the assignment.
		asg=$(ls -d ${crsedir}/Assignment* 2>/dev/null )
		echo Course${crseid}:
		for i in $asg
		do
			#Print its status.
			echo -ne "\t"
			echo -n ${i} | awk -F/ '{printf("%s",$3);}'
			if [ -f ${i}/${currentid} ]
			then
				echo -e "\tDone."
			else
				echo -e  "\tNot done."
			fi
		done
	done
	return 0
}

#Change password for the current user.
function passwd()
{
	echo "You are about to change your password!"
	local oldpw=$(func inputPasswd "your old one")
	local info=$(func accInfo ${currentid})
	local oldpwc=$(func getPasswd ${info})
	#Check the password first.
	if [ ! ${oldpwc} = ${oldpw} ]
	then
		error "Sorry, wrong password!"
		return 1
	fi
	local newpw=$(func newPassword "your new one")
	#Replace the password code in info string.
	info=$(echo -n "${info}" |sed 's/'"${oldpwc}"'/'"${newpw}"'/' )
	#Delete it form profile and then add the new one.
	sed -i '/'":${currentid}\$"'/d' ${mgrprofile}
	echo >>${mgrprofile} "${info}"
	echo Done.
	#Have to login again.
	login ${currentid}
}

#List all the assignment information.
function asginfo()
{
	#If no arguments, list all courses and all assignment.
	if [ $# -eq 0 ]
	then
		#List all courses and print then one by one.
		local crses=$(cat $crseprofile | cut -d ':' -f2 )
		for i in $crses
		do
			if ! incrse $i
			then
				continue
			fi
			asginfo $i
		done
	#If one argument privided, list all assignment in it.
	elif [ $# -eq 1 ]
	then
		if ! incrse $1
		then
			error "Sorry, you are not in Course$1."
			return 1
		fi
		#List all assignments and print them one by one.
		local asg=$(ls -d ${crse}/Course$1/Assignment* 2>/dev/null )
		for i in $asg
		do
			asginfo $1 $(echo -n $i | cut -d/ -f3 | sed 's/Assignment//')
		done
	#If two argument privied, list the certain assignment of the certain course.
	elif [ $# -eq 2 ]
	then
		#Check the ids and print the detail.
		if ! incrse $1
		then
			error "Sorry, you are not in Course$1"
			return 1
		fi
		local asgdir=${crse}/Course${1}/Assignment${2}
		if [ ! -d ${asgdir} ]
		then
			error "Sorry, assignment $2 does not exist."
			return 1
		fi
		echo Course$1 Assignment$2:
		nl -bt ${asgdir}/.detail
	fi
	return 0
}

initialize
login $1
getcmd
