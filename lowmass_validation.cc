#include <memory>

#include "FWCore/Framework/interface/Frameworkfwd.h"
#include "FWCore/Framework/interface/EDAnalyzer.h"
#include "FWCore/MessageLogger/interface/MessageLogger.h"

#include "FWCore/Framework/interface/Event.h"
#include "FWCore/Framework/interface/MakerMacros.h"
#include "FWCore/Utilities/interface/InputTag.h"
#include "FWCore/ParameterSet/interface/ParameterSet.h"
#include "CommonTools/UtilAlgos/interface/TFileService.h"
#include "FWCore/ServiceRegistry/interface/Service.h"
#include "DataFormats/Common/interface/Handle.h"
#include "DataFormats/Common/interface/Ptr.h"
#include "DataFormats/VertexReco/interface/Vertex.h"

#include "flashgg/MicroAOD/interface/VertexSelectorBase.h"
#include "DataFormats/EgammaCandidates/interface/PhotonCore.h"
#include "DataFormats/EgammaCandidates/interface/Photon.h"
#include "DataFormats/PatCandidates/interface/Photon.h"
#include "flashgg/DataFormats/interface/Photon.h"
#include "flashgg/DataFormats/interface/DiPhotonCandidate.h"

#include "HLTrigger/HLTcore/interface/HLTConfigProvider.h"

#include "flashgg/MicroAOD/interface/PhotonIdUtils.h"

#include "flashgg/DataFormats/interface/VertexCandidateMap.h"

#include "FWCore/Common/interface/TriggerNames.h"

#include <DataFormats/Math/interface/deltaR.h>

#include "TTree.h"
#include "TFile.h"
#include "TH2F.h"
#include "TH1F.h"

// **********************************************************************

// define the structures used to create tree branches and fill the trees

// per-event tree:
struct eventInfo {

    int ndiphoton;

};

using namespace std;
using namespace edm;
using namespace flashgg;

// **********************************************************************

class LowMassValidation : public edm::EDAnalyzer
{
public:
    explicit LowMassValidation( const edm::ParameterSet & );
    ~LowMassValidation();

private:

    edm::Service<TFileService> fs_;

    virtual void beginJob() override;
    virtual void analyze( const edm::Event &, const edm::EventSetup & ) override;
    virtual void endJob() override;

    eventInfo evInfo;

    TH1F *massTurnOn[10][10];

    TH1F *ptLead[9  ];
    TH1F *ptTrail[9  ];
    TH1F *ptLeadNorm[9  ];
    TH1F *ptTrailNorm[9  ];
    TH1F *massDiphotonhisto[9  ];
    TH1F *massHiggshisto[9  ];
    TH1F *higgsEta[9  ];
    TH1F *higgsPhi[9  ];
    TH1F *higgsP[9  ];
    TH1F *higgsPt[9  ];
    TH2F *phi1phi2[9  ];
    TH2F *eta1eta2[9  ];
    TH2F *pt1pt2[9  ];
    TH2F *pt1pt2Norm[9  ];
    TH2F *pt1pt2Zoom[9  ];
    TH2F *wide_pt1pt2[9  ];

    TH1F *diphoton_LeadR9[9  ];
    TH1F *diphoton_SubLeadR9[9  ];

    TH1F *diphoton_LeadHoE[9  ];
    TH1F *diphoton_SubLeadHoE[9  ];

    TH1F *diphoton_LeadTrkIso03[9  ];
    TH1F *diphoton_SubLeadTrkIso03[9  ];

    TH1F *diphoton_LeadHcalIso03[9  ];
    TH1F *diphoton_SubLeadHcalIso03[9  ];

    TH1F *diphoton_Leadpfiso02[9  ];
    TH1F *diphoton_SubLeadpfiso02[9  ];

    const Double_t bins[19] = {5, 10, 15, 20, 25, 30, 35, 40, 45, 50, 55, 60, 65, 70, 75, 80, 90, 100, 110};
    bool cut[9  ];
    int cut_index;

    TH1F *cutflow;
    TFile *theFileOut;
    TString name;

