# telldus
This is a fork of the upstream telldus repository.

The reason I forked it is I wanted to use my tellstick duo on raspbian.
There is an upstream deb repository, but I want to be able to build from source.

# Using telldus on raspbian
This recipe creates a native package. Log on to your raspbian system to compile
 - clone this repo (duh)
 - install tools for building
 > $sudo apt install build-essential dpkg-dev
 - install the dependencies (or wait for dpkg-buildpackage to tell you which are missing):
 > $sudo apt install libftdi-dev help2man cmake libconfuse-dev doxygen
 - go to the telldus-core directory:
 > $cd telldus-core
 - create the deb package (being root is not neccessary and not recommended):
 > $dpkg-buildpackage -b -uc
 - if everything goes well, you now have some .deb in the parent directory
 > $sudo dpkg -i ../*.deb
 
 
