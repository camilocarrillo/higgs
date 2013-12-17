import FWCore.ParameterSet.Config as cms

process = cms.Process("Demo")

process.load("FWCore.MessageService.MessageLogger_cfi")

process.maxEvents = cms.untracked.PSet( input = cms.untracked.int32(-1) )

process.source = cms.Source("PoolSource",
    fileNames = cms.untracked.vstring(
#        'file:/afs/cern.ch/user/c/carrillo/workspace/higgs/yy/145CA5DA-3EFC-E111-BA2B-002618943967.root'
	 'file:/tmp/carrillo/130.root'
#        'file:/afs/cern.ch/work/c/carrillo/higgs/yy/90/crab_0_131210_121658/res/output_1_1_3IO.root'
    )
)

process.demo = cms.EDAnalyzer('GenHltAnalyzer',
    GenParticleLabel = cms.InputTag("genParticles"),
    hltInputTag      = cms.InputTag( "TriggerResults::HLT" )
)


process.p = cms.Path(process.demo)
