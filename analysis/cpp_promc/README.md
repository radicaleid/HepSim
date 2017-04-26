# HepSim/analysis/cpp-promc

This example shows how to process HepSim ProMC files on a Linux/bash system, build anti-KT jets,
and fill ROOT histograms

1) Install ProMC (http://atlaswww.hep.anl.gov/asc/promc/) and ROOT
2) Check the installation. The variables: 

   echo $PROMC
   echo $ROOTSYS

 should return the installation path. 

3) Compile: 

   make

4) Download ProMC files from HepSim and put them to the "data" directory.
   Use hs-tools as:
   
   hs-get tev100_higgs_ttbar_mg5 data

   (see HepSim documentation)

5) Process all files inside "data"

 
   ./example

6) Loook at the output root file  

    output.root

  with jet pT and Eta

----------  
S.Chekanov (ANL) 
