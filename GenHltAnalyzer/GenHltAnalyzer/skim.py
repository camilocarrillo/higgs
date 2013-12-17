import FWCore.ParameterSet.Config as cms

process = cms.Process("GENHLTskim")

process.load("FWCore.MessageService.MessageLogger_cfi")

process.maxEvents = cms.untracked.PSet( input = cms.untracked.int32(-1) )

process.source = cms.Source("PoolSource",
                            fileNames = cms.untracked.vstring(
    'file:/afs/cern.ch/user/c/carrillo/workspace/higgs/yy/145CA5DA-3EFC-E111-BA2B-002618943967.root'
    )
                            )

process.FEVT = cms.OutputModule("PoolOutputModule",
                                outputCommands = cms.untracked.vstring('drop *','keep *_TriggerResults_*_HLT', 'keep *_genParticles_*_*'),
                                fileName = cms.untracked.string('/tmp/carrillo/output.root')
                                )


process.outpath = cms.EndPath(process.FEVT)
