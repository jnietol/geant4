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
// G4UTubs
//
// Class description:
//
// Wrapper class for G4Tubs to make use of VecGeom Tube.

// 30.10.13 G.Cosmo, CERN
// --------------------------------------------------------------------
#ifndef G4UTUBS_HH
#define G4UTUBS_HH

#include "G4UAdapter.hh"

#if ( defined(G4GEOM_USE_USOLIDS) || defined(G4GEOM_USE_PARTIAL_USOLIDS) )

#include <VecGeom/volumes/UnplacedTube.h>

#include "G4Polyhedron.hh"

class G4UTubs : public G4UAdapter<vecgeom::GenericUnplacedTube>
{
  using Shape_t = vecgeom::GenericUnplacedTube;
  using Base_t = G4UAdapter<vecgeom::GenericUnplacedTube>;

  public:

    G4UTubs( const G4String& pName,
                   G4double pRMin,
                   G4double pRMax,
                   G4double pDz,
                   G4double pSPhi,
                   G4double pDPhi );
      // Constructs a tubs with the given name and dimensions

   ~G4UTubs() override;

    void ComputeDimensions(       G4VPVParameterisation* p,
                            const G4int n,
                            const G4VPhysicalVolume* pRep ) override;

    G4VSolid* Clone() const override;

    G4double GetInnerRadius   () const;
    G4double GetOuterRadius   () const;
    G4double GetZHalfLength   () const;
    G4double GetStartPhiAngle () const;
    G4double GetDeltaPhiAngle () const;
    G4double GetSinStartPhi   () const;
    G4double GetCosStartPhi   () const;
    G4double GetSinEndPhi     () const;
    G4double GetCosEndPhi     () const;

    void SetInnerRadius   (G4double newRMin);
    void SetOuterRadius   (G4double newRMax);
    void SetZHalfLength   (G4double newDz);
    void SetStartPhiAngle (G4double newSPhi, G4bool trig=true);
    void SetDeltaPhiAngle (G4double newDPhi);

    inline G4GeometryType GetEntityType() const override;

    void BoundingLimits(G4ThreeVector& pMin, G4ThreeVector& pMax) const override;

    G4bool CalculateExtent(const EAxis pAxis,
                           const G4VoxelLimits& pVoxelLimit,
                           const G4AffineTransform& pTransform,
                           G4double& pMin, G4double& pMax) const override;

    G4Polyhedron* CreatePolyhedron() const override;

    G4UTubs(const G4UTubs& rhs);
    G4UTubs& operator=(const G4UTubs& rhs);
      // Copy constructor and assignment operator.

};

// --------------------------------------------------------------------
// Inline methods
// --------------------------------------------------------------------

inline G4GeometryType G4UTubs::GetEntityType() const
{
  return "G4Tubs";
}

#endif  // G4GEOM_USE_USOLIDS

#endif
