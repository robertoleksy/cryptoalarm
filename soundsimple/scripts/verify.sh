#!/bin/bash
for f in  $(find ~/Alarm_data/ -name '*.sig*' -or -name '*.wav' -or -name '*.pub')
	do
	cp $f ~/Alarm_data/
done

cd $HOME/cryptoalarm/soundsimple/build/src/
#cp ~/Alarm_data/key_1.pub .

for f in  ~/Alarm_data/*.wav
	do
	./simplesound -v "$f"
	if [ "$?" -ne "1" ] ; then
		echo "*********VERIFICATION ERROR*********"
		break
	fi
done
