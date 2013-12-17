// -*- C++ -*-
//
// Package:    GenAnalyzer
// Class:      GenAnalyzer
// 
/**\class GenAnalyzer GenAnalyzer.cc genAnalyzer/GenAnalyzer/src/GenAnalyzer.cc

 Description: [one line class summary]

 Implementation:
     [Notes on implementation]
*/
//
// Original Author:  Camilo Andres Carrillo Montoya,42 R-021,+41227671624,
//         Created:  Wed Dec  4 18:13:51 CET 2013
// $Id$
//
//


// system include files
#include <memory>

// user include files
#include "FWCore/Framework/interface/EDAnalyzer.h"

#include "FWCore/Framework/interface/Event.h"
#include "FWCore/Framework/interface/MakerMacros.h"

#include "FWCore/ParameterSet/interface/ParameterSet.h"

#include "DataFormats/HepMCCandidate/interface/GenParticle.h"
#include "DataFormats/Candidate/interface/Particle.h"
#include "DataFormats/Candidate/interface/Candidate.h"
#include "DataFormats/Common/interface/TriggerResults.h"
#include "FWCore/Common/interface/TriggerNames.h"

#include "TFile.h"
#include "TH1F.h"
#include "TH2F.h"
#include "TMath.h"
#include "TCanvas.h"
#include "TGaxis.h"
#include "TStyle.h"
#include "TEfficiency.h"
#define Nhltpaths 442

using namespace std;
using namespace edm;
using namespace reco;

//
// class declaration
//

class GenAnalyzer : public edm::EDAnalyzer {
   public:
      explicit GenAnalyzer(const edm::ParameterSet&);
      ~GenAnalyzer();

      static void fillDescriptions(edm::ConfigurationDescriptions& descriptions);
      TFile * theFileOut;
      TH1F * ptLeadhisto;
      TH1F * ptTrailhisto;
      TH1F * massDiphotonhisto;
      TH1F * massHiggshisto;
      TH1F * numerator;
      TH1F * denominator;
      TH1F * higgsEtaHist;
      TH1F * higgsPhiHist;
      TH1F * higgsPHist;
      TH1F * higgsPtHist;
      TH2F * phi1phi2;
      TH2F * eta1eta2;
      TH2F * pt1pt2;
      TEfficiency * Effbit[Nhltpaths];
      TEfficiency * Eff_bit_194_195_205;
      string arrayHLTpathsNames[Nhltpaths];
      bool filled; 
  
   private:
      virtual void beginJob() ;
      virtual void analyze(const edm::Event&, const edm::EventSetup&);
      virtual void endJob() ;

      virtual void beginRun(edm::Run const&, edm::EventSetup const&);
      virtual void endRun(edm::Run const&, edm::EventSetup const&);
      virtual void beginLuminosityBlock(edm::LuminosityBlock const&, edm::EventSetup const&);
      virtual void endLuminosityBlock(edm::LuminosityBlock const&, edm::EventSetup const&);
      edm::InputTag theGenParticleLabel;
      edm::InputTag theHltInputLabel;


  
      
      // ----------member data ---------------------------
};

//
// constants, enums and typedefs
//

//
// static data member definitions
//

//
// constructors and destructor
//
GenAnalyzer::GenAnalyzer(const edm::ParameterSet& iConfig)

{
   //now do what ever initialization is needed
  theGenParticleLabel = iConfig.getParameter<edm::InputTag>("GenParticleLabel");
  theHltInputLabel    = iConfig.getParameter<edm::InputTag>("hltInputTag");

}


GenAnalyzer::~GenAnalyzer()
{
 
   // do anything here that needs to be done at desctruction time
   // (e.g. close files, deallocate resources etc.)

}

//
// member functions
//

