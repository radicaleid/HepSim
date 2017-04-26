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
// promc
#include "ProMC.pb.h"
#include "ProMCBook.h"


using namespace std;
using namespace promc;

// find all files inside a directory
std::vector<std::string> open(std::string path = ".") {
  std::vector<std::string> files;
  if (stat(path.c_str(), &sb) == 0 && S_ISDIR(sb.st_mode)) { 
    DIR*    dir;
    dirent* pdir;
    dir = opendir(path.c_str());
    while (pdir = readdir(dir)) {
        string rfile=path + "/" + pdir->d_name;
        if (rfile.find(".promc") != std::string::npos) 
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

  for(unsigned int m=0; m < files.size(); m++){
  string Rfile=files[m]; 
  ProMCBook*  epbook = new ProMCBook(Rfile.c_str(),"r");

   cout << "\n\n Start to read.." << endl;
  // get the version number
  int  h=epbook->getVersion();
  if (m==0) cout << "Version = " << h << endl;
  // get the description of this file
  string d=epbook->getDescription();
  if (m==0) cout << "Description = " << d << endl;
  int  nev=epbook->getEvents();
  cout << "Events = " << nev  << endl;
  // get the header file with units, cross sections etc.
  ProMCHeader header = epbook->getHeader();

  // here are the units
  double kEV=(double)(header.momentumunit());
  //double kLe=(double)(header.lengthunit());


  // loop over all events
  for (int nn=0; nn<nev; nn++){
  if (epbook->next() !=0) continue;
  ProMCEvent eve = epbook->get();

  // get truth information 
  ProMCEvent_Particles  *pa=eve.mutable_particles();
  h_debug->Fill(1.0);
  ntot++;

  double xlumi=0;
  if (m>0) {
     xlumi=(double)ntot/cross; // lumi so far 
     h_debug->Fill(2.0);
   }

   if (ntot%1000==0)
          cout <<  " # Events=" << ntot  << " X-cross="<< cross << " pb" << " Lumi so far=" << xlumi/1000.0 << " fb-1" << endl;


   vector<fjcore::PseudoJet> avec;

   // fill stable and no neutrino 
   for (int j=0; j<pa->pdg_id_size(); j++){
    if (pa->status(j)!=1) continue;
    if (abs(pa->pdg_id(j)==12) || abs(pa->pdg_id(j))==14 || abs(pa->pdg_id(j))==16 ) continue;
    double px= pa->px(j)/kEV;
    double py= pa->py(j)/kEV;
    double pz= pa->pz(j)/kEV;
    double e= pa->energy(j)/kEV;
    double pt=sqrt(px*px+py*py); 
    double eta=-log(tan(atan2(pt,(double)pz)/2));
    if ( pt < 0.4)                   continue;
    if ( fabs(eta)> 3.0 )            continue;
    avec.push_back( fjcore::PseudoJet(px,py,pz,e) );
   }


   fjcore::ClusterSequence clust_seq(avec, jet_def);
   vector<fjcore::PseudoJet> inclusive_jets = clust_seq.inclusive_jets(50.0);
   // perform exclusive jets
   // vector<PseudoJet> inclusive_jets = clust_seq.exclusive_jets_up_to(2);
   // cout << inclusive_jets.size() << endl;

   // sort in PT
   vector<fjcore::PseudoJet> sorted_jets = sorted_by_pt(inclusive_jets);
   // cout << sorted_jets.size() << endl;

 for (unsigned int k = 0; k<sorted_jets.size(); k++) {
              double eta=sorted_jets[k].pseudorapidity();
              // double phi=sorted_jets[k].phi();
              double pt = sorted_jets[k].perp();
              //double e = sorted_jets[k].e();
              h_pt->Fill(pt);
              h_eta->Fill(eta);
         } // end loop


  } // end event loop 

  
  ProMCStat stat = epbook->getStatistics();
  cross=stat.cross_section_accumulated();
  epbook->close(); // close
  nfiles++;
  xcross=xcross+cross;


  } // end loop over all files



   xcross=xcross/(double)nfiles; // average cross for all files
   cout << "Total events=" << ntot << endl;
   cout << "Total files=" << nfiles << endl;
   cout << "Average cross section for all files = " << xcross << " pb"<< endl;
   double width=h_pt->GetBinWidth(1);
   double lumi=(double)ntot/xcross;
   cout << "Lumi for all files  =" << lumi << " pb-1" << endl;
   double norm=width*lumi;
   h_pt->Scale(1.0/norm);

   h_cross->Fill(0.0,(double)xcross); // 1 
   h_cross->Fill(1.0,(double)ntot); // 2 
   h_cross->Fill(2.0,(double)lumi); // 3 
   h_cross->Fill(3.0,(double)nfiles);
   cout << "Nr of selected jets=" << nselect << endl;
 
/*
// calculate cross sections
   TAxis *xaxis = h_pt->GetXaxis(); 
   xaxis->SetTitle("p_{T}(jet) GeV");
   TAxis *yaxis = h_pt->GetYaxis();
   yaxis->SetTitle("d #sigma / dp_{T} [pb / GeV]");

    norm=lumi*(h_pt->GetBinWidth(1));
    h_pt->Scale(1.0/norm); 
    TAxis *xaxis1 = h_pt->GetXaxis();
    xaxis1->SetTitle("p_{T}(jet) GeV");
    TAxis *yaxis1 = h_pt->GetYaxis();
    yaxis1->SetTitle("d #sigma / dp_{T} [pb / GeV]");
*/
 
   //m_ntuple->Fill();
   RootFile->Write();
   RootFile->Print();
   RootFile->Close();

   cout << "Writing ROOT file "+ outputfile << endl;

    return 0;
}
