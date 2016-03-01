#!/bin/bash

#### echo "XMPP sending is DISABLED, not sending (in $0)" ; exit 0

to=$1
shift
#echo " $@" | sendxmpp -v $to
echo " $@" | sendxmpp  $to
