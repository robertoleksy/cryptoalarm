#!/bin/bash
date=$(date +%Y-%m-%d_%H:%M:%S%z)
start_dir=`pwd`
rm /dev/shm/chainsign /dev/shm/fifo

function ask() {
        REPLY=$1
        if ! [[ $REPLY =~ ^[Yy]$ ]]; then
                echo "Aborting."
                exit 0
        fi
}

function start_chainsign_daemon() {
	echo "[INFO] Starting chainsign daemon"
	if ! [ -d ../chainsign ]; then
		echo "[ERROR] chainsign directory doesn't exist"
		echo "Run ./do script or(and) check directory"
	fi

	cd ../chainsign

	if ! [ -f chainsign ]; then
		cmake . && make
	fi

	./chainsign --daemon &
	cd $start_dir
}


if [[ $0 != "./motion-start.sh" ]]; then 
	echo "[WARN] This script should be called using command: ./motion-start"
	echo "[WARN] Continue? "
	read yn
	ask $yn
fi

if [ -d ~/keys ]; then
	files=`ls ~/keys/ | wc -l`
	if [[ $files -gt 0 ]] ; then
		echo "****ERROR****"
		echo "~/keys dir is not empty"
		exit
	fi
fi

if [ -d ~/cryptoalarm/video/rec ]; then
	files=`ls ~/cryptoalarm/video/rec | wc -l`
	if [[ $files -gt 0 ]] ; then
	        echo "****ERROR****"
	        echo "~/cryptoalarm/video/rec dir is not empty"
        	exit
	fi
fi

if [[ $1 != "--dry-run" ]]; then
	start_chainsign_daemon 

	# wait for file
	while [ ! -f ~/keys/key_1.pub ]
	do
		sleep 1
	done
	sha512sum ~/keys/key_1.pub > ~/keys/key_1.pub.sha512
	echo "----------------------------------"
	echo "SAVE THIS CONTROL SUM"
	sha512sum ~/keys/key_1.pub
	echo "----------------------------------"

	echo "guarding-since-$date[$1]" > ~/stat

	./lib_sendxmpp.sh "rfree.mobile@jit.si" "starting..."
	#sleep 10
	./lib_sendxmpp.sh "rfree.mobile@jit.si" "starting..."
	#sleep 60

	./lib_sendxmpp.sh "rfree.mobile@jit.si" "starting...3"
	#sleep 5
	./lib_sendxmpp.sh "rfree.mobile@jit.si" "starting...2"
	#sleep 5
	./lib_sendxmpp.sh "rfree.mobile@jit.si" "starting...1"
	#sleep 5
	./lib_sendxmpp.sh "rfree.mobile@jit.si" "starting...GO"
	#sleep 5
fi

debian_ver=`cat /etc/debian_version`
echo "debian ver = $debian_ver"
if [[ $debian_ver < 8 ]]; then
	echo "OLD VER"
	motion -c motion.conf-old2
else
	echo "NEW ver"
	motion
fi

if [[ $1 != "--dry-run" ]]; then
	./lib_sendxmpp.sh "rfree.mobile@jit.si" "end of program"
fi
