import FWCore.ParameterSet.Config as cms

from RecoEgamma.EgammaPhotonProducers.looseChi2Estimator_cfi import *
from RecoEgamma.EgammaPhotonProducers.propAlongMomentumWithMaterialForElectrons_cfi import *
from RecoEgamma.EgammaPhotonProducers.propOppoMomentumWithMaterialForElectrons_cfi import *
#TrajectoryBuilder
from RecoTracker.CkfPattern.CkfTrajectoryBuilder_cff import *
import RecoTracker.CkfPattern.CkfTrajectoryBuilder_cfi
TrajectoryBuilderForConversions = RecoTracker.CkfPattern.CkfTrajectoryBuilder_cfi.CkfTrajectoryBuilder.clone()
TrajectoryBuilderForConversions.estimator = 'eleLooseChi2'
TrajectoryBuilderForConversions.TTRHBuilder = 'WithTrackAngle'
TrajectoryBuilderForConversions.updator = 'KFUpdator'
TrajectoryBuilderForConversions.propagatorAlong = 'alongMomElePropagator'
TrajectoryBuilderForConversions.propagatorOpposite = 'oppositeToMomElePropagator'
TrajectoryBuilderForConversions.trajectoryFilter.refToPSet_ = 'TrajectoryFilterForConversions'
TrajectoryBuilderForConversions.maxCand = 5
TrajectoryBuilderForConversions.lostHitPenalty = 30.
TrajectoryBuilderForConversions.intermediateCleaning = True
TrajectoryBuilderForConversions.alwaysUseInvalidHits = True

