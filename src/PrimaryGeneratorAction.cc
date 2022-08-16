
#include "PrimaryGeneratorAction.hh"

#include "G4LogicalVolumeStore.hh"
#include "G4LogicalVolume.hh"

//particular geometry class
#include "G4Box.hh"
#include "G4Tubs.hh"

//Run manager
#include "G4RunManager.hh"

//particle generator
#include "G4ParticleGun.hh"

//Tabel cu prop. particulelor
#include "G4ParticleTable.hh"

//Def. 
#include "G4ParticleDefinition.hh"

//Sistemul de unitati 
#include "G4SystemOfUnits.hh"
#include "Randomize.hh"

namespace B1
{

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

PrimaryGeneratorAction::PrimaryGeneratorAction()
{
  G4int n_particle = 1;
  fParticleGun  = new G4ParticleGun(n_particle);

  //Stabilim cinematica paritculeor - imprumutam profilul lor din tabel
  G4ParticleTable* particleTable = G4ParticleTable::GetParticleTable();
 
 //numele particulelor;
  G4String particleName;
  
  G4ParticleDefinition* particle
    = particleTable->FindParticle(particleName="gamma");
  
  //stabilim identitatea particulei/definitia aferenta
  fParticleGun->SetParticleDefinition(particle);
  
  //Directia impulsului 
  fParticleGun->SetParticleMomentumDirection(G4ThreeVector(0.,0.,1.));
  
  //Energia part.
  fParticleGun->SetParticleEnergy(6.*MeV);
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......


PrimaryGeneratorAction::~PrimaryGeneratorAction()
{
  delete fParticleGun;
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

void PrimaryGeneratorAction::GeneratePrimaries(G4Event* anEvent)
{
  //this function is called at the begining of each event
 

  // In order to avoid dependence of PrimaryGeneratorAction
  // on DetectorConstruction class we get Envelope volume
  // from G4LogicalVolumeStore.

  G4double envSizeXY = 0;
  G4double envSizeZ = 0;

  if (!fEnvelopeBox)
  {
    G4LogicalVolume* envLV
      = G4LogicalVolumeStore::GetInstance()->GetVolume("Envelope");
    if ( envLV ) fEnvelopeBox = dynamic_cast<G4Box*>(envLV->GetSolid());
  }

  if ( fEnvelopeBox ) {
    envSizeXY = fEnvelopeBox->GetXHalfLength()*2.;
    envSizeZ = fEnvelopeBox->GetZHalfLength()*2.;
  }
 
  else  {
    G4ExceptionDescription msg;
    msg << "Envelope volume of box shape not found.\n";
    msg << "Perhaps you have changed geometry.\n";
    msg << "The gun will be place at the center.";
    G4Exception("PrimaryGeneratorAction::GeneratePrimaries()",
     "MyCode0002",JustWarning,msg);
  }

  //gun particle position
  G4double size = 0.0;
  G4double x0 = size * envSizeXY * (G4UniformRand()-0.5);
  G4double y0 = size * envSizeXY * (G4UniformRand()-0.5);
  G4double z0 = -0.5 * envSizeZ;

  fParticleGun->SetParticlePosition(G4ThreeVector(x0,y0,z0));

  fParticleGun->GeneratePrimaryVertex(anEvent);
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

}


