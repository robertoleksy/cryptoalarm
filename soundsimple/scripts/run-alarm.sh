#!/bin/bash 
#set -x

out_dir="out_directory" 
instance="my_instance" 
key="$out_dir/$instance-key1.pub"

echo $out_dir 
echo $instance 
echo "$key"


cd $HOME/chainsign
if [ "$(pidof chainsign)" ]; then
    echo "ok chainsign is running" 
else
    echo "starting chainsign"
    ./chainsign --daemon $instance $out_dir &   
    ./chainsign --client start   
    sleep 5 
fi 

#$HOME/PyMailSender/sendmail.py PUBLIC_KEY "Alarm is running" "$HOME/chainsign/$key"

#cd $HOME/motion-alert/soundsimple/src/
cd $HOME/cryptoalarm/soundsimple/build/src
./simplesound -a
