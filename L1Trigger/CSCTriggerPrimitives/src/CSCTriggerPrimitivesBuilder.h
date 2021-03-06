#ifndef L1Trigger_CSCTriggerPrimitives_CSCTriggerPrimitivesBuilder_h
#define L1Trigger_CSCTriggerPrimitives_CSCTriggerPrimitivesBuilder_h

/** class CSCTriggerPrimitivesBuilder
 *
 * Algorithm to build anode, cathode, and correlated LCTs from wire and
 * comparator digis in endcap muon CSCs by implementing a 'build' function
 * required by CSCTriggerPrimitivesProducer.
 *
 * Configured via the Producer's ParameterSet.
 *
 * author Slava Valuev, UCLA.
 *
 * The builder was expanded to use GEM pad or GEM pad clusters
 * In addition the builder can produce GEM coincidence pads in
 * case an upgrade scenario with GEMs is run.
 *
 * authors: Sven Dildick (TAMU), Tao Huang (TAMU)
 */

#include "CondFormats/CSCObjects/interface/CSCBadChambers.h"
#include "DataFormats/CSCDigi/interface/CSCComparatorDigiCollection.h"
#include "DataFormats/CSCDigi/interface/CSCWireDigiCollection.h"
#include "DataFormats/CSCDigi/interface/CSCALCTDigiCollection.h"
#include "DataFormats/CSCDigi/interface/CSCCLCTDigiCollection.h"
#include "DataFormats/CSCDigi/interface/CSCCLCTPreTriggerDigiCollection.h"
#include "DataFormats/CSCDigi/interface/CSCCorrelatedLCTDigiCollection.h"
#include "DataFormats/CSCDigi/interface/CSCCLCTPreTriggerCollection.h"
#include "DataFormats/GEMDigi/interface/GEMPadDigiCollection.h"
#include "DataFormats/GEMDigi/interface/GEMPadDigiClusterCollection.h"
#include "DataFormats/GEMDigi/interface/GEMCoPadDigiCollection.h"
#include "FWCore/ParameterSet/interface/ParameterSet.h"
#include "FWCore/MessageLogger/interface/MessageLogger.h"

class CSCDBL1TPParameters;
class CSCMotherboard;
class CSCMuonPortCard;
class CSCGeometry;
class GEMGeometry;
class CSCTriggerPrimitivesBuilder
{
 public:

  /** Configure the algorithm via constructor.
   *  Receives ParameterSet percolated down from EDProducer which owns this
   *  Builder.
   */
  explicit CSCTriggerPrimitivesBuilder(const edm::ParameterSet&);

  ~CSCTriggerPrimitivesBuilder();

  /** Sets configuration parameters obtained via EventSetup mechanism. */
  void setConfigParameters(const CSCDBL1TPParameters* conf);

  /// set CSC and GEM geometries for the matching needs
  void setCSCGeometry(const CSCGeometry *g) { csc_g = g; }
  void setGEMGeometry(const GEMGeometry *g) { gem_g = g; }

  /* temporary function to check if running on data.
   * Currently in simulation the central BX is BX6; in data
   * it is BX8. This mismatch in conventions is expeced to
   * be resolved in the near future. The member runOnData_
   * is used in the builder to shift the LCT BX readout
   * with +2 from [3,9] to [5,11].
   */
  void runOnData(bool runOnData) {runOnData_ = runOnData;}

  /** Build anode, cathode, and correlated LCTs in each chamber and fill
   *  them into output collections.  Select up to three best correlated LCTs
   *  in each (sub)sector and put them into an output collection as well. */
  void build(const CSCBadChambers* badChambers,
             const CSCWireDigiCollection* wiredc,
             const CSCComparatorDigiCollection* compdc,
             const GEMPadDigiCollection* gemPads,
             const GEMPadDigiClusterCollection* gemPadClusters,
             CSCALCTDigiCollection& oc_alct,
             CSCCLCTDigiCollection& oc_clct,
             CSCCLCTPreTriggerDigiCollection& oc_clctpretrigger,
             CSCCLCTPreTriggerCollection & oc_pretrig,
             CSCCorrelatedLCTDigiCollection& oc_lct,
             CSCCorrelatedLCTDigiCollection& oc_sorted_lct,
             GEMCoPadDigiCollection& oc_gemcopad);

  /** Max values of trigger labels for all CSCs; used to construct TMB
   *  processors. */
  enum trig_cscs {MAX_ENDCAPS = 2, MAX_STATIONS = 4, MAX_SECTORS = 6,
		  MAX_SUBSECTORS = 2, MAX_CHAMBERS = 9};
 private:

  /** template function to put data in the output
      helps to reduce the large amount of code duplication!
   */
  template <class T, class S>
  void put(const T&, S&, const CSCDetId&, std::string comment);

  /** Min and max allowed values for various CSC elements, defined in
   *  CSCDetId and CSCTriggerNumbering classes. */
  static const int min_endcap;    // endcaps
  static const int max_endcap;
  static const int min_station;   // stations per endcap
  static const int max_station;
  static const int min_sector;    // trigger sectors per station
  static const int max_sector;
  static const int min_subsector; // trigger subsectors per sector
  static const int max_subsector;
  static const int min_chamber;   // chambers per trigger subsector
  static const int max_chamber;

  /// temporary flag to run on data
  bool runOnData_;

  /// a flag whether to skip chambers from the bad chambers map
  bool checkBadChambers_;

  /** SLHC: special configuration parameters for ME11 treatment. */
  bool smartME1aME1b, disableME1a;

  /** SLHC: special switch for disabling ME42 */
  bool disableME42;

  /** SLHC: special switch for the upgrade ME1/1 TMB */
  bool runME11ILT_;

  /** SLHC: special switch for the upgrade ME2/1 TMB */
  bool runME21ILT_;

  /** SLHC: special switch for the upgrade ME3/1 and ME4/1 TMB */
  bool runME3141ILT_;

  /** SLHC: special switch to use gem clusters */
  bool useClusters_;

  int m_minBX, m_maxBX; // min and max BX to sort.

  /** Pointers to TMB processors for all possible chambers. */
  std::unique_ptr<CSCMotherboard>
    tmb_[MAX_ENDCAPS][MAX_STATIONS][MAX_SECTORS][MAX_SUBSECTORS][MAX_CHAMBERS];

  // pointers to the geometry
  const CSCGeometry* csc_g;
  const GEMGeometry* gem_g;

  /** Pointer to MPC processor. */
  std::unique_ptr<CSCMuonPortCard> m_muonportcard;
};

template <class T, class S>
void CSCTriggerPrimitivesBuilder::put(const T& t, S& s, const CSCDetId& detid,
                                      std::string comment)
{
  if (!t.empty()) {
    LogTrace("L1CSCTrigger")
      << "Put " << t.size() << comment
      << ((t.size() > 1) ? "s " : " ") << "in collection\n";
    s.put(std::make_pair(t.begin(),t.end()), detid);
  }
}

#endif
