#define delAna_cxx
// The class definition in delAna.h has been generated automatically
// by the ROOT utility TTree::MakeSelector(). This class is derived
// from the ROOT class TSelector. For more information on the TSelector
// framework see $ROOTSYS/README/README.SELECTOR or the ROOT User Manual.


// The following methods are defined in this file:
//    Begin():        called every time a loop on the tree starts,
//                    a convenient place to create your histograms.
//    SlaveBegin():   called after Begin(), when on PROOF called only on the
//                    slave servers.
//    Process():      called for each event, in this function you decide what
//                    to read and fill your histograms.
//    SlaveTerminate: called at the end of the loop on the tree, when on PROOF
//                    called only on the slave servers.
//    Terminate():    called at the end of the loop on the tree,
//                    a convenient place to draw/fit your histograms.
//
// To use this file, try the following session on your Tree T:
//
// root> T->Process("delAna.C")
// root> T->Process("delAna.C","some options")
// root> T->Process("delAna.C+")
//


#include "delAna.h"
#include <TH2.h>
#include <TStyle.h>

//Additional headers:
#include <iomanip>
#include "BookHistograms.h"
#include "Functions.h"

void delAna::Begin(TTree * /*tree*/)
{
  // The Begin() function is called at the start of the query.
  // When running with PROOF Begin() is only called on the client.
  // The tree argument is deprecated (on PROOF 0 is passed).

  //TString option = GetOption();
}

void delAna::SlaveBegin(TTree * /*tree*/)
{
  // The SlaveBegin() function is called after the Begin() function.
  // When running with PROOF SlaveBegin() is called on each slave server.
  // The tree argument is deprecated (on PROOF 0 is passed).

  time(&start);
  cout<<"\nn-events \t time_taken (sec)"<<endl;

  nEvtTotal   = 0;
  nEvtRan     = 0;
  nEvtTrigger = 0;
  nEvtPass    = 0;

  _HstFile = new TFile(_HstFileName,"recreate");
  BookHistograms();
  
  //TString option = GetOption();

}

void delAna::SlaveTerminate()
{
  // The SlaveTerminate() function is called after all entries or objects
  // have been processed. When running with PROOF SlaveTerminate() is called
  // on each slave server.

  //Create output files:
  _HstFile->Write();
  _HstFile->Close();
  ofstream fout(_SumFileName);

  cout<<"---------------------------------------------"<<endl;
  cout<<"Summary:"                                     <<endl;
  cout<<"nEvtTotal = "   <<nEvtTotal                   <<endl;
  cout<<"nEvtRan = "     <<nEvtRan                     <<endl;
  cout<<"nEvtTrigger = " <<nEvtTrigger                 <<endl;
  cout<<"nEvtPass = "    <<nEvtPass                    <<endl;
  cout<<"---------------------------------------------"<<endl;

  time(&end);
  double time_taken = double(end-start);
  cout<<"\nTime taken by the programe is= "<<fixed<<time_taken<<setprecision(5);
  cout<<" sec \n"<<endl;

  //Write down a summary to the text file:
  fout<<"---------------------------------------------"<<endl;
  fout<<"Summary:"                                     <<endl;
  fout<<"nEvtTotal = "   <<nEvtTotal                   <<endl;
  fout<<"nEvtRan = "     <<nEvtRan                     <<endl;
  fout<<"nEvtTrigger = " <<nEvtTrigger                 <<endl;
  fout<<"nEvtPass = "    <<nEvtPass                    <<endl;
  fout<<"---------------------------------------------"<<endl;
  
}

void delAna::Terminate()
{
  // The Terminate() function is the last function to be called during
  // a query. It always runs on the client, it can be used to present
  // the results graphically or save the results to file.

}

