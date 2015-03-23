# Introduction #

This is the procedure for compiling and running Geant4 spacal code on lxplus machines with 64-bit SLC6.

# Details #
1) All the necessary libraries and compiler versions are already installed on lxplus machines running 64-bit SLC6, thus after logging on, like this:
```
   ssh lxplus6.cern.ch
```
you just need to source the following scripts and to set the following variables to setup an appropriate environment:
```script

export SCRAM_ARCH=x86_64-slc6-gcc46
source /afs/cern.ch/sw/lcg/contrib/gcc/4.6/x86_64-slc6/setup.sh
source /afs/cern.ch/sw/lcg/external/geant4/9.6.p03/x86_64-slc6-gcc46-opt/CMake-setup.sh
export CXX=`which g++`
export CC=`which gcc`
```

You can also put those commands in a script and launch the script on startup (i.e. calling it in .bashrc)


2) Make a dir where you want to put the code, say G4\_spacal
```
   mkdir G4_spacal
   cd G4_spacal
```

3) Get the code
```
   svn checkout http://spacal.googlecode.com/svn/trunk/ spacalCode
```
this will download the code and place it in a dir **spacalCode**

4) Make a build directory
```
   mkdir build
   cd build
   mkdir spacalCode
   cd spacalCode
```

5) Build using cmake
```
   cmake -DGeant4_DIR=/afs/cern.ch/sw/lcg/external/geant4/9.6.p03/x86_64-slc6-gcc46-opt/lib64/Geant4-9.6.3 ../../spacalCode
```
CMake will now run to configure the build and generate Makefiles.

Note1: If you do not want to type this long path,  in your .bashrc  set a variable, say g4Dir:
```
   export g4Dir="/afs/cern.ch/sw/lcg/external/geant4/9.6.p03/x86_64-slc6-gcc46-opt/lib64/Geant4-9.6.3"
```
than you can run cmake as:
```
   cmake -DGeant4_DIR=$g4Dir /path_to/your_source_code/
```

6) When cmake finishes we can now build by simply running make:

```
   make
```
or if your machine has more than 1 core processor
```
   make -jN
```
where N is the number of core processors

7) now you are ready to run
```
    ./SPc -m beam_circ.mac
```

This will run the macro beam\_circ.mac.