    edm::EDGetTokenT<edm::View<flashgg::DiPhotonCandidate> > diphotonToken_;
    edm::EDGetTokenT<edm::View<flashgg::DiPhotonCandidate> > diphotonToken_1;
    edm::EDGetTokenT<edm::View<flashgg::DiPhotonCandidate> > diphotonToken_2;

    edm::EDGetTokenT<edm::TriggerResults> triggerBits_;
    edm::ParameterSetID triggerNamesID_;
    bool first;

    stringstream label;
    
};

// ******************************************************************************************
// ******************************************************************************************

//
// constants, enums and typedefs
//

//
// static data member definitions
//

// data to compute preselection efficiencies per lead/sublead photon category

//
// constructors and destructor
//
LowMassValidation::LowMassValidation( const edm::ParameterSet &iConfig ):
    diphotonToken_( consumes<View<flashgg::DiPhotonCandidate> >( iConfig.getParameter<InputTag> ( "DiPhotonTag" ) ) ),
    diphotonToken_1( consumes<View<flashgg::DiPhotonCandidate> >( iConfig.getParameter<InputTag> ( "DiPhotonTag1" ) ) ),
    diphotonToken_2( consumes<View<flashgg::DiPhotonCandidate> >( iConfig.getParameter<InputTag> ( "DiPhotonTag2" ) ) ),
    triggerBits_( consumes<edm::TriggerResults>( iConfig.getParameter<edm::InputTag>( "bits" ) ) )
{

}


LowMassValidation::~LowMassValidation()
{


}



