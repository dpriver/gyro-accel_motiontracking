#/bin/sh -f


#install a newer avr-gcc version
#wget ftp://ftp.irisa.fr/pub/mirrors/gcc.gnu.org/gcc/releases/gcc-4.9.0/gcc-4.9.0.tar.bz2
#bunzip2 -c gcc-4.9.0.tar.bz2 | tar xf -
#cd gcc-4.9.0
#mkdir obj-avr
#cd obj-avr
#../configure --prefix=$PREFIX --target=avr --enable-languages=c,c++ --disable-nls --disable-libssp --with-dwarf2

#make
#make install

# install a newer cmake version
if ( test "`uname -s`" = "Darwin" )
then
  echo "Already installed a valid version"
else
  #install a newer cmake since at this time Travis only has version 2.8.7
  echo "yes" | sudo add-apt-repository ppa:kalakris/cmake
  sudo apt-get update -qq
  sudo apt-get install cmake
fi
