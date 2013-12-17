
#include "Riostream.h"
#include <fstream>
#include <iomanip>
#include <vector>
#include <string>
#include <TEfficiency.h>


void make_up(){
  gROOT->Reset();
  gStyle->SetOptStat(0);
  gStyle->SetPalette(1);
  gStyle->SetPaintTextFormat("2.2f");
  
  int Nhltpaths=442;
  
  //TFile * theFile = new TFile("/afs/cern.ch/user/c/carrillo/higgs/yy/hlt/CMSSW_5_3_2_patch4/src/genAnalyzer/GenAnalyzer/genAnalyzer.root");
  TFile * theFile = new TFile("/afs/cern.ch/user/c/carrillo/workspace/higgs/yy/-mass-/genAnalyzer.root");
  //TFile * theFile = new TFile("/afs/cern.ch/user/c/carrillo/workspace/higgs/yy/all.root");

  cout<<"getting the histo"<<endl;
  
  TH1F * numerator = (TH1F*)(theFile->Get("numerator"));
  TH1F * denominator = (TH1F*)(theFile->Get("denominator"));
  cout<<"creating canvas"<<endl;
  TCanvas * Ca0 = new TCanvas("Ca0","bit0",1200,800);
  Ca0->cd();
  
  system("mkdir gen");
  system("mkdir hlt");


  //all hlt bits efficiency plot
  if(TEfficiency::CheckConsistency(*numerator,*denominator)){
    TEfficiency* pEff = new TEfficiency(*numerator,*denominator);
    binomialEfficiency1D(pEff,Nhltpaths);
  }
  cout<<"cleaning canvas"<<endl;
  Ca0->SaveAs("hlt/all_hlt_eff.png");
  Ca0->Clear();

  //turnon curve
  //Interesting bits
  
  string interesting_bits[]={"418","6","416","8","5","412","233","7","234","10","187","9","22","198","430","404","188","199","21","190","197","208","201","207","195","196","206","193","424","192","204","194","203","205","209","189","200","191"};
  
  //string in_bits[]={"418","6"}
  //for(int k=0;k<2;k++){
  
  for(int k=0;k<38;k++){
    //cout<<k<<endl;
    TEfficiency * Effbit = (TEfficiency*) (theFile->Get(("bin"+interesting_bits[k]).c_str()));
    if(!Effbit) cout<<"Effbit not found"<<endl;
    binomialEfficiency2D(Effbit);
    Ca0->SaveAs(("hlt/bit_"+interesting_bits[k]+"_"+Effbit->GetTitle()+"_efficiency.png").c_str());
    Ca0->Clear();
  }

  TEfficiency * EffbitOR = (TEfficiency*) (theFile->Get("Eff_bit_194_195_205"));
  if(!EffbitOR) cout<<"EffbitOR not found"<<endl;
  binomialEfficiency2D(EffbitOR);
  Ca0->SaveAs("hlt/bitOR_efficiency.png");
  Ca0->Clear();

  TH1F * ptLeadhisto = (TH1F*)(theFile->Get("ptLeadhisto"));
  ptLeadhisto->Draw(); 
  ptLeadhisto->SetFillColor(kBlue);(); 
  ptLeadhisto->SetXTitle("p_{T} (GeV)");
  Ca0->SaveAs("gen/ptLeadhisto.png");Ca0->Clear();  

  TH1F * ptTrailhisto = (TH1F*)(theFile->Get("ptTrailhisto"));
  ptTrailhisto->Draw(); 
  ptTrailhisto->SetFillColor(kBlue);
  ptTrailhisto->SetXTitle("p_{T} (GeV)");
  Ca0->SaveAs("gen/ptTrailhisto.png");Ca0->Clear();  

  TH1F * massDiphotonhisto = (TH1F*)(theFile->Get("massDiphotonhisto"));
  massDiphotonhisto->Draw(); 
  massDiphotonhisto->SetFillColor(kBlue);
  massDiphotonhisto->SetXTitle("mass_{#gamma #gamma} (GeV)");
  Ca0->SaveAs("gen/massDiphotonhisto.png");Ca0->Clear();  

  TH1F * massHiggshisto = (TH1F*)(theFile->Get("massHiggshisto"));
  massHiggshisto->Draw(); 
  massHiggshisto->SetFillColor(kBlue);
  massHiggshisto->SetXTitle("mass_{H} (GeV)");
  Ca0->SaveAs("gen/massHiggshisto.png");Ca0->Clear();  
 
  TH1F * higgsEtaHist = (TH1F*)(theFile->Get("higgsEtaHist"));
  higgsEtaHist->Draw(); 
  higgsEtaHist->SetFillColor(kBlue); 
  higgsEtaHist->SetXTitle("#eta_{H}");
  Ca0->SaveAs("gen/higgsEtaHist.png");Ca0->Clear();  

  TH1F * higgsPhiHist = (TH1F*)(theFile->Get("higgsPhiHist"));
  higgsPhiHist->Draw(); 
  higgsPhiHist->SetFillColor(kBlue);
  higgsPhiHist->SetXTitle("#phi_{H}");
  Ca0->SaveAs("gen/higgsPhiHist.png");Ca0->Clear();  
  
  TH1F * higgsPHist = (TH1F*)(theFile->Get("higgsPHist"));
  higgsPHist->Draw();
  higgsPHist->SetFillColor(kBlue);
  higgsPHist->SetXTitle("p (GeV)");
  Ca0->SaveAs("gen/higgsPHist.png");Ca0->Clear();

  TH2F * phi1phi2 = (TH2F*)(theFile->Get("phi1phi2"));
  phi1phi2->Draw("colz");
  phi1phi2->SetXTitle("#phi_{Lead}");
  phi1phi2->SetYTitle("#phi_{Trail}");
  Ca0->SaveAs("gen/phi1phi2.png");Ca0->Clear();

  TH2F * eta1eta2 = (TH2F*)(theFile->Get("eta1eta2"));
  eta1eta2->Draw("colz");
  eta1eta2->SetXTitle("#eta_{Lead}");
  eta1eta2->SetYTitle("#eta_{Trail}");
  Ca0->SaveAs("gen/eta1eta2.png");Ca0->Clear();

  TH2F * pt1pt2 = (TH2F*)(theFile->Get("pt1pt2"));
  pt1pt2->Draw("colz");
  pt1pt2->SetXTitle("p_{T} Lead (GeV)");
  pt1pt2->SetYTitle("p_{T} Trail (GeV)");
  Ca0->SaveAs("gen/pt1pt2.png");Ca0->Clear();
  
  TH1F * numerator = (TH1F*)(theFile->Get("numerator"));
  numerator->Draw();
  numerator->SetFillColor(kBlue);
  numerator->SetXTitle("HLT bit");
  Ca0->SaveAs("hlt/numerator.png");Ca0->Clear();

  TH1F * denominator = (TH1F*)(theFile->Get("denominator"));
  denominator->Draw();
  denominator->SetFillColor(kBlue);
  denominator->SetXTitle("#HLT bit");
  Ca0->SaveAs("hlt/denominator.png");Ca0->Clear();

  Ca0->SetLogy();

  TH1F * higgsPtHist = (TH1F*)(theFile->Get("higgsPtHist"));
  higgsPtHist->Draw();
  higgsPtHist->SetFillColor(kBlue);
  higgsPtHist->SetXTitle("p_{T} (GeV)");
  Ca0->SaveAs("gen/higgsPtHist.png");Ca0->Clear();

  exit(0);
}

