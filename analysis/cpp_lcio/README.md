# HepSim/analysis/cpp-lcio 

This example shows how to process LCIO files from HepSim on a Linux/bash system, build anti-KT jets using FastJet, and fill ROOT histograms. 

 1. Install LCIO (https://github.com/iLCSoft/LCIO) and ROOT packages. If ROOT is already installed, the LCIO package can be installed as: 

```
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
```

For simulations created using FPadSim 4.0 (Aug. 2017), we use the LCIO library v02.07.05.

Check your installation. The variables LCIO_DIR and ROOTSYS should return the installation paths. Check this as: 

```
   echo $LCIO_DIR
   echo $ROOTSYS
```

 3. Compile this example package as "make" 
 
 4. Download LCIO files that correspond to a give tag (rfullXXX) from HepSim and put them to the "data" directory. Use the hs-tools package: 

```   
   hs-get gev35ep_lepto6_dis1q2%rfull055 data
```

  (see the HepSim documentation http://atlaswww.hep.anl.gov/hepsim/doc)

 5. Process all files inside "data"

```
    export LD_LIBRARY_PATH=$LD_LIBRARY_PATH:$LCIO_DIR/lib
   ./example
```

 6. Loook at the output root file "output.root". It has histograms with jet pT and Eta.  


S.Chekanov (ANL) 
