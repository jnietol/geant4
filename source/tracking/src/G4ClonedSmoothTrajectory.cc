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
// G4ClonedSmoothTrajectory class implementation
//
// Makoto Asai (JLab) - Oct.2024
// --------------------------------------------------------------------

#include "G4ClonedSmoothTrajectory.hh"
#include "G4SmoothTrajectory.hh"

#include "G4AttDef.hh"
#include "G4AttDefStore.hh"
#include "G4AttValue.hh"
#include "G4ParticleTable.hh"
#include "G4ClonedSmoothTrajectoryPoint.hh"
#include "G4UIcommand.hh"
#include "G4UnitsTable.hh"
#include "G4AutoLock.hh"

// #define G4ATTDEBUG
#ifdef G4ATTDEBUG
#  include "G4AttCheck.hh"
#endif

G4Allocator<G4ClonedSmoothTrajectory>*& aClonedSmoothTrajectoryAllocator()
{
  static G4Allocator<G4ClonedSmoothTrajectory>* _instance = nullptr;
  return _instance;
}

G4ClonedSmoothTrajectory::G4ClonedSmoothTrajectory(const G4SmoothTrajectory& right)
{
  ParticleName = right.ParticleName;
  PDGCharge = right.PDGCharge;
  PDGEncoding = right.PDGEncoding;
  fTrackID = right.fTrackID;
  fParentID = right.fParentID;
  initialKineticEnergy = right.initialKineticEnergy;
  initialMomentum = right.initialMomentum;
  positionRecord = new G4TrajectoryPointContainer();

  for (auto& i : *right.positionRecord) {
    auto rightPoint = (G4ClonedSmoothTrajectoryPoint*)i;
    positionRecord->push_back(new G4ClonedSmoothTrajectoryPoint(*rightPoint));
  }
}

G4ClonedSmoothTrajectory::~G4ClonedSmoothTrajectory()
{
  if (positionRecord != nullptr) {
    for (auto& i : *positionRecord) {
      delete i;
    }
    positionRecord->clear();
    delete positionRecord;
  }
}

void G4ClonedSmoothTrajectory::ShowTrajectory(std::ostream& os) const
{
  // Invoke the default implementation in G4VTrajectory...
  //
  G4VTrajectory::ShowTrajectory(os);

  // ... or override with your own code here.
}

void G4ClonedSmoothTrajectory::DrawTrajectory() const
{
  // Invoke the default implementation in G4VTrajectory...
  //
  G4VTrajectory::DrawTrajectory();

  // ... or override with your own code here.
}

const std::map<G4String, G4AttDef>* G4ClonedSmoothTrajectory::GetAttDefs() const
{
  G4bool isNew;
  std::map<G4String, G4AttDef>* store = G4AttDefStore::GetInstance("G4ClonedSmoothTrajectory", isNew);
  if (isNew) {
    G4String ID("ID");
    (*store)[ID] = G4AttDef(ID, "Track ID", "Physics", "", "G4int");

    G4String PID("PID");
    (*store)[PID] = G4AttDef(PID, "Parent ID", "Physics", "", "G4int");

    G4String PN("PN");
    (*store)[PN] = G4AttDef(PN, "Particle Name", "Physics", "", "G4String");

    G4String Ch("Ch");
    (*store)[Ch] = G4AttDef(Ch, "Charge", "Physics", "e+", "G4double");

    G4String PDG("PDG");
    (*store)[PDG] = G4AttDef(PDG, "PDG Encoding", "Physics", "", "G4int");

    G4String IKE("IKE");
    (*store)[IKE] = G4AttDef(IKE, "Initial kinetic energy", "Physics", "G4BestUnit", "G4double");

    G4String IMom("IMom");
    (*store)[IMom] = G4AttDef(IMom, "Initial momentum", "Physics", "G4BestUnit", "G4ThreeVector");

    G4String IMag("IMag");
    (*store)[IMag] =
      G4AttDef(IMag, "Initial momentum magnitude", "Physics", "G4BestUnit", "G4double");

    G4String NTP("NTP");
    (*store)[NTP] = G4AttDef(NTP, "No. of points", "Physics", "", "G4int");
  }
  return store;
}

std::vector<G4AttValue>* G4ClonedSmoothTrajectory::CreateAttValues() const
{
  auto values = new std::vector<G4AttValue>;

  values->push_back(G4AttValue("ID", G4UIcommand::ConvertToString(fTrackID), ""));

  values->push_back(G4AttValue("PID", G4UIcommand::ConvertToString(fParentID), ""));

  values->push_back(G4AttValue("PN", ParticleName, ""));

  values->push_back(G4AttValue("Ch", G4UIcommand::ConvertToString(PDGCharge), ""));

  values->push_back(G4AttValue("PDG", G4UIcommand::ConvertToString(PDGEncoding), ""));

  values->push_back(G4AttValue("IKE", G4BestUnit(initialKineticEnergy, "Energy"), ""));

  values->push_back(G4AttValue("IMom", G4BestUnit(initialMomentum, "Energy"), ""));

  values->push_back(G4AttValue("IMag", G4BestUnit(initialMomentum.mag(), "Energy"), ""));

  values->push_back(G4AttValue("NTP", G4UIcommand::ConvertToString(GetPointEntries()), ""));

#ifdef G4ATTDEBUG
  G4cout << G4AttCheck(values, GetAttDefs());
#endif

  return values;
}

void G4ClonedSmoothTrajectory::AppendStep(const G4Step* aStep)
{
  positionRecord->push_back(new G4ClonedSmoothTrajectoryPoint(
    aStep->GetPostStepPoint()->GetPosition(), aStep->GetPointerToVectorOfAuxiliaryPoints()));
}

G4ParticleDefinition* G4ClonedSmoothTrajectory::GetParticleDefinition()
{
  return (G4ParticleTable::GetParticleTable()->FindParticle(ParticleName));
}

void G4ClonedSmoothTrajectory::MergeTrajectory(G4VTrajectory* secondTrajectory)
{
  if (secondTrajectory == nullptr) return;

  auto seco = (G4ClonedSmoothTrajectory*)secondTrajectory;
  G4int ent = seco->GetPointEntries();

  // initial point of the second trajectory should not be merged
  //
  for (G4int i = 1; i < ent; ++i) {
    positionRecord->push_back((*(seco->positionRecord))[i]);
  }
  delete (*seco->positionRecord)[0];
  seco->positionRecord->clear();
}
