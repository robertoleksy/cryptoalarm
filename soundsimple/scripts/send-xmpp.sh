#!/bin/bash 


# usage ./send <message> 

message=$1 

opt="-t"

m="$1 alertalert2-snd"
#echo "$1""$m"  | sendxmpp $opt alarm happuri@mempo.org
#echo "$1""$m"  | sendxmpp $opt alarm abbys@dukgo.com
#echo "$1""$m"  | sendxmpp $opt alarm rfree.m@mempo.org
#echo "$1""$m"  | sendxmpp $opt alarm rfree.w@mempo.org
#echo "$m"  | sendxmpp $opt alarm rfree@mempo.org
#echo "$1""$m"  | sendxmpp $opt alarm rfree.mobile@jit.si

echo "$1""$m"  | sendxmpp $opt alarm dorota@meshnet.pl
