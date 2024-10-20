void delAna::BookHistograms(){
  
  h.hist[0] = new TH1F("m_ll", "dilepton mass", 200, 0, 200);
  h.hist[1] = new TH1F("m_ee", "dielectron mass", 200, 0, 200);
  h.hist[2] = new TH1F("m_mm", "dimuon mass", 200, 0, 200);

}
