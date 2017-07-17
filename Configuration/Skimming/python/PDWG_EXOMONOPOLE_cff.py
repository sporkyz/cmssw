import FWCore.ParameterSet.Config as cms

# Unprescale HLT_MET and HLT_SinglePhoton triggers
import HLTrigger.HLTfilters.hltHighLevel_cfi
hltMonopole = HLTrigger.HLTfilters.hltHighLevel_cfi.hltHighLevel.clone()
hltMonopole.TriggerResultsTag = cms.InputTag( "TriggerResults", "", "HLT" )
hltMonopole.HLTPaths = cms.vstring(
    "HLT_Photon175_v*",
    "HLT_PFMET300_v*",
    "HLT_PFMET170_HBHE_BeamHaloCleaned_v*"
)
hltMonopole.throw = False
hltMonopole.andOr = True

from Configuration.EventContent.EventContent_cff import AODEventContent
EXOMonopoleSkimContent = AODEventContent.clone()
EXOMonopoleSkimContent.outputCommands.append('keep *_hybridSuperClusters_*_*')
EXOMonopoleSkimContent.outputCommands.append('keep *_multi5x5SuperClusters_multi5x5EndcapSuperClusters_*')
EXOMonopoleSkimContent.outputCommands.append('keep *_multi5x5SuperClusters_uncleanOnlyMulti5x5EndcapBasicClusters_*')
EXOMonopoleSkimContent.outputCommands.append('keep *_multi5x5SuperClusters_uncleanOnlyMulti5x5EndcapSuperClusters_*')
EXOMonopoleSkimContent.outputCommands.append('keep *_siStripClusters_*_*')
EXOMonopoleSkimContent.outputCommands.append('keep *_siPixelClusters_*_*')
EXOMonopoleSkimContent.outputCommands.append('drop *_generalTracks_*_*')
EXOMonopoleSkimContent.outputCommands.append('keep *_generalTracks_*_*')
EXOMonopoleSkimContent.outputCommands.append('drop *_generalTracks_QualityMasks_*')
EXOMonopoleSkimContent.outputCommands.append('keep *_ecalRecHit_EcalRecHitsEB_*')
EXOMonopoleSkimContent.outputCommands.append('keep *_ecalRecHit_EcalRecHitsEE_*')
EXOMonopoleSkimContent.outputCommands.append('keep *_hbhereco_*_*')

# monopole skim sequence
EXOMonopoleSkimSequence = cms.Sequence(
    hltMonopole
    )
