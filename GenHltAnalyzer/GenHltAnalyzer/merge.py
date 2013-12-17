import FWCore.ParameterSet.Config as cms

process = cms.Process("merge")

process.load("FWCore.MessageService.MessageLogger_cfi")

process.maxEvents = cms.untracked.PSet( input = cms.untracked.int32(-1) )

import FWCore.Utilities.FileUtils as FileUtils
mylist = FileUtils.loadListFromFile('90.txt')
process.source = cms.Source("PoolSource",fileNames =
cms.untracked.vstring( *mylist ))

process.FEVT = cms.OutputModule("PoolOutputModule",
                                fileName = cms.untracked.string('/tmp/carrillo/90.root')
                                )

process.outpath = cms.EndPath(process.FEVT)