Bool_t delAna::Process(Long64_t entry)
{
  // The Process() function is called for each entry in the tree (or possibly
  // keyed object in the case of PROOF) to be processed. The entry argument
  // specifies which entry in the currently loaded tree is to be processed.
  // When processing keyed objects with PROOF, the object is already loaded
  // and is available via the fObject pointer.
  //
  // This function should contain the \"body\" of the analysis. It can contain
  // simple or elaborate selection criteria, run algorithms on the data
  // of the event and typically fill histograms.
  //
  // The processing can be stopped by calling Abort().
  //
  // Use fStatus to set the return value of TTree::Process().
  //
  // The return value is currently not used.

  fReader.SetLocalEntry(entry);

  time(&buffer);
  double time_taken_so_far = double(buffer-start);
  if(_verbosity==0 && nEvtTotal%10==0)     cout<<nEvtTotal<<" \t "<<time_taken_so_far<<endl;
  else if(_verbosity>0 && nEvtTotal%10==0) cout<<nEvtTotal<<" \t "<<time_taken_so_far<<endl;

  nEvtTotal++;

  //----------------------------------
  // Construction of physics objects
  //----------------------------------

  //Clear all the arrays from the previous event:
  genMuon.clear(); genElectron.clear(); genLightLepton.clear();
  Muon.clear(); Electron.clear(); LightLepton.clear();

  //GenParticles:

  int test_evt = 1; //For displaying an event
  if(nEvtTotal == test_evt){
    cout<<"\n--------------------------------------------"<<endl;
    cout<<"Contents of event no. "<< test_evt <<":"<<endl;
    cout<<"--------------------------------------------"<<endl;
    cout<<"index\tpdgid\tmom1\tmom2\tdau1\tdau2"<<endl;
  }

  for(int i=0; i<(int)*Particle_size; i++){
    Particle temp;
    temp.v.SetPtEtaPhiM(Particle_PT[i], Particle_Eta[i], Particle_Phi[i], Particle_Mass[i]);
    temp.charge = Particle_Charge[i];
    temp.isolation = 0; //default
    temp.status = Particle_Status[i];
    temp.pdgid = Particle_PID[i];
    temp.mid_1 = Particle_M1[i];
    temp.mid_2 = Particle_M2[i];
    temp.did_1 = Particle_D1[i];
    temp.did_2 = Particle_D2[i];
    //temp.mid_1 = Particle_PID[Particle_M1[i]];
    //temp.mid_2 = Particle_PID[Particle_M2[i]];
    //temp.did_1 = Particle_PID[Particle_D1[i]];
    //temp.did_2 = Particle_PID[Particle_D2[i]];
      
    temp.index = i;

    //Fillup the genMuon collection:
    if(fabs(temp.pdgid)==13){
      bool passcuts = temp.status == 1;
      if(passcuts){
	genMuon.push_back(temp);
	genLightLepton.push_back(temp);
      }
    }

    //Display the genParticle collection for a particular event:
    if(nEvtTotal == test_evt){
      cout<<i<<"\t";
      cout<<temp.pdgid<<"\t";
      cout<<temp.mid_1<<"\t";
      cout<<temp.mid_2<<"\t";
      cout<<temp.did_1<<"\t";
      cout<<temp.did_2<<endl;
    }  
  }
  
  //Muons:
  for(int i=0; i<(int)*Muon_size; i++){
    Particle temp;
    temp.v.SetPtEtaPhiM(Muon_PT[i], Muon_Eta[i], Muon_Phi[i], 0.105);
    temp.charge = Muon_Charge[i];
    temp.isolation = Muon_IsolationVar[i];
    temp.status = 1; //default
    temp.pdgid = 13*Muon_Charge[i]; //Setting by hand
    temp.mid_1 = 0; //default
    temp.mid_2 = 0; //default
    temp.did_1 = 0; //default
    temp.did_2 = 0; //default
    temp.index = i;

    bool passcuts = true;
    if(passcuts){
      Muon.push_back(temp);
      LightLepton.push_back(temp);
    }  
  }
  
  if(nEvtTotal == test_evt){
    cout<<"--------------------------------------------\n"<<endl;
  }
  
  //Once done, sort all arrays in descending order of pT
  //Edit the Sort() function in "Functions.h" after adding new arrays of objects.
  Sort();

  //----------------------------------
  //        Analysis block
  //----------------------------------

  if((int)LightLepton.size() > 1){
    float dilepton_mass = (LightLepton.at(0).v + LightLepton.at(1).v).M();
    h.hist[0]->Fill(dilepton_mass);
  }
  

  return kTRUE;
}

