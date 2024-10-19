
//These additional functions must be loaded to the main script.

void delAna::Sort()
{
  //Sort selected objects by pT (always descending).
  //Muons:
  for(int i=0; i<(int)genMuon.size()-1; i++){
    for(int j=i+1; j<(int)genMuon.size(); j++){
      if( genMuon[i].v.Pt() < genMuon[j].v.Pt() )
	swap(genMuon.at(i),genMuon.at(j)); }}
  //Electron:
  for(int i=0; i<(int)genElectron.size()-1; i++){
    for(int j=i+1; j<(int)genElectron.size(); j++){
      if( genElectron[i].v.Pt() < genElectron[j].v.Pt() )
	swap(genElectron.at(i),genElectron.at(j)); }}
  //LightLepton:
  for(int i=0; i<(int)LightLepton.size()-1; i++){
    for(int j=i+1; j<(int)LightLepton.size(); j++){
      if( LightLepton[i].v.Pt() < LightLepton[j].v.Pt() )
	swap(LightLepton.at(i),LightLepton.at(j)); }}
  //Muons:
  for(int i=0; i<(int)Muon.size()-1; i++){
    for(int j=i+1; j<(int)Muon.size(); j++){
      if( Muon[i].v.Pt() < Muon[j].v.Pt() )
	swap(Muon.at(i),Muon.at(j)); }}
  //Electron:
  for(int i=0; i<(int)Electron.size()-1; i++){
    for(int j=i+1; j<(int)Electron.size(); j++){
      if( Electron[i].v.Pt() < Electron[j].v.Pt() )
	swap(Electron.at(i),Electron.at(j)); }}
  //LightLepton:
  for(int i=0; i<(int)LightLepton.size()-1; i++){
    for(int j=i+1; j<(int)LightLepton.size(); j++){
      if( LightLepton[i].v.Pt() < LightLepton[j].v.Pt() )
	swap(LightLepton.at(i),LightLepton.at(j)); }}

}

float delAna::delta_phi(float phi1, float phi2)
{
  phi1 = TVector2::Phi_0_2pi(phi1);
  phi2 = TVector2::Phi_0_2pi(phi2);
  float dphi = fabs(phi1 - phi2);
  if(dphi>TMath::Pi()) dphi = 2*TMath::Pi() - dphi;
  return dphi;
}

float delAna::transv_mass(float pt_lep, float MET, float dphi)
{
  //The inputs are the Energy of the lepton, MET and dPhi between the lepton and MET
  float mT = sqrt(2* pt_lep * MET *(1-cos(dphi)));
  return mT;
}

