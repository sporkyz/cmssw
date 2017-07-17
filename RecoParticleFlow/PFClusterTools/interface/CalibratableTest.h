#ifndef CALIBRATABLETEST_H_
#define CALIBRATABLETEST_H_

// system include files
#include <memory>
#include <vector>
// user include files
#include "FWCore/Framework/interface/Frameworkfwd.h"
#include "FWCore/Framework/interface/one/EDAnalyzer.h"
#include "FWCore/Framework/interface/Event.h"
#include "FWCore/Framework/interface/MakerMacros.h"
#include "FWCore/ParameterSet/interface/ParameterSet.h"
#include "FWCore/ServiceRegistry/interface/Service.h"
#include "CommonTools/UtilAlgos/interface/TFileService.h"

#include "DataFormats/ParticleFlowReco/interface/Calibratable.h"

#include "DataFormats/ParticleFlowCandidate/interface/PFCandidateFwd.h"
#include "DataFormats/ParticleFlowReco/interface/PFSimParticleFwd.h"
#include "DataFormats/ParticleFlowReco/interface/PFRecTrackFwd.h"
#include "DataFormats/ParticleFlowReco/interface/PFRecHitFwd.h"
#include "DataFormats/ParticleFlowReco/interface/PFClusterFwd.h"

#include <TTree.h>


/**
 * \class CalibratableTest
 * \brief EDAnalyzer to exercise and demonstrate usage of Calibratable tree
 * \author Jamie Ballin, Imperial College London
 * \date November 2008
 * 
 * OBJECTIVE: this analyzer will create a tree of PFClusterTools/Calibratable
 * instances. Each entry of the tree represents particle flow information relating
 * to one pion in a multi-pion event. The use of monte carlo is assumed as
 * sim particle information is extracted too.
 * 
 * USAGE: This is an analyzer, not a producer: the tree it produces is stored in a seperate
 * ROOT file using the CMSSW TFileService. 
 * 
 * Consult test/CalibratableTest_FastSim.py for a sample CMSSW configuration.
 * Run the example from that directory with,
 * 		cmsRun CalibratableTest_FastSim.py
 * This will produce a file with a tree inside. Browse the contents in bare root 
 * with a TBrowser.
 * 
 * NOTE: This analyzer does not exercise the complete functionality of the Calibratable
 * class. For instance, tracks and rechits are not stored. Contact me (the author)
 * for more details, and for an analyzer that we use to extract particle information
 * to calibrate the particle flow algorithm. A MUCH more involved example from my
 * private analysis is to be found in,
 * 		UserCode/JamieBallin/interface/DipionDelegate.h
 * 
 * General details about the usage of Calibratable may be found at,
 * 		https://twiki.cern.ch/twiki/bin/view/CMS/PFClusterToolsPackage
 */
class CalibratableTest : public edm::one::EDAnalyzer<> {
public:
	explicit CalibratableTest(const edm::ParameterSet&);
	~CalibratableTest();

	/*
	 * Returns the collection in the event matching the Handle.
	 */
	template<class T> void getCollection(edm::Handle<T>& c,
			const edm::InputTag& tag, const edm::Event& event) const;

private:
	/* 
	 * The usual EDAnalyzer methods
	 */
	virtual void beginJob() override;
	virtual void analyze(const edm::Event&, const edm::EventSetup&) override;
	virtual void endJob() override;

	/*
	 * Called for each particle in the event to fill the tree and
	 * reset calib_.
	 */
	virtual void fillTreeAndReset();

	/*
	 * Finds the main pions generated by FastSim
	 */
	std::vector<unsigned> findPrimarySimParticles(
			const std::vector<reco::PFSimParticle>& sims);

	/*
	 * Finds PFCandidates within some deltaR of the supplied sim particle.
	 */
	std::vector<unsigned> findCandidatesInDeltaR(const reco::PFSimParticle& pft,
			const std::vector<reco::PFCandidate>& cands, const double& deltaR);

	/*
	 * Fills calib_ with PFCandidate information.
	 */
	void extractCandidate(const reco::PFCandidate& cand);

	/*
	 * Computes delta R between two objects.
	 */
	double deltaR(const double& eta1, const double& eta2, const double& phi1,
			const double& phi2);

	//Calibratable tree
	TTree* tree_;

	//This analyzer produces a file with a tree so we need,
	edm::Service<TFileService> fileservice_;

	//Debug?
	int debug_;

	//True if this event was generally successful: 
	//if no sim particles are found, this will be false
	bool thisEventPasses_;
	//True if this particle should be written to the tree
	//If no PFCandidates are found within a delta R cone of the sim particle,
	//this will be false.
	bool thisParticlePasses_;

	//The Calibratable object
	pftools::Calibratable* calib_;

	//Increment nWrites_ for every record written to the tree
	//Incremement nFails_ for every event/particle not written to the tree (thisEventPasses_ == false)
	unsigned nParticleWrites_, nParticleFails_;
	unsigned nEventWrites_, nEventFails_;
	
	//Size of cone around sim particle to look for PFCandidates
	double deltaRCandToSim_;

	//Collection input tags
	edm::InputTag inputTagCandidates_;
	edm::InputTag inputTagSimParticles_;
	edm::InputTag inputTagClustersEcal_;
	edm::InputTag inputTagClustersHcal_;

	//collections
	edm::Handle<reco::PFCandidateCollection>* pfCandidates_;
	edm::Handle<reco::PFSimParticleCollection>* simParticles_;
	edm::Handle<reco::PFClusterCollection>* clustersEcal_;
	edm::Handle<reco::PFClusterCollection>* clustersHcal_;

};

template<class T> void CalibratableTest::getCollection(edm::Handle<T>& c,
		const edm::InputTag& tag, const edm::Event& event) const {

	try {
		event.getByLabel(tag, c);
		if(!c.isValid()) {
			std::cout << "Warning! Collection for label " << tag << " is not valid!" << std::endl;
		}
	}
	catch (cms::Exception& err) {
		std::cout << "Couldn't get collection\n";
		//std::ostringstream s;
		//LogError("Error getting collection!") << s;
	}
}

#endif /*CALIBRATABLETEST_H_*/
