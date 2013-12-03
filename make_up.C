#include "Riostream.h"
#include <fstream>
#include <iomanip>
#include <vector>
#include <string>

void setHisto(TH2F * histo){
  //  histo->GetXaxis()->LabelsOption("v");
  //histo->Draw("CONT4COLZ ");
  //histo->Draw("COLZ ");
  histo->Draw("colztextE");
  //histo->Draw("");
  histo->GetXaxis()->SetTitle("Lead E#gamma (GeV)");
  histo->GetYaxis()->SetTitle("Trail E#gamma (GeV)");
  histo->SetTitle("Efficiency bit0");
}

void make_up(){
  gROOT->Reset();
  gStyle->SetOptStat(0);
  gStyle->SetPalette(1);
  gStyle->SetPaintTextFormat("2.2f");
  cout<<"getting the file"<<endl;
  TFile * theFile = new TFile("TurnOnError.root");
  cout<<"getting the histo"<<endl;
  TH2F * turnon = (TH2F*) (theFile->Get("bit0"));
  if(turnon) cout<<"histo not found"<<endl;
  cout<<"creating canvas"<<endl;
  TCanvas * Ca0 = new TCanvas("Ca0","bit0",1200,800);
  setHisto(turnon);
  cout<<"saving plot"<<endl;
  Ca0->SaveAs("turn_on_plot.png");
  exit(0);
}