// ------------ method called for each event  ------------
void
GenAnalyzer::analyze(const edm::Event& iEvent, const edm::EventSetup& iSetup)
{
   using namespace edm;
   // Access Gen Particles
   Handle<GenParticleCollection> genParticles;
   iEvent.getByLabel(theGenParticleLabel, genParticles );
   std::cout<<"before the if genParticles.isValid()"<<std::endl;
   if(genParticles.isValid()) std::cout<<"valid"<<std::endl;
   else return;

   // Access Trigger Results
   edm::Handle<edm::TriggerResults> hltTriggerResultHandle;
   iEvent.getByLabel(theHltInputLabel,hltTriggerResultHandle);

   if(filled==false){
     const edm::TriggerNames & triggerNames = iEvent.triggerNames(*hltTriggerResultHandle);
     std::cout<<triggerNames.triggerName(32)<<std::endl; 
     //std::cout<<arrayHLTpathsNames.size()<<std::endl;
     for(int i=0;i<Nhltpaths;i++){
       arrayHLTpathsNames[i]=triggerNames.triggerName(i);
     }
     filled=true;
     //std::cout<<arrayHLTpathsNames.size()<<std::endl;
   }

   


   int hltCount= hltTriggerResultHandle->size();
   bool allHLTResults[Nhltpaths] = { false };
   int bin;
   
   if(!hltTriggerResultHandle.isValid()) {
     std::cout << "invalid handle for HLT TriggerResults" << std::endl;
   }else{
     for(bin = 0 ; bin < hltCount ; bin++){
       allHLTResults[bin] = hltTriggerResultHandle->accept(bin);
       //std::cout<<"bit:"<<i<<" "<<allHLTResults[i]<<std::endl;
     }
   }
   
   for(bin = 0 ; bin < hltCount ; bin++){
     if(allHLTResults[bin])numerator->Fill(bin);
     denominator->Fill(bin);
   }
   
   for (size_t i = 0; i < genParticles->size(); i++){
     const Candidate & p = (*genParticles)[i];
     //Higgs in status 3
     if (p.pdgId() == 25 && p.status() == 3){
       std::cout<<"Higgs mass:"<<p.mass()<<" status:"<<p.status()<<std::endl;
       
       higgsEtaHist->Fill(p.p4().eta());
       higgsPhiHist->Fill(p.p4().phi());
       higgsPHist->Fill(sqrt(p.p4().Vect().Dot(p.p4().Vect()))); //TO CHECK
       higgsPtHist->Fill(p.pt());
       
       // size_t nd = p.numberOfDaughters(); 
       //for(size_t j = 0; j < nd; ++ j ) {
       //const Candidate * d = p.daughter(j);
       const Candidate * photon0 = p.daughter(0);
       const Candidate * photon1 = p.daughter(1);
       if (photon0->pdgId()!=22) break;
       if (photon1->pdgId()!=22) break;

       if(photon0->pt() <= photon1->pt()){//photon1 Lead
	 ptLeadhisto->Fill(photon1->pt());
	 ptTrailhisto->Fill(photon0->pt());
	 phi1phi2->Fill(photon1->phi(),photon0->phi());
	 eta1eta2->Fill(photon1->eta(),photon0->eta());
	 pt1pt2->Fill(photon1->pt(),photon0->pt());
	 //Efficiency per bit
	 for(int i=0;i<Nhltpaths;i++){
	   Effbit[i]->Fill(allHLTResults[i],photon1->pt(),photon0->pt());
	 }
	 
	 Eff_bit_194_195_205->Fill(allHLTResults[194]+allHLTResults[195]+allHLTResults[205],photon1->pt(),photon0->pt());

       }else{ //photon0 Lead
	 ptLeadhisto->Fill(photon0->pt());
	 ptTrailhisto->Fill(photon1->pt());
	 phi1phi2->Fill(photon0->phi(),photon1->phi());
	 eta1eta2->Fill(photon0->eta(),photon1->eta());
	 pt1pt2->Fill(photon0->pt(),photon1->pt());
	 //Efficiency per bit
	 for(int i=0;i<Nhltpaths;i++){
	   Effbit[i]->Fill(allHLTResults[i],photon0->pt(),photon1->pt());
	 }

	 Eff_bit_194_195_205->Fill(allHLTResults[194]+allHLTResults[195]+allHLTResults[205],photon0->pt(),photon1->pt());
       }
       
       massHiggshisto->Fill(p.mass());
       
       Particle::LorentzVector diphoton = photon0->p4()+photon1->p4();
       massDiphotonhisto->Fill(sqrt(diphoton.Dot(diphoton)));
       
     }
   }
   
}


