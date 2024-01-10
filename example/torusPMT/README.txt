***GENERAL NOTES FOR DETECTOR OPTICAL COMPONENT DEFINITION IN GEANT4 (from user manuel)***

When a photon arrives at a medium boundary its behavior depends on the nature of the two materials that join at that
boundary. Medium boundaries may be formed between two dielectric materials or a dielectric and a metal.

In the case of an interface between a dielectric and a metal, the photon can be absorbed by the metal or reflected back
into the dielectric. If the photon is absorbed it can be detected according to the photoelectron efficiency of the metal.

The program defaults to the GLISUR model and polished surface finish when no specific model and surface finish is
specified by the user (G4OpticalSurface (const G4String &name, G4OpticalSurfaceModel model=glisur, G4OpticalSurfaceFinish finish=polished, G4SurfaceType type=dielectric_dielectric, G4double value=1.0)). In the case of a dielectric-metal interface, or when the GLISUR model is specified, the only surface finish options available are polished or ground. For dielectric-metal surfaces, the G4OpBoundaryProcess also defaults to unit reflectivity and zero detection efficiency. In cases where the user specifies the UNIFIED model (Fig. 5.1), but does not otherwise specify the model reflection probability constants, the default becomes Lambertian reflection.

No surface defined
In the special case where no surface has been defined, but the two volumes defining the surface have the refractive
index defined using the material property RINDEX, the surface is taken to be perfectly smooth, and both materials
are taken to be dielectric. The photon can undergo total internal reflection, refraction or reflection, depending on
the photon’s wavelength, angle of incidence, and the refractive indices on both sides of the boundary. Furthermore,
reflection and transmission probabilities are sensitive to the state of linear polarization.

Refractive index (RINDEX) is not assigned to surfaces. It is assigned to the materials of the two volumes that make up the surface. There is one exception for back-painted surfaces. 


NOTES for torusPMT implementation

1. We assign only REFLECTIVITY and EFFICIENCY properties of surfaces defined as dielectric_metal. There are two cases for a photon incident on an optical surface (dielectric_metal): reflection or absorption (namely, Reflectivity versus Absorption). If the photon is absorbed, it can be detected according to the quantum efficiency value of the metal of that surface. For photocathode surfaces, we do not assign REFLECTIVITY direclty to the surfaces, instead we assign REALRINDEX and IMAGINARYRINDEX. G4 kernel calculates REFLECTIVITY from these values internally. Note that complex refractive index is only defined for dielectric_metal surfaces, not defined for materials!

2. We choose GLISUR model for photocathode surfaces (dielectric_metal and polished) and UNIFIED model for reflector surfaces (dielectric_dielectric and PFP). All macro files defined to control optical surface properties are located under the macros/os directory and the corresponding data files are located under the data/surface_mpt directory.  

3. we give path for macro files from within some .cpp files. Also, we give path for data files from within the macro files. This should be fixed later!!! The paths are given relative to the CMAKE_BINARY_DIR where executable file is generated:
mMacrosDirectoryPath = "torusPMT/macros/mpt/"   ToroidalPMTMaterialFactory.cc 
mMacrosDirectoryPath = "torusPMT/macros/os/"    ToroidalPMTOSFactory.cc
mMacrosDirectoryPath = "torusPMT/macros/pmts/"  ToroidalPMTLVFactory.cc 
mMacrosDirectoryPath = "torusPMT/macros/pmts/"  ToroidalPMTLVBuilder.cc //default path

