//#include "tdrstyle.C"
#include <iostream>
#include "Riostream.h"
#include <fstream>
#include <iomanip>
#include <vector>
#include <string>
#include <TEfficiency.h>
#include <TLegend.h>
#include <TFile.h>
#include <TH1F.h>
#include <TH2F.h>
#include <TGraphAsymmErrors.h>
#include "TStyle.h"
#include "TCanvas.h"

void manyInSame(){

  gROOT->Reset();
  gStyle->SetOptStat(000000);
  //setTDRStyle();

  //TFile * theFile1 = new TFile("/afs/cern.ch/user/c/carrillo/www/higgs/runII/EBPVptLead050/output_hlt.root");
  //TFile * theFile2 = new TFile("/afs/cern.ch/user/c/carrillo/www/higgs/runII/EBPVptLead052/output_hlt.root");
  //TFile * theFile3 = new TFile("/afs/cern.ch/user/c/carrillo/www/higgs/runII/EBPVptLead054/output_hlt.root");
  //TFile * theFile4 = new TFile("/afs/cern.ch/user/c/carrillo/www/higgs/runII/EBPVptLead056/output_hlt.root");

  //TFile * theFile1 = new TFile("/afs/cern.ch/work/c/carrillo/microAOD/ANDPVorEB/052/110lowmass052.root");
  //TFile * theFile2 = new TFile("/afs/cern.ch/work/c/carrillo/microAOD/ANDPVorEB/054/110lowmass054.root");

  //TFile * theFile1 = new TFile("/afs/cern.ch/work/c/carrillo/microAOD/EB/test/110lowmass_EB.root");
  //TFile * theFile1 = new TFile("/afs/cern.ch/work/c/carrillo/microAOD/ANDPVorEB/test/110lowmass_ANDPVorEB.root");
  //TFile * theFile1 = new TFile("/afs/cern.ch/work/c/carrillo/microAOD/ANDPVorEBpreselection/110lowmass_ANDPVorEBpreselection.root");
  //TFile * theFile1 = new TFile("/afs/cern.ch/work/c/carrillo/microAOD/ANDPVorEBpreselectionEta/110lowmass_ANDPVorEBpreselectionEta.root");
  //TFile * theFile1 = new TFile("/afs/cern.ch/work/c/carrillo/microAOD/ANDPV/110lowmass_ANDPV.root");
  //TFile * theFile1 = new TFile("/afs/cern.ch/user/c/carrillo/workspace/microAOD/ANDPVorEB04530/ANDPVorEB04530.root");
  //TFile * theFile1 = new TFile("/afs/cern.ch/user/c/carrillo/workspace/microAOD/ANDPVorEB04930/ANDPVorEB04930.root");
  //TFile * theFile1 = new TFile("/afs/cern.ch/user/c/carrillo/workspace/microAOD/ANDPVorEB04430/ANDPVorEB04430.root");
  //TFile * theFile1 = new TFile("/afs/cern.ch/user/c/carrillo/workspace/microAOD/ANDPVorEB04427/ANDPVorEB04427.root");
  //TFile * theFile1 = new TFile("/afs/cern.ch/user/c/carrillo/workspace/microAOD/ANDPVorEB04927/ANDPVorEB04927.root");
  //TFile * theFile1 = new TFile("/afs/cern.ch/user/c/carrillo/workspace/microAOD/ANDPVorEB04927pre/ANDPVorEB04927pre.root");
  //TFile * theFile1 = new TFile("/afs/cern.ch/user/c/carrillo/workspace/microAOD/ANDPVorEB04930pre/ANDPVorEB04930pre.root");
  //TFile * theFile1 = new TFile("/afs/cern.ch/user/c/carrillo/workspace/microAOD/ANDPVorEB04430pre/ANDPVorEB04430pre.root");
  //TFile * theFile1 = new TFile("/afs/cern.ch/user/c/carrillo/workspace/microAOD/ANDPVorEB04427pre/ANDPVorEB04427pre.root");
  //TFile * theFile1 = new TFile("/afs/cern.ch/user/c/carrillo/workspace/microAOD/ANDPVorEB04432pre/ANDPVorEB04432pre.root");
  //TFile * theFile1 = new TFile("/afs/cern.ch/user/c/carrillo/workspace/microAOD/ANDPVorEB04932pre/ANDPVorEB04932pre.root");
  TFile * theFile1 = new TFile("/afs/cern.ch/user/c/carrillo/workspace/microAOD/Xsynch_3249/Xsynch_3249.root");
   
  cout<<"Reading histos"<<endl;

  TH1F * hist0 = (TH1F*) (theFile1->Get("massDiphoton_0"));


  
  TH1F * histo[4][4];

  histo[0][0] = (TH1F*) (theFile1->Get("massDiphoton_2"));
  histo[0][1] = (TH1F*) (theFile1->Get("massDiphoton_4"));
  histo[0][2] = (TH1F*) (theFile1->Get("massDiphoton_6"));
  histo[0][3] = (TH1F*) (theFile1->Get("massDiphoton_8"));

  /*
  histo[1][0] = (TH1F*) (theFile2->Get("massDiphoton_2"));
  histo[1][1] = (TH1F*) (theFile2->Get("massDiphoton_4"));
  histo[1][2] = (TH1F*) (theFile2->Get("massDiphoton_6"));
  histo[1][3] = (TH1F*) (theFile2->Get("massDiphoton_8"));

  histo[2][0] = (TH1F*) (theFile3->Get("massDiphoton_2"));
  histo[2][1] = (TH1F*) (theFile3->Get("massDiphoton_4"));
  histo[2][2] = (TH1F*) (theFile3->Get("massDiphoton_6"));
  histo[2][3] = (TH1F*) (theFile3->Get("massDiphoton_8"));

  histo[3][0] = (TH1F*) (theFile4->Get("massDiphoton_2"));
  histo[3][1] = (TH1F*) (theFile4->Get("massDiphoton_4"));
  histo[3][2] = (TH1F*) (theFile4->Get("massDiphoton_6"));
  histo[3][3] = (TH1F*) (theFile4->Get("massDiphoton_8"));
  */
  
  cout<<"Creating Canvas"<<endl;
  TCanvas *Ca1 = new TCanvas("Ca1","m_#gamma#gamma",1200,800);
  
  float max = -1;

  int color[8];
  color[0]=1;
  color[1]=9;
  color[2]=2;
  color[3]=4;
  color[4]=6;
  color[5]=5;


  float thismax=0;

  int imin=0;
  int imax=1;

  int jmin=0;
  int jmax=4;
  
  for(int i = imin;i<imax;i++){
    for(int j = jmin;j<jmax;j++){
      thismax = histo[i][j]->GetMaximum();
      if(thismax>max) max = thismax;
      cout<<i<<" "<<j<<" "<<histo[i][j]->GetMaximum()<<" "<<thismax<<endl;
    }
  }
  cout<<max<<endl;
  
  int scale=100;
  
  for(int i = imin;i<imax;i++){
    for(int j = jmin;j<jmax;j++){
      histo[i][j]->SetMaximum(35000);
      histo[i][j]->GetXaxis()->SetRangeUser(55.,70.);
      histo[i][j]->SetMinimum(1000);
    }
  }
  
  bool first = true;
  for(int i = imin;i<imax;i++){
    for(int j = jmin;j<jmax;j++){
	cout<<"Drawing "<<i<<" "<<j<<"th"<<endl;
	histo[i][j]->SetLineWidth(2);  

	if(j==0||j==1){
	  histo[i][j]->SetMarkerColor(9);
	  histo[i][j]->SetLineColor(9);  
	}else{
	  histo[i][j]->SetMarkerColor(2);
	  histo[i][j]->SetLineColor(2);  
	}

	if(first==true){
	  histo[i][j]->Rebin(scale);
	  histo[i][j]->GetXaxis()->SetRangeUser(55.,90.);
	  //histo[i][j]->DrawNormalized("Ehist");
	  histo[i][j]->Draw("E");
	  first=false;
	  histo[i][j]->GetXaxis()->SetTitle("m_{#gamma #gamma} (GeV)");
	  histo[i][j]->SetTitle("Study p_{T}/m_{#gamma #gamma} cuts");
	  histo[i][j]->SetMarkerStyle(20);
	}else{
	  histo[i][j]->Rebin(scale);
	  histo[i][j]->GetXaxis()->SetRangeUser(55.,90.);
	  //histo[i][j]->DrawNormalized("sameEhist");
	  histo[i][j]->Draw("sameE");
	  histo[i][j]->SetMarkerStyle(20);
	}
    }
  }
  
  hist0->Rebin(scale);
  hist0->GetXaxis()->SetRangeUser(55.,90.);
  hist0->Draw("sameE");
  hist0->SetMarkerStyle(9);


  cout<<"out of the loop"<<endl;

  /*
  stringstream sta[N+1];
 
  TLegend *leg = new TLegend(0.4974874,0.8304196,0.9974874,0.9965035,NULL,"brNDC");
  leg->SetTextFont(62);
  leg->SetLineColor(1);
  leg->SetLineStyle(1);
  leg->SetLineWidth(1);
  leg->SetFillColor(19);
  leg->SetFillStyle(1001);
  
  float x[N+1], y[N+1], ey[N+1];
 
 for(int i = 1;i<N+1;i++){
   sta[i]<<"Mass "<<mass[i]<<"GeV Mean "<<endl<<particleBeta[i]->GetMean()<<endl<<" RMS "<<particleBeta[i]->GetRMS();
   x[i]=mass[i];
   y[i]=particleBeta[i]->GetMean();
   ey[i]=particleBeta[i]->GetRMS();
   particleBeta[i]->SetMaximum(1.2*max);
 }
 
 for(int i = 1;i<N+1;i++){
   leg->AddEntry(particleBeta[i],(sta[i].str()).c_str(),"l");
 }
 
 leg->Draw("same");
  */
  
  Ca1->SaveAs("mgammagamma.png");
  //Ca1->SaveAs("mgammagammaNorma.png");

 /*
 TGraphErrors * grp = new TGraphErrors(N+1,x,y,0,ey);
 
 TCanvas * Ca0 = new TCanvas("Ca0","Canvas",1200,800);    
 
 grp->SetTitle("Beta for different mass points");
 grp->SetLineColor(kRed);
 grp->SetMarkerStyle(20);
 grp->SetMarkerSize(1.0);
 grp->GetXaxis()->SetTitle("gluino mass(GeV)");
 grp->GetYaxis()->SetTitle("Mean Beta error bars RMS");
 grp->Draw("AP");

 Ca0->SaveAs("BetaVsMass.png");
 */
 exit(0) ;
}









