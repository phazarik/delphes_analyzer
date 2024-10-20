#include <TROOT.h>
#include <TChain.h>
#include <TFile.h>

/*

-----------
HOW TO RUN
-----------

First, load the following libraries in the ROOT prompt. Put correct paths to Delphes.
[] gInterpreter->AddIncludePath("/home/phazarik/mcgeneration/delphes");
[] gInterpreter->AddIncludePath("/home/phazarik/mcgeneration/delphes/external");
[] gSystem->Load("/home/phazarik/mcgeneration/delphes/libDelphes.so");

Then compile the analyzer codes and load the library:
[] .L delAna.C+

Run this code on the loaded libray.
[] .x ana.C(0)

*/

void ana(int sample=0){

  const char *hstfilename, *sumfilename; 
  TChain *chain = new TChain("Delphes");
  delAna m_selec;
  
  if(sample==0){//EGamma data
    chain->Add("inputs/ppToZToLL_10K.root");
    hstfilename = "outputs/hst_ppToZToLL.root";
    sumfilename = "outputs/sum_ppToZToLL.txt";
  }

  std::cout<<"Output : "<<hstfilename<<std::endl;
  m_selec.SetHstFileName(hstfilename);
  m_selec.SetSumFileName(sumfilename);
  m_selec.SetVerbose(0);
  
  chain->Process(&m_selec);
}
