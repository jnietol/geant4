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
// P. Arce, June-2014 Conversion neutron_hp to particle_hp
//
// June-2019 - E. Mendoza - re-build "two_body_reaction", to be used by
//  incident charged particles (now isotropic emission in the CMS).
//  Also restrict nresp use below 20 MeV (for future developments).
//  Add photon emission when no data available.
//
// V. Ivanchenko, July-2023 Basic revision of particle HP classes

#ifndef G4ParticleHPInelasticCompFS_h
#define G4ParticleHPInelasticCompFS_h 1

#include "G4ParticleHPFinalState.hh"
#include "G4HadFinalState.hh"
#include "G4HadProjectile.hh"
#include "G4NRESP71M03.hh"
#include "G4Nucleus.hh"
#include "G4ParticleHPAngular.hh"
#include "G4ParticleHPDeExGammas.hh"
#include "G4ParticleHPEnAngCorrelation.hh"
#include "G4ParticleHPEnergyDistribution.hh"
#include "G4ParticleHPPhotonDist.hh"
#include "globals.hh"

class G4ParticleHPInelasticCompFS : public G4ParticleHPFinalState
{
 public:

  G4ParticleHPInelasticCompFS();
  ~G4ParticleHPInelasticCompFS() override;

  void Init(G4double A, G4double Z, G4int M, const G4String& dirName,
            const G4String& aSFType, G4ParticleDefinition*) override;

  void InitGammas(G4double AR, G4double ZR);

  G4HadFinalState* ApplyYourself(const G4HadProjectile& theTrack) override = 0;

  G4ParticleHPFinalState* New() override = 0;

  G4double GetXsec(G4double anEnergy) const override
  {
    return std::max(0., theXsection[50]->GetY(anEnergy));
  }

  G4ParticleHPVector* GetXsec() const override { return theXsection[50]; }

  G4int SelectExitChannel(G4double eKinetic);

  void CompositeApply(const G4HadProjectile& theTrack,
                      G4ParticleDefinition* aHadron);

  void InitDistributionInitialState(G4ReactionProduct& anIncidentPart,
				    G4ReactionProduct& aTarget, G4int it);

  G4ParticleHPInelasticCompFS(G4ParticleHPInelasticCompFS&) = delete;
  G4ParticleHPInelasticCompFS& operator=(const G4ParticleHPInelasticCompFS &right) = delete;

 private:

  void two_body_reaction(G4ReactionProduct* proj, G4ReactionProduct* targ,
			 G4ReactionProduct* product, G4double exEnergy);

  G4bool use_nresp71_model(const G4ParticleDefinition*, const G4int itt,
			   const G4ReactionProduct& theTarget,
                           G4ReactionProduct& boosted);

  G4NRESP71M03 nresp71_model;

 protected:

  G4ParticleHPVector* theXsection[51];
  G4ParticleHPEnergyDistribution* theEnergyDistribution[51];
  G4ParticleHPAngular* theAngularDistribution[51];
  G4ParticleHPEnAngCorrelation* theEnergyAngData[51];
  G4ParticleHPPhotonDist* theFinalStatePhotons[51];

  std::vector<G4double> QI;
  std::vector<G4int> LR;

  G4ParticleHPDeExGammas theGammas;
  G4String gammaPath;
};

#endif
