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
//
//
// G4PSTermination
#include "G4PSTermination.hh"

///////////////////////////////////////////////////////////////////////////////
// (Description)
//   This is a primitive scorer class for scoring only terminated traks inside
//  the cell.
//
// Created: 2007-02-02  Tsukasa ASO, Akinori Kimura.
// 2010-07-22   Introduce Unit specification.
// 2011-09-09   Modify comment in PrintAll().
//
///////////////////////////////////////////////////////////////////////////////

G4PSTermination::G4PSTermination(const G4String& name, G4int depth)
  : G4VPrimitiveScorer(name, depth)
{
  SetUnit("");
}

G4bool G4PSTermination::ProcessHits(G4Step* aStep, G4TouchableHistory*)
{
  if(aStep->GetTrack()->GetTrackStatus() != fStopAndKill)
    return false;

  G4int index  = GetIndex(aStep);
  G4double val = 1.0;
  if(weighted)
    val *= aStep->GetPreStepPoint()->GetWeight();
  EvtMap->add(index, val);
  return true;
}

void G4PSTermination::Initialize(G4HCofThisEvent* HCE)
{
  EvtMap = new G4THitsMap<G4double>(detector->GetName(), GetName());
  if(HCID < 0)
  {
    HCID = GetCollectionID(0);
  }
  HCE->AddHitsCollection(HCID, (G4VHitsCollection*) EvtMap);
}

void G4PSTermination::clear() { EvtMap->clear(); }

void G4PSTermination::PrintAll()
{
  G4cout << " MultiFunctionalDet  " << detector->GetName() << G4endl;
  G4cout << " PrimitiveScorer " << GetName() << G4endl;
  G4cout << " Number of entries " << EvtMap->entries() << G4endl;
  for(const auto& [copy, terminations] : *(EvtMap->GetMap()))
  {
    G4cout << "  copy no.: " << copy
           << "  terminations: " << *(terminations) << G4endl;
  }
}

void G4PSTermination::SetUnit(const G4String& unit)
{
  if(unit.empty())
  {
    unitName  = unit;
    unitValue = 1.0;
  }
  else
  {
    G4String msg = "Invalid unit [" + unit + "] (Current  unit is [" +
                   GetUnit() + "] ) for " + GetName();
    G4Exception("G4PSTermination::SetUnit", "DetPS0017", JustWarning, msg);
  }
}
