/***************************************************************************
 *  How to use ProMC files from HepSim, and how to build anti-KT jets 
 *  S.Chekanov (ANL) chekanov@anl.gov
 *  A library for HEP events storage and processing based on Google's PB   
 *  The project web site: http://atlaswww.hep.anl.gov/hepsim/
****************************************************************************/

#include<iostream>
#include<fstream>
#include<stdlib.h>

// check directory
#include <sys/stat.h>
#include <TROOT.h>
#include <TFile.h>
#include <TH1D.h>
#include "TMath.h"
#include"time.h"
#include <dirent.h>
#include <string>
#include <vector>
#include <map>
struct stat sb;

// fastjet
#include "fjcore.hh"

// lcio
#include "lcio.h"
#include <stdio.h>
#include "IO/LCReader.h"
#include "IMPL/LCTOOLS.h"
#include "EVENT/LCEvent.h"
#include "EVENT/LCRunHeader.h"
#include "EVENT/ReconstructedParticle.h"
#include "IOIMPL/LCFactory.h"
#include "EVENT/MCParticle.h"
#include "EVENT/LCCollection.h"
#include "IMPL/LCEventImpl.h"
#include "UTIL/LCTOOLS.h"
#include "UTIL/Operators.h"
#include "UTIL/LCIterator.h"
#include "IMPL/LCCollectionVec.h"
#include "IMPL/MCParticleImpl.h"
#include "EVENT/MCParticle.h"
#include "EVENT/ReconstructedParticle.h"
#include "EVENT/Track.h"
#include "EVENT/Cluster.h"




using namespace std;


// find all files inside a directory
std::vector<std::string> open(std::string path = ".") {
  std::vector<std::string> files;
  if (stat(path.c_str(), &sb) == 0 && S_ISDIR(sb.st_mode)) { 
    DIR*    dir;
    dirent* pdir;
    dir = opendir(path.c_str());
    while (pdir = readdir(dir)) {
        string rfile=path + "/" + pdir->d_name;
        if (rfile.find(".slcio") != std::string::npos) 
             files.push_back(rfile); 
    }
    } else {
        cout << "Directory " << path << " does not exist!!" << endl;
        std::exit(0); 
        }
    return files;
}


// main example
int main(int argc, char **argv)
{
  // total events
  int ntot=0; // total events
  int nfiles=0; // total files


  string dir="./data/";
  std::vector<std::string> files = open(dir);

  double cross=0;
  double xcross=0;
  int nselect=0;

  string outputfile="output.root";
  cout << "\n -> Output file is =" << outputfile << endl;
  TFile * RootFile = new TFile(outputfile.c_str(), "RECREATE", "Histogram file");
//  RootFile->SetCompressionLevel(0);

  TH1D * h_debug = new TH1D("debug", "debug", 10, 0, 10.);
  TH1D * h_cross = new TH1D("cross", "cross,events,lumi", 5, 0, 5.);
  TH1D * h_pt = new TH1D("jet_pt", "pt",100,0,1000);
  TH1D * h_eta = new TH1D("jet_eta", "eta", 40, -10, 10);

  // fastjet
  double R = 0.6;
  fjcore::JetDefinition jet_def(fjcore::antikt_algorithm, R);

  //JetDefinition jet_def(fastjet::kt_algorithm, Rparam, strategy);
  //JetDefinition jet_def(fastjet::cambridge_algorithm, Rparam, strategy);
  //JetDefinition jet_def(fastjet::antikt_algorithm, Rparam, strategy);


  int nEvents=0;
  int MaxEvents=1000;

  for(unsigned int m=0; m < files.size(); m++){
  string Rfile=files[m]; 

  IO::LCReader* lcReader = IOIMPL::LCFactory::getInstance()->createLCReader() ;
  lcReader->open( Rfile.c_str() ) ;


  EVENT::LCEvent* evt=0 ;

  if (nEvents>MaxEvents) break;

 //----------- the event loop -----------
  while( (evt = lcReader->readNextEvent()) != 0 ) {
    if (nEvents==0) UTIL::LCTOOLS::dumpEvent( evt ) ;

    // UTIL::LCTOOLS::dumpEvent( evt ) ;

    nEvents ++ ;
    if (nEvents%100==0) cout <<  " # Events=" << nEvents << endl;


    // get truth
    std::string mcpName("MCParticle") ;
    IMPL::LCCollectionVec* col = (IMPL::LCCollectionVec*) evt->getCollection( mcpName  ) ;
    int nMCP = col->getNumberOfElements() ;
    for(int i=0 ; i<nMCP ; ++i){
      EVENT::MCParticle* mcp =  (EVENT::MCParticle*) col->getElementAt(i) ;
      float px = mcp->getMomentum()[0];
      float py = mcp->getMomentum()[1];
      float pz = mcp->getMomentum()[2];
      float m = mcp->getMomentum()[3];
     }


// clusters
  IMPL::LCCollectionVec* col5 = (IMPL::LCCollectionVec*) evt->getCollection("ReconClusters") ;
  int nCL = col5->getNumberOfElements() ;
   for(int i=0 ; i<nCL ; ++i){
    EVENT::Cluster* mcp =  (EVENT::Cluster*) col5->getElementAt(i) ;
    const float* pos= mcp->getPosition();
    float x=pos[0];
    float y=pos[1];
    float z=pos[2];
    double e = mcp->getEnergy();
    double _tmp = std::sqrt(x*x + y*y + z*z);
    double px =  e*x/_tmp;
    double py =  e*y/_tmp;
    double pz =  e*z/_tmp;
    }

   } // end event loop


   }

  cout << "Writing ROOT file "+ outputfile << endl;

    return 0;
}
