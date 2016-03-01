#!/bin/bash

date=$(date +%Y-%m-%d_%H:%M:%S%z)
event=$1
mark=$2
tag="msg[$HOSTNAME $USER $date]"
msg="$tag $event"
log=~/alert.log

if [ -r ./var/mute ] ; then
	echo -- "Event is MUTED." "$@"
	exit 1
fi

echo "" >> $log
echo "$tag - START SCRIPT for event=$event" >> $log

function announce() {
	msg="$@"
	echo " $msg" >> $log
	
	#./lib_sendirc.sh "var/chan1" "$msg" &

	pids=()
	./lib_sendxmpp.sh "dorota@meshnet.pl" "$msg"
#	pids+=($!)
	#timeout 20s ./lib_sendxmpp.sh "rfree@jit.si" "$msg" 
#	pids+=($!)
 #	timeout 20s ./lib_sendxmpp.sh "rfree.other@jit.si" "$msg" 
#	pids+=($!)
 #	timeout 20s ./lib_sendxmpp.sh "vyrly@jabber.org" "$msg" 
#	pids+=($!)
 	#timeout 20s ./lib_sendxmpp.sh "dedee.htc@jit.si" "$msg" 
#	pids+=($!)

	echo " $tag waiting pids: ${pids[@]} START" >> $log
	#wait "${pids[@]}"
	echo " $tag waiting pids: ${pids[@]} DONE." >> $log
}

announce "$msg ($mark)"

base="rec"
file1=$( ls -rt1 $base/*jpg | tail -n 1 )
file2=$( ls -rt1 $base/*avi | tail -n 1 )
file1info=$( sha1sum "$file1" )
file2info=$( sha1sum "$file2" )

msg2="$tag $event details: $file1info $file2info"

announce "$msg2"

echo "$tag - END SCRIPT" >> $log


