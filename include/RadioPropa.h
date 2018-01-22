/// CRPRopa public definitions
#ifndef CRPROPA_H
#define CRPROPA_H

#include "radiopropa/Candidate.h"
#include "radiopropa/Common.h"
#include "radiopropa/Cosmology.h"
#include "radiopropa/EmissionMap.h"
#include "radiopropa/Grid.h"
#include "radiopropa/GridTools.h"
#include "radiopropa/Logging.h"
#include "radiopropa/Module.h"
#include "radiopropa/ModuleList.h"
#include "radiopropa/ParticleID.h"
#include "radiopropa/ParticleMass.h"
#include "radiopropa/ParticleState.h"
#include "radiopropa/PhotonBackground.h"
#include "radiopropa/PhotonPropagation.h"
#include "radiopropa/Random.h"
#include "radiopropa/Referenced.h"
#include "radiopropa/Source.h"
#include "radiopropa/Units.h"
#include "radiopropa/Variant.h"
#include "radiopropa/Vector3.h"
#include "radiopropa/Version.h"

#include "radiopropa/module/Boundary.h"
#include "radiopropa/module/BreakCondition.h"
#include "radiopropa/module/DiffusionSDE.h"
#include "radiopropa/module/EMCascade.h"
#include "radiopropa/module/EMDoublePairProduction.h"
#include "radiopropa/module/EMInverseComptonScattering.h"
#include "radiopropa/module/EMPairProduction.h"
#include "radiopropa/module/EMTripletPairProduction.h"
#include "radiopropa/module/ElasticScattering.h"
#include "radiopropa/module/ElectronPairProduction.h"
#include "radiopropa/module/HDF5Output.h"
#include "radiopropa/module/NuclearDecay.h"
#include "radiopropa/module/Observer.h"
#include "radiopropa/module/OutputCRPropa2.h"
#include "radiopropa/module/OutputROOT.h"
#include "radiopropa/module/OutputShell.h"
#include "radiopropa/module/ParticleCollector.h"
#include "radiopropa/module/PhotoDisintegration.h"
#include "radiopropa/module/PhotoPionProduction.h"
#include "radiopropa/module/PhotonEleCa.h"
#include "radiopropa/module/PhotonOutput1D.h"
#include "radiopropa/module/PropagationCK.h"
#include "radiopropa/module/Redshift.h"
#include "radiopropa/module/SimplePropagation.h"
#include "radiopropa/module/SynchrotronRadiation.h"
#include "radiopropa/module/TextOutput.h"
#include "radiopropa/module/Tools.h"
#include "radiopropa/module/AdiabaticCooling.h"

#include "radiopropa/magneticField/AMRMagneticField.h"
#include "radiopropa/magneticField/JF12Field.h"
#include "radiopropa/magneticField/MagneticField.h"
#include "radiopropa/magneticField/MagneticFieldGrid.h"
#include "radiopropa/magneticField/PshirkovField.h"
#include "radiopropa/magneticField/QuimbyMagneticField.h"
#include "radiopropa/magneticField/ArchimedeanSpiralField.h"

#include "radiopropa/advectionField/AdvectionField.h"


#endif // CRPROPA_H