void binomialEfficiency2D(TEfficiency * teff){
  TH2D numerator = (TH2D)teff->GetPassedHistogram();
  TH2D denominator = (TH2D)teff->GetTotalHistogram();
  
  const Double_t bins[17]={20,25,30,35,40,45,50,55,60,65,70,75,80,90,100,110,120};
  TH2F * efficiency = new TH2F ("efficiency",teff->GetTitle(),16,bins,16,bins);
  efficiency->SetXTitle("E_{T} #gamma Lead");
  efficiency->SetYTitle("E_{T} #gamma Trail");
  float eff,err;
  for(int i=0;i<17;i++)
    for(int j=0;j<=i;j++){
      if(denominator.GetBinContent(i,j)!=0){
	eff = numerator.GetBinContent(i,j)/denominator.GetBinContent(i,j);
	err = sqrt(eff*(1-eff)/denominator.GetBinContent(i,j));
	efficiency->SetBinContent(i,j,eff);
	efficiency->SetBinError(i,j,err);
	//cout<<i<<" "<<j<<" "<<eff<<"+/-"<<err<<endl;
      }
    }
  efficiency->Draw("colztextE");
}

void binomialEfficiency1D(TEfficiency * teff,int Nhltpaths){
  TH1D numerator = (TH1D)teff->GetPassedHistogram();
  TH1D denominator = (TH1D)teff->GetTotalHistogram();
  TH1F * efficiency = new TH1F ("efficiency","hlt bit efficiency %",Nhltpaths,-0.5,Nhltpaths-0.5);
  efficiency->SetXTitle("HLT bit");
  efficiency->SetYTitle("#epsilon");
  float eff,err;
  for(int j=0;j<=Nhltpaths;j++){
    if(denominator.GetBinContent(j)!=0){
      eff = numerator.GetBinContent(j)/denominator.GetBinContent(j);
      err = sqrt(eff*(1-eff)/denominator.GetBinContent(j));
      efficiency->SetBinContent(j,eff);
      efficiency->SetBinError(j,err);
      cout<<"hlteff "<<j<<" "<<eff<<" +/- "<<err<<endl;
    }
  }
  efficiency->Draw("E");
  efficiency->SetMarkerColor(kRed);
  efficiency->SetMarkerStyle(23);
  efficiency->SetMarkerSize(0.5);
}
      