void
LowMassValidation::analyze( const edm::Event &iEvent, const edm::EventSetup &iSetup )
{

    // ********************************************************************************

    // access edm objects

    Handle<View<flashgg::DiPhotonCandidate> > diphotons;
    iEvent.getByToken( diphotonToken_, diphotons );

    Handle<View<flashgg::DiPhotonCandidate> > diphotons1;
    iEvent.getByToken( diphotonToken_1, diphotons1 );

    Handle<View<flashgg::DiPhotonCandidate> > diphotons2;
    iEvent.getByToken( diphotonToken_2, diphotons2 );

    edm::Handle<edm::TriggerResults> triggerBits;

    iEvent.getByToken( triggerBits_, triggerBits );

    if( !triggerBits.isValid() ) {
        std::cout<< "TriggerResults product not found - returning result=false!"<<std::endl;
        return;
    }

    const edm::TriggerNames &triggerNames = iEvent.triggerNames( *triggerBits );

    //vector<std::string> const& names = triggerNames.triggerNames();
    if(first==true){
        for (unsigned index = 0; index < triggerNames.size(); ++index) {
            cout << index << " " << triggerNames.triggerName(index) << " "<< triggerBits->accept(index)<<endl;
            label.str("");
            label<<"bit"<<index;
            cout<<label.str()<<endl;
        }

        std::string REDIname ("_Diphoton30_18_R9Id_OR_IsoCaloId_AND_HE_R9Id_Mass95_v1");                     
        std::string EBPVname ("_Diphoton30EB_18EB_R9Id_OR_IsoCaloId_AND_HE_R9Id_DoublePixelVeto_Mass55_v1"); 
        std::string ANDPVname ("_Diphoton30PV_18PV_R9Id_AND_IsoCaloId_AND_HE_R9Id_DoublePixelVeto_Mass55_v1");
        std::string ANDSoname ("_Diphoton30_18_Solid_R9Id_AND_IsoCaloId_AND_HE_R9Id_Mass55_v1");              

        std::cout<<REDIname<<" "<<triggerNames.triggerName(309)<<std::endl;
        std::cout<<EBPVname<<" "<<triggerNames.triggerName(313)<<std::endl;
        std::cout<<ANDPVname<<" "<<triggerNames.triggerName(311)<<std::endl;
        std::cout<<ANDSoname<<" "<<triggerNames.triggerName(312)<<std::endl;

        first=false;
    }

    //std::cout << "sizes:" << diphotons->size() << "," << diphotons1->size() << "," << diphotons2->size() << std::endl;

    if(diphotons->size()==1){
    for( size_t idipho = 0; idipho < diphotons->size(); idipho++ ) {
        Ptr<flashgg::DiPhotonCandidate> dipho = diphotons->ptrAt( idipho );

        //std::cout<<"dR "<< deltaR(dipho->leadingPhoton()->p4(), dipho->subLeadingPhoton()->p4() )<<std::endl;
       
        //checking if preselected in 1
        bool preselected_in_1 = false;
        for( size_t idipho1 = 0; idipho1 < diphotons1->size(); idipho1++ ) {
            Ptr<flashgg::DiPhotonCandidate> dipho1 = diphotons1->ptrAt( idipho1 );
            if(    dipho->eta() == dipho1->eta() 
                   && dipho->phi() == dipho1->phi()
                   && dipho->mass() == dipho1->mass() ) {
                preselected_in_1 = true;
            }
        }

        //checking if preselected in 2
        bool preselected_in_2 = false;
        for( size_t idipho2 = 0; idipho2 < diphotons2->size(); idipho2++ ) {
            Ptr<flashgg::DiPhotonCandidate> dipho2 = diphotons2->ptrAt( idipho2 );
            if( dipho->eta() == dipho2->eta() 
                && dipho->phi() == dipho2->phi()
                && dipho->mass() == dipho2->mass() ) {
                preselected_in_2 = true;
            }
        }

        assert(preselected_in_2==preselected_in_1);
        /*        
          309 HLT_Diphoton30_18_R9Id_OR_IsoCaloId_AND_HE_R9Id_Mass95_v1 1
          313 HLT_Diphoton30EB_18EB_R9Id_OR_IsoCaloId_AND_HE_R9Id_DoublePixelVeto_Mass55_v1 0
          311 HLT_Diphoton30PV_18PV_R9Id_AND_IsoCaloId_AND_HE_R9Id_DoublePixelVeto_Mass55_v1 1
          312 HLT_Diphoton30_18_Solid_R9Id_AND_IsoCaloId_AND_HE_R9Id_Mass55_v1 1
        */

        bool rediscovery=triggerBits->accept(309);
        bool EBPV=triggerBits->accept(313);
        bool ANDPV=triggerBits->accept(311);
        //bool ANDSolid=triggerBits->accept(312);
        
        //bool haseveto = dipho->leadingPhoton()->passElectronVeto()&&dipho->subLeadingPhoton()->passElectronVeto();
        bool haspveto = !dipho->leadingPhoton()->hasPixelSeed()&&!dipho->subLeadingPhoton()->hasPixelSeed();

        //bool masscut = false;
        //if(dipho->mass()>60) masscut=true;
        


        bool barrel=false;
        if(fabs(dipho->leadingPhoton()->superCluster()->eta())<1.4442&&fabs(dipho->subLeadingPhoton()->superCluster()->eta())<1.4442){
            barrel=true;
        }
        bool rediscovery_preselection=preselected_in_1&&haspveto;
        bool lowmass_preselection_EBPV=preselected_in_1&&haspveto&&barrel;
        
        double mass=dipho->mass();
        bool kinematic_cuts=dipho->leadingPhoton()->pt() > 18 && dipho->subLeadingPhoton()->pt() > 18  && mass > 55
            && abs(dipho->leadingPhoton()->superCluster()->eta())<2.5 && abs(dipho->subLeadingPhoton()->superCluster()->eta())<2.5 
            && (dipho->leadingPhoton()->full5x5_r9()>0.8||dipho->leadingPhoton()->egChargedHadronIso()<20||dipho->leadingPhoton()->egChargedHadronIso()/dipho->leadingPhoton()->pt()<0.3)
            && (dipho->subLeadingPhoton()->full5x5_r9()>0.8||dipho->subLeadingPhoton()->egChargedHadronIso()<20||dipho->subLeadingPhoton()->egChargedHadronIso()/dipho->subLeadingPhoton()->pt()<0.3)
            && haspveto
            && dipho->leadingPhoton()->hadTowOverEm()<0.008 && dipho->subLeadingPhoton()->hadTowOverEm()<0.008;
        
        bool BarrelBarrelCase = (  (     (    fabs(dipho->leadingPhoton()->superCluster()->eta())<1.4442 && fabs(dipho->subLeadingPhoton()->superCluster()->eta())<1.4442 )
                                         && (    (   dipho->leadingPhoton()->full5x5_r9()>0.85)
                                                 || (   dipho->leadingPhoton()->full5x5_r9()<=0.85 && dipho->leadingPhoton()->full5x5_r9()>0.5
                                                        && dipho->leadingPhoton()->full5x5_sigmaIetaIeta()<0.015
                                                        && dipho->leadingPhoton()->pfPhoIso03()<4.0 
                                                        && dipho->leadingPhoton()->trkSumPtHollowConeDR03()<6.0) )
                                         && (    (   dipho->subLeadingPhoton()->full5x5_r9()>0.85)  
                                                 || (   dipho->subLeadingPhoton()->full5x5_r9()<=0.85 && dipho->subLeadingPhoton()->full5x5_r9()>0.5
                                                        && dipho->subLeadingPhoton()->full5x5_sigmaIetaIeta()<0.015
                                                        && dipho->subLeadingPhoton()->pfPhoIso03()<4.0
                                                        && dipho->subLeadingPhoton()->trkSumPtHollowConeDR03()<6.0) )  ));

        bool BarrelBarrelCaseAND = (    fabs(dipho->leadingPhoton()->superCluster()->eta())<1.4442 && fabs(dipho->subLeadingPhoton()->superCluster()->eta())<1.4442 )
                                            && (    (   dipho->leadingPhoton()->full5x5_r9()>0.85))
                                            && (    (   dipho->subLeadingPhoton()->full5x5_r9()>0.85)  );
                                            
        
        bool noBarrelBarrelCase = ((    (    fabs(dipho->leadingPhoton()->superCluster()->eta())<1.4442&& fabs(dipho->subLeadingPhoton()->superCluster()->eta())>1.566 )
                                        && (    dipho->leadingPhoton()->full5x5_r9()>0.85
                                                && dipho->leadingPhoton()->full5x5_sigmaIetaIeta()<0.015
                                                && dipho->leadingPhoton()->pfPhoIso03()<4.0
                                                && dipho->leadingPhoton()->trkSumPtHollowConeDR03()<6.0)
                                        && (    dipho->subLeadingPhoton()->full5x5_r9()>0.90
                                                && dipho->subLeadingPhoton()->full5x5_sigmaIetaIeta()<0.035
                                                && dipho->subLeadingPhoton()->pfPhoIso03()<4.0
                                                && dipho->subLeadingPhoton()->trkSumPtHollowConeDR03()<6.0)  )

                                   || (   (     fabs(dipho->subLeadingPhoton()->superCluster()->eta())<1.4442 && fabs(dipho->leadingPhoton()->superCluster()->eta())>1.566 )
                                          && (    dipho->subLeadingPhoton()->full5x5_r9()>0.85
                                                  && dipho->subLeadingPhoton()->full5x5_sigmaIetaIeta()<0.015
                                                  && dipho->subLeadingPhoton()->pfPhoIso03()<4.0
                                                  && dipho->subLeadingPhoton()->trkSumPtHollowConeDR03()<6.0)
                                          && (    dipho->leadingPhoton()->full5x5_r9()>0.90
                                                  && dipho->leadingPhoton()->full5x5_sigmaIetaIeta()<0.035
                                                  && dipho->leadingPhoton()->pfPhoIso03()<4.0
                                                  && dipho->leadingPhoton()->trkSumPtHollowConeDR03()<6.0)  )
                                   
                                   || (   (     fabs(dipho->subLeadingPhoton()->superCluster()->eta())>1.566 && fabs(dipho->leadingPhoton()->superCluster()->eta())>1.566)
                                          && (    dipho->subLeadingPhoton()->full5x5_r9()>0.90
                                                  && dipho->subLeadingPhoton()->full5x5_sigmaIetaIeta()<0.035
                                                  && dipho->subLeadingPhoton()->pfPhoIso03()<4.0
                                                  && dipho->subLeadingPhoton()->trkSumPtHollowConeDR03()<6.0)
                                          && (    dipho->leadingPhoton()->full5x5_r9()>0.90
                                                  && dipho->leadingPhoton()->full5x5_sigmaIetaIeta()<0.035
                                                  && dipho->leadingPhoton()->pfPhoIso03()<4.0
                                                  && dipho->leadingPhoton()->trkSumPtHollowConeDR03()<6.0)
                                          )  )
            ;
        
        
        bool lowmass_preselection_EBPVorANDPV=kinematic_cuts&&(BarrelBarrelCase||noBarrelBarrelCase);

        bool lowmass_preselection_ANDPV=kinematic_cuts&&(BarrelBarrelCaseAND||noBarrelBarrelCase);

        //bool lowmass_preselection_ANDSo=preselected_in_1&&haspveto;
        
        assert(lowmass_preselection_EBPV||lowmass_preselection_ANDPV==lowmass_preselection_EBPVorANDPV);
        
        
        bool normcuts04627=false;
        if(dipho->leadingPhoton()->pt()/dipho->mass()> 0.46 && dipho->subLeadingPhoton()->pt()/dipho->mass()> 0.27)
            normcuts04627=true;
        
        bool normcuts04727=false;
        if(dipho->leadingPhoton()->pt()/dipho->mass()> 0.47 && dipho->subLeadingPhoton()->pt()/dipho->mass()> 0.27)
            normcuts04727=true;
        
        bool normcuts04827=false;
        if(dipho->leadingPhoton()->pt()/dipho->mass()> 0.48 && dipho->subLeadingPhoton()->pt()/dipho->mass()> 0.27)
            normcuts04827=true;
        
        bool normcuts04927=false;
        if(dipho->leadingPhoton()->pt()/dipho->mass()> 0.49 && dipho->subLeadingPhoton()->pt()/dipho->mass()> 0.27)
            normcuts04927=true;
            
        /*
        cut[0] = EBPV||ANDPV; //all minitree level plots for reference, no cuts
        
        cut[1] = EBPV||ANDPV;
        cut[2] = cut[1]&&normcuts04627;
        
        cut[3] = EBPV||ANDPV;
        cut[4] = cut[3]&&normcuts04727;
        
        cut[5] = EBPV||ANDPV;
        cut[6] = cut[5]&&normcuts04827;
        
        cut[7] = EBPV||ANDPV;
        cut[8] = cut[7]&&normcuts04927;
        */

        cut[0] = EBPV||ANDPV||rediscovery;
        
        cut[1] = rediscovery;
        cut[2] = cut[1]&&rediscovery_preselection;
        
        cut[3] = EBPV;
        cut[4] = cut[3]&&lowmass_preselection_EBPV;
        
        cut[5] = ANDPV;
        cut[6] = cut[5]&&lowmass_preselection_ANDPV;
        
        cut[7] = EBPV||ANDPV;
        cut[8] = cut[7]&&lowmass_preselection_EBPVorANDPV;
        
        //std::cout<<(EBPV&&lowmass_preselection_EBPV)<<std::endl;
        
        for( cut_index = 0; cut_index < 9  ; cut_index++ ) { //Loop over the different histograms
            if( cut[cut_index] ) { //all hitograms below will be filled up if the boolean is true.
                cutflow->Fill( cut_index );
                phi1phi2[cut_index]->Fill( dipho->leadingPhoton()->phi(), dipho->subLeadingPhoton()->phi() );
                eta1eta2[cut_index]->Fill( dipho->leadingPhoton()->eta(), dipho->subLeadingPhoton()->eta() );
                pt1pt2[cut_index]->Fill( dipho->leadingPhoton()->pt(), dipho->subLeadingPhoton()->pt() );
                pt1pt2Norm[cut_index]->Fill( dipho->leadingPhoton()->pt() / dipho->mass(), dipho->subLeadingPhoton()->pt() / dipho->mass() );
                pt1pt2Zoom[cut_index]->Fill( dipho->leadingPhoton()->pt() / dipho->mass(), dipho->subLeadingPhoton()->pt() / dipho->mass() );
                wide_pt1pt2[cut_index]->Fill( dipho->leadingPhoton()->pt(), dipho->subLeadingPhoton()->pt() );
                higgsEta[cut_index]->Fill( dipho->eta() );
                higgsPhi[cut_index]->Fill( dipho->phi() );
                higgsP[cut_index]->Fill( dipho->p() );
                higgsPt[cut_index]->Fill( dipho->pt() );
                ptLead[cut_index]->Fill( dipho->leadingPhoton()->pt() );
                ptTrail[cut_index]->Fill( dipho->subLeadingPhoton()->pt() );
                ptLeadNorm[cut_index]->Fill( dipho->leadingPhoton()->pt() / dipho->mass() );
                ptTrailNorm[cut_index]->Fill( dipho->subLeadingPhoton()->pt() / dipho->mass() );
                massHiggshisto[cut_index]->Fill( dipho->mass() );
                massDiphotonhisto[cut_index]->Fill( dipho->mass() );

                diphoton_LeadR9[cut_index]->Fill( dipho->leadingPhoton()->r9() );
                diphoton_SubLeadR9[cut_index]->Fill( dipho->subLeadingPhoton()->r9() );
                /*
                  diphoton_LeadTrkIso03[cut_index]->Fill(dipho->leadingPhoton()->TrkIso03);
                  diphotonsubLeadTrkIso03[cut_index]->Fill(dipho->subLeadingPhoton()->SubLeadTrkIso03);
                  
                  diphoton_LeadHcalIso03[cut_index]->Fill(dipho->leadingPhoton()->HcalIso03);
                  diphotonsubLeadHcalIso03[cut_index]->Fill(dipho->subLeadingPhoton()->HcalIso03);
                  
                  diphoton_Leadpfiso02[cut_index]->Fill(dipho->leadingPhoton()->pfiso02);
                  diphotonsubLeadpfiso02[cut_index]->Fill(dipho->subLeadingPhoton()->pfiso02);
                */
            }
        }
    }
}


void
LowMassValidation::beginJob()
{
    theFileOut = new TFile( "output_hlt.root", "RECREATE" );

    cutflow = new TH1F( "cutflow", "cutflow", 9  , -0.5, 8.5 );

    for( cut_index = 0; cut_index < 9  ; cut_index++ ) { //Loop over the different histograms
        name = Form( "ptLead_%d", cut_index );
        ptLead[cut_index]            = new TH1F( name, name, 100, 0., 120. );
        name = Form( "ptTrail_%d", cut_index );
        ptTrail[cut_index]           = new TH1F( name, name, 100, 0., 120. );
        name = Form( "ptLeadNorm_%d", cut_index );
        ptLeadNorm[cut_index]        = new TH1F( name, name, 100, 0., 1. );
        name = Form( "ptTrailNorm_%d", cut_index );
        ptTrailNorm[cut_index]       = new TH1F( name, name, 100, 0., 1. );
        name = Form( "massDiphoton_%d", cut_index );
        massDiphotonhisto[cut_index] = new TH1F( name, name, 15000, 30., 180. );
        name = Form( "massHiggs_%d", cut_index );
        massHiggshisto[cut_index]    = new TH1F( name, name, 100, 30., 180. );
        name = Form( "higgsEta_%d", cut_index );
        higgsEta[cut_index]          = new TH1F( name, name, 100, -5, 5 );
        name = Form( "higgsPhi_%d", cut_index );
        higgsPhi[cut_index]          = new TH1F( name, name, 180, -3.15, 3.15 );
        name = Form( "higgsP_%d", cut_index );
        higgsP[cut_index]            = new TH1F( name, name, 500, 0, 1200 );
        name = Form( "higgsPt_%d", cut_index );
        higgsPt[cut_index]           = new TH1F( name, name, 500, 0, 1200 );
        name = Form( "phi1phi2_%d", cut_index );
        phi1phi2[cut_index]          = new TH2F( name, name, 180, -3.15, 3.15, 180, -3.15, 3.15 );
        name = Form( "eta1eta2_%d", cut_index );
        eta1eta2[cut_index]          = new TH2F( name, name, 100, -5, 5, 100, -5, 5 );
        name = Form( "pt1pt2_%d", cut_index );
        pt1pt2[cut_index]            = new TH2F( name, name, 100, 0., 110., 100, 0., 110. );
        name = Form( "pt1pt2Norm_%d", cut_index );
        pt1pt2Norm[cut_index]        = new TH2F( name, name, 100, 0., 1., 100, 0., 1. );
        name = Form( "pt1pt2Zoom_%d", cut_index );
        pt1pt2Zoom[cut_index]        = new TH2F( name, name, 20, 0.3, 0.5, 20, 0.2, 0.4 );
        name = Form( "wide_pt1pt2_%d", cut_index );
        wide_pt1pt2[cut_index]       = new TH2F( name, name, 19 - 1, bins, 19 - 1, bins );

        name = Form( "diphoton_LeadR9_%d", cut_index );
        diphoton_LeadR9[cut_index]       = new TH1F( name, name, 100, 0., 1. );
        name = Form( "diphoton_SubLeadR9_%d", cut_index );
        diphoton_SubLeadR9[cut_index]       = new TH1F( name, name, 100, 0., 1. );

        name = Form( "diphoton_LeadHoE_%d", cut_index );
        diphoton_LeadHoE[cut_index]       = new TH1F( name, name, 100, 0., 0.01 );
        name = Form( "diphoton_SubLeadHoE_%d", cut_index );
        diphoton_SubLeadHoE[cut_index]       = new TH1F( name, name, 100, 0., 0.01 );

        name = Form( "diphoton_LeadTrkIso03_%d", cut_index );
        diphoton_LeadTrkIso03[cut_index]       = new TH1F( name, name, 100, 0., 0.01 );
        name = Form( "diphoton_SubLeadTrkIso03_%d", cut_index );
        diphoton_SubLeadTrkIso03[cut_index]       = new TH1F( name, name, 100, 0., 0.01 );

        name = Form( "diphoton_LeadHcalIso03_%d", cut_index );
        diphoton_LeadHcalIso03[cut_index]       = new TH1F( name, name, 100, 0., 0.01 );
        name = Form( "diphoton_SubLeadHcalIso03_%d", cut_index );
        diphoton_SubLeadHcalIso03[cut_index]       = new TH1F( name, name, 100, 0., 0.01 );

        name = Form( "diphoton_Leadpfiso02_%d", cut_index );
        diphoton_Leadpfiso02[cut_index]       = new TH1F( name, name, 100, 0., 0.01 );
        name = Form( "diphoton_SubLeadpfiso02_%d", cut_index );
        diphoton_SubLeadpfiso02[cut_index]       = new TH1F( name, name, 100, 0., 0.01 );
    }

    first=true;
}

void
LowMassValidation::endJob()
{
    theFileOut->cd();
    cutflow->Write();
    for( cut_index = 0; cut_index < 9  ; cut_index++ ) { //Loop over the different histograms
        cout << "index=" << cut_index << endl;
        ptLead[cut_index]->Write();
        ptTrail[cut_index]->Write();
        ptLeadNorm[cut_index]->Write();
        ptTrailNorm[cut_index]->Write();
        massDiphotonhisto[cut_index]->Write();
        massHiggshisto[cut_index]->Write();
        higgsEta[cut_index]->Write();
        higgsPhi[cut_index]->Write();
        higgsP[cut_index]->Write();
        higgsPt[cut_index]->Write();
        phi1phi2[cut_index]->Write();
        eta1eta2[cut_index]->Write();
        pt1pt2[cut_index]->Write();
        pt1pt2Norm[cut_index]->Write();
        pt1pt2Zoom[cut_index]->Write();
        wide_pt1pt2[cut_index]->Write();
        diphoton_LeadR9[cut_index]->Write();
        diphoton_SubLeadR9[cut_index]->Write();
        diphoton_LeadHoE[cut_index]->Write();
        diphoton_SubLeadHoE[cut_index]->Write();
        diphoton_LeadTrkIso03[cut_index]->Write();
        diphoton_SubLeadTrkIso03[cut_index]->Write();
        diphoton_LeadHcalIso03[cut_index]->Write();
        diphoton_SubLeadHcalIso03[cut_index]->Write();
        diphoton_Leadpfiso02[cut_index]->Write();
        diphoton_SubLeadpfiso02[cut_index]->Write();
    }
    theFileOut->Close();

} // end of endJob


typedef LowMassValidation FlashggLowMassValidation;
DEFINE_FWK_MODULE( FlashggLowMassValidation );
// Local Variables:
// mode:c++
// indent-tabs-mode:nil
// tab-width:4
// c-basic-offset:4
// End:
// vim: tabstop=4 expandtab shiftwidth=4 softtabstop=4