// ------------ method called once each job just before starting event loop  ------------
void 
GenAnalyzer::beginJob()
{
  filled=false;
  theFileOut = new TFile("genAnalyzer.root", "RECREATE");
  ptLeadhisto = new TH1F ("ptLeadhisto","ptLeadhisto",100,0.,150.);
  ptTrailhisto = new TH1F ("ptTrailhisto","ptTrailhisto",100,0.,150.);
  massDiphotonhisto = new TH1F ("massDiphotonhisto","massDiphotonhisto",100,80.,150.);
  massHiggshisto= new TH1F ("massHiggshisto","massHiggshisto",100,80.,170.);
  
  higgsEtaHist = new TH1F ("higgsEtaHist","GEN Higgs #eta",100,-5,5);
  higgsPhiHist = new TH1F ("higgsPhiHist","GEN Higgs #phi",180,-3.15,3.15);
  higgsPHist = new TH1F ("higgsPHist","GEN Higgs P",500,0,1200);
  higgsPtHist = new TH1F ("higgsPtHist","GEN Higgs P_{T}",500,0,1200);  
  
  phi1phi2 = new TH2F ("phi1phi2","#phi correlation #gamma #gamma",180,-3.15,3.15,180,-3.15,3.15);
  eta1eta2 = new TH2F ("eta1eta2","#eta correlation #gamma #gamma",100,-5,5,100,-5,5);
  pt1pt2 = new TH2F ("pt1pt2","p_{T} correlation #gamma #gamma",100,20.,120.,100,20.,120.);

  numerator= new TH1F ("numerator","numerator",Nhltpaths,-0.5,Nhltpaths-0.5);
  denominator= new TH1F ("denominator","denominator",Nhltpaths,-0.5,Nhltpaths-0.5);
  
  const Double_t bins[15]={30,35,40,45,50,55,60,65,70,75,80,90,100,110,120};
  
  char histo_name[128];
  for(int i=0;i<Nhltpaths;i++){
    std::string bin = std::to_string(i);
    sprintf(histo_name,"bin%s",bin.c_str());
    Effbit[i]= new TEfficiency (histo_name,";E_T #gamma Lead;E_T #gamma Trail;#epsilon",14,bins,14,bins);
  }

  Eff_bit_194_195_205 = new TEfficiency ("Eff_bit_194_195_205",";E_T #gamma Lead;E_T #gamma Trail;#epsilon",14,bins,14,bins);
  
}

// ------------ method called once each job just after ending the event loop  ------------
void 
GenAnalyzer::endJob() 
{

  theFileOut->cd();

  for(int i=0;i<Nhltpaths;i++){
    Effbit[i]->SetTitle(arrayHLTpathsNames[i].c_str());
    Effbit[i]->Write();
  }

  Eff_bit_194_195_205->Write();
  

  
  ptLeadhisto->Write();
  ptTrailhisto->Write();
  massDiphotonhisto->Write();
  massHiggshisto->Write();
  numerator->Write();
  denominator->Write();
  
  higgsEtaHist->Write();
  higgsPhiHist->Write(); 
  higgsPHist->Write(); 
  higgsPtHist->Write();
  phi1phi2->Write();
  eta1eta2->Write();
  pt1pt2->Write();
  
  theFileOut->Close();
}

// ------------ method called when starting to processes a run  ------------
void 
GenAnalyzer::beginRun(edm::Run const&, edm::EventSetup const&)
{
}

// ------------ method called when ending the processing of a run  ------------
void 
GenAnalyzer::endRun(edm::Run const&, edm::EventSetup const&)
{
}

// ------------ method called when starting to processes a luminosity block  ------------
void 
GenAnalyzer::beginLuminosityBlock(edm::LuminosityBlock const&, edm::EventSetup const&)
{
}

// ------------ method called when ending the processing of a luminosity block  ------------
void 
GenAnalyzer::endLuminosityBlock(edm::LuminosityBlock const&, edm::EventSetup const&)
{
}

// ------------ method fills 'descriptions' with the allowed parameters for the module  ------------
void
GenAnalyzer::fillDescriptions(edm::ConfigurationDescriptions& descriptions) {
  //The following says we do not know what parameters are allowed so do no validation
  // Please change this to state exactly what you do use, even if it is no parameters
  edm::ParameterSetDescription desc;
  desc.setUnknown();
  descriptions.addDefault(desc);
}

//define this as a plug-in
DEFINE_FWK_MODULE(GenAnalyzer);
