//
// ********************************************************************
// * License and Disclaimer                                           *
// *                                                                  *
// * The  Geant4 software  is  copyright of the Copyright Holders  of *
// * the Geant4 Collaboration.  It is provided  under  the terms  and *
// * conditions of the Geant4 Software License,  included in the file *
// * LICENSE and available at  http://cern.ch/geant4/license .  These *
// * include a list of copyright holders.                             *
// *                                                                  *
// * Neither the authors of this software system, nor their employing *
// * institutes,nor the agencies providing financial support for this *
// * work  make  any representation or  warranty, express or implied, *
// * regarding  this  software system or assume any liability for its *
// * use.  Please see the license in the file  LICENSE  and URL above *
// * for the full disclaimer and the limitation of liability.         *
// *                                                                  *
// * This  code  implementation is the result of  the  scientific and *
// * technical work of the GEANT4 collaboration.                      *
// * By using,  copying,  modifying or  distributing the software (or *
// * any work based  on the software)  you  agree  to acknowledge its *
// * use  in  resulting  scientific  publications,  and indicate your *
// * acceptance of all terms of the Geant4 Software license.          *
// ********************************************************************
//
// G4RichTrajectory
//
// Class description:
//
// This class extends G4Trajectory, which includes the following:
//   1) List of trajectory points which compose the trajectory,
//   2) static information of particle which generated the
//      trajectory,
//   3) trackID and parent particle ID of the trajectory.
// The extended information, only publicly accessible through AttValues,
// includes:
//   4) physical volume and next physical volume;
//   5) creator process;
// ...and much more.

// Contact:
//   Questions and comments on G4Trajectory should be sent to
//     Katsuya Amako  (e-mail: Katsuya.Amako@kek.jp)
//     Makoto  Asai   (e-mail: asai@slac.stanford.edu)
//     Takashi Sasaki (e-mail: Takashi.Sasaki@kek.jp)
//   and on the extended code to:
//     John Allison   (e-mail: John.Allison@manchester.ac.uk)
//     Joseph Perl    (e-mail: perl@slac.stanford.edu)
// --------------------------------------------------------------------
#ifndef G4RICHTRAJECTORY_HH
#define G4RICHTRAJECTORY_HH

#include "G4TouchableHandle.hh"
#include "G4VTrajectory.hh"
#include "G4ParticleDefinition.hh"  // Include from 'particle+matter'
#include "G4Step.hh"
#include "G4Track.hh"
#include "G4RichTrajectoryPoint.hh"  // Include from 'tracking'
#include "G4ios.hh"  // Include from 'system'
#include "globals.hh"  // Include from 'global'

#include "trkgdefs.hh"
#include <stdlib.h>  // Include from 'system'

#include <vector>

class G4Polyline;
class G4ClonedRichTrajectory;

class G4RichTrajectory : public G4VTrajectory
{
  using G4TrajectoryPointContainer = std::vector<G4VTrajectoryPoint*>;

  friend class G4ClonedRichTrajectory;

 public:
  // Constructors/destructor
  //
  G4RichTrajectory() = default;
  G4RichTrajectory(const G4Track* aTrack);
  ~G4RichTrajectory() override;
  G4RichTrajectory(G4RichTrajectory&);
  G4RichTrajectory& operator=(const G4RichTrajectory&) = delete;

  // Operators
  //
  G4bool operator==(const G4RichTrajectory& r) const { return (this == &r); }

  inline void* operator new(size_t);
  inline void operator delete(void*);

  // cloning with the master thread allocator
  G4VTrajectory* CloneForMaster() const override;

  // Get/Set functions

  inline G4int GetTrackID() const override { return fTrackID; }
  inline G4int GetParentID() const override { return fParentID; }
  inline G4String GetParticleName() const override { return ParticleName; }
  inline G4double GetCharge() const override { return PDGCharge; }
  inline G4int GetPDGEncoding() const override { return PDGEncoding; }
  inline G4double GetInitialKineticEnergy() const { return initialKineticEnergy; }
  inline G4ThreeVector GetInitialMomentum() const override { return initialMomentum; }

  // Other (virtual) member functions
  //
  void ShowTrajectory(std::ostream& os = G4cout) const override;
  void DrawTrajectory() const override;
  void AppendStep(const G4Step* aStep) override;
  void MergeTrajectory(G4VTrajectory* secondTrajectory) override;
  inline G4int GetPointEntries() const override;
  inline G4VTrajectoryPoint* GetPoint(G4int i) const override;

  G4ParticleDefinition* GetParticleDefinition();

  // Get methods for HepRep style attributes
  //
  const std::map<G4String, G4AttDef>* GetAttDefs() const override;
  std::vector<G4AttValue>* CreateAttValues() const override;

 private:
  G4TrajectoryPointContainer* positionRecord = nullptr;
  G4int fTrackID = 0;
  G4int fParentID = 0;
  G4int PDGEncoding = 0;
  G4double PDGCharge = 0.0;
  G4String ParticleName = "dummy";
  G4double initialKineticEnergy = 0.0;
  G4ThreeVector initialMomentum;

  // Extended information (only publicly accessible through AttValues)...
  //
  G4TrajectoryPointContainer* fpRichPointContainer = nullptr;
  G4TouchableHandle fpInitialVolume;
  G4TouchableHandle fpInitialNextVolume;
  const G4VProcess* fpCreatorProcess = nullptr;
  G4int fCreatorModelID = 0;
  G4TouchableHandle fpFinalVolume;
  G4TouchableHandle fpFinalNextVolume;
  const G4VProcess* fpEndingProcess = nullptr;
  G4double fFinalKineticEnergy = 0.0;
};

extern G4TRACKING_DLL G4Allocator<G4RichTrajectory>*& aRichTrajectoryAllocator();

inline void* G4RichTrajectory::operator new(size_t)
{
  if (aRichTrajectoryAllocator() == nullptr) {
    aRichTrajectoryAllocator() = new G4Allocator<G4RichTrajectory>;
  }
  return (void*)aRichTrajectoryAllocator()->MallocSingle();
}

inline void G4RichTrajectory::operator delete(void* aRichTrajectory)
{
  aRichTrajectoryAllocator()->FreeSingle((G4RichTrajectory*)aRichTrajectory);
}

inline G4int G4RichTrajectory::GetPointEntries() const
{
  return G4int(fpRichPointContainer->size());
}

inline G4VTrajectoryPoint* G4RichTrajectory::GetPoint(G4int i) const
{
  return (*fpRichPointContainer)[i];
}

#endif
