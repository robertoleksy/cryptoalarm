
### INSTALL ###

git submodule update
git submodule init




### Technical plan ###

Work in progress.


```

~/.config/cryptoalarm.conf
install_dir_var=~/cryptoalarm-var/
install_dir_bin=~/cryptoalarm-bin/




Usage: Sound system

 ~/motion-alert/soundsimple/scripts/run-alarm.sh


~/cryptoalarm-var/sound/
~/cryptoalarm-var/video/
~/cryptoalarm-var/text/


~/cryptoalarm/sound/
        key1.pub
        key1.pub.sha512
        key_33.prv
        2016-02-23
                12-41-07
                        2016-02-23_12-41-07.tar.gz
                        2016-02-23_12-41-07.wav
                        2016-02-23_12-41-07.wav.sig
                        key_32.pub
                        key_32.pub.sig
                12-41-30
                				[... wav wav.sig .tar.gz]
                        key_33.pub
                        key_33.pub.sig



Video
~/keys/
        key1.pub
        key_1.pub.sha512
        key_2.pub
        key_2.pub.sig
~/motion-alert/video/rec
        20160223-15_20_52-cam1-01-03-movie.avi
        20160223-15_20_52-cam1-01-03-movie.avi.sig



```


# Soundsimple
### Dependencies
Needed packages for distributions based on Debian. Tested on Debian (Wheezy and Jessie) and Linux Mint 17.1 Rebecca.
``
sudo aptitude install libxcb-image0-dev libx11-xcb-dev libopenal-dev libflac++-dev libvorbis-dev libalut-dev libudev-dev libjpeg-dev libxcb-randr0-dev libfreetype6-dev libfftw3-3 mffm-fftw1  fftw2  fftw3 libfftw3-dev libcrypto++-dev g++ cmake libqt4-opengl-dev libssl-dev libboost-all-dev
`` 

### Building 
Run script
``
./do-build
``

### Usage 
After build:
``
~/motion-alert/soundsimple/scripts/run-alarm.sh
``

###  Verify
1) Compare your key and ~/Alarm_data/key_1.pub file
2) run
``
~/motin-alert/soundsimple/scripts/verify.sh
``
3) rm -rf ~/Alarm_data and run again
``
~/motion-alert/soundsimple/scripts/run-alarm.sh
``
or just run above script for continue chain

All above commands run as normal user

### Testing
building working on:
    
    Distributor ID: Ubuntu
    Description:    Ubuntu 15.04
    Release:    15.04
    Codename:   vivid
