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
// G4ImportanceProcess
//
// Class description:
//
// Used internally by importance sampling in the "mass" geometry.
// This process is a forced post step process. It will apply
// importance sampling if the particle crosses a boundary in the 
// "mass" geometry.

// Author: Michael Dressel, 2002
// --------------------------------------------------------------------
#ifndef G4ImportanceProcess_hh
#define G4ImportanceProcess_hh 1

#include "G4VProcess.hh"
#include "G4VTrackTerminator.hh"
#include "G4VTouchable.hh"

class G4SamplingPostStepAction;
class G4VImportanceAlgorithm;
class G4VIStore;

class G4Step;
class G4Navigator;
class G4TransportationManager;
class G4PathFinder;

#include "G4FieldTrack.hh"
#include "G4TouchableHandle.hh"
#include "G4MultiNavigator.hh"   // For ELimited enum

class G4ImportanceProcess : public G4VProcess, public G4VTrackTerminator
{

 public:

  G4ImportanceProcess(const G4VImportanceAlgorithm &aImportanceAlgorithm,
                      const G4VIStore &aIstore,
                      const G4VTrackTerminator *TrackTerminator,
                      const G4String &aName = "ImportanceProcess",
                            G4bool para = false);
    // creates a G4ParticleChange

  virtual ~G4ImportanceProcess();
    // delete the G4ParticleChange

  G4ImportanceProcess(const G4ImportanceProcess &) = delete;
  G4ImportanceProcess &operator=(const G4ImportanceProcess &) = delete;

  //--------------------------------------------------------------
  // Set Parallel World
  //--------------------------------------------------------------

  void SetParallelWorld(const G4String& parallelWorldName);

  //--------------------------------------------------------------
  //     Process interface
  //--------------------------------------------------------------

  void StartTracking(G4Track*);
  

  virtual G4double 
  PostStepGetPhysicalInteractionLength(const G4Track& aTrack,
                                       G4double previousStepSize,
                                       G4ForceCondition* condition);
    // make process beeing forced

  virtual G4VParticleChange* PostStepDoIt(const G4Track&, const G4Step&);
    // manage the importance sampling in the "mass" geometry

  virtual void KillTrack() const;
    // used in case no scoring process follows that does the killing

  virtual const G4String& GetName() const;

  //  no operation in  AtRestDoIt and  AlongStepDoIt

  virtual G4double 
  AlongStepGetPhysicalInteractionLength(const G4Track&,
                                        G4double  ,
                                        G4double  ,
                                        G4double& ,
                                        G4GPILSelection*);
  virtual G4double 
  AtRestGetPhysicalInteractionLength(const G4Track& ,
                                     G4ForceCondition*);
  
  virtual G4VParticleChange* AtRestDoIt(const G4Track&, const G4Step&);
  virtual G4VParticleChange* AlongStepDoIt(const G4Track&, const G4Step&);
  
 private:

  void CopyStep(const G4Step& step);

  G4Step* fGhostStep = nullptr;
  G4StepPoint* fGhostPreStepPoint = nullptr;
  G4StepPoint* fGhostPostStepPoint = nullptr;

  G4ParticleChange* fParticleChange = nullptr;
  const G4VImportanceAlgorithm &fImportanceAlgorithm;
  const G4VIStore& fIStore;
  G4SamplingPostStepAction* fPostStepAction = nullptr;

  G4TransportationManager* fTransportationManager = nullptr;
  G4PathFinder* fPathFinder = nullptr;

  // -------------------------------
  // Navigation in the Ghost World:
  // -------------------------------
  G4String             fGhostWorldName = "NoParallelWorld";
  G4VPhysicalVolume*   fGhostWorld = nullptr;
  G4Navigator*         fGhostNavigator = nullptr;
  G4int                fNavigatorID = -1;
  G4TouchableHandle    fOldGhostTouchable;
  G4TouchableHandle    fNewGhostTouchable;
  G4FieldTrack         fFieldTrack = '0';
  G4double             fGhostSafety = -1;
  G4bool               fOnBoundary = false;

  G4bool               fParaflag = false;
  G4FieldTrack         fEndTrack = '0';
  ELimited             feLimited = kDoNot;
};

#endif
