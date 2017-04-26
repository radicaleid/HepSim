# HepSim/analysis/cpp-lcio 

This example shows how to process HepSim LCIO files on a Linux/bash system, build anti-KT jets,
and fill ROOT histograms

1) Install LCIO (https://github.com/iLCSoft/LCIO) and ROOT


   wget https://github.com/iLCSoft/LCIO/archive/v02-06.zip  
   unzip v02-06.zip  -d lcio
   export LCIO_DIR=`pwd`/lcio 
   cd lcio/LCIO-02-06
   rm -rf build
   mkdir build
   cd build
   cmake -DINSTALL_DOC=OFF -DCMAKE_INSTALL_PREFIX=$LCIO_DIR -DCMAKE_SKIP_RPATH=1 ..
   make install -j4
   cd ../../.. 


2) Check the installation. The variables: 

   echo $LCIO_DIR
   echo $ROOTSYS

 should return the installation path. 

3) Compile: 

   make

4) Download ProMC files from HepSim and put them to the "data" directory.
   Use hs-tools as:
   
   gev35ep_lepto6_dis1q2%rfull055 data

   (see HepSim documentation)

5) Process all files inside "data"

 
   ./example

6) Loook at the output root file  

    output.root

  with jet pT and Eta

----------  
S.Chekanov (ANL) 
