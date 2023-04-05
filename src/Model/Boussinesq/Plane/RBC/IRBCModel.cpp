/** 
 * @file PhysicalModel.cpp
 * @brief Source of the Boussinesq Rayleigh-Benard convection in a plane layer (toroidal/poloidal formulation) model
 */

// System includes
//

// Project includes
//
#include "QuICC/Model/Boussinesq/Plane/RBC/IRBCModel.hpp"
#include "QuICC/Model/Boussinesq/Plane/RBC/Transport.hpp"
#include "QuICC/Model/Boussinesq/Plane/RBC/Momentum.hpp"
#include "Model/Boussinesq/Plane/RBC/gitHash.hpp"
#include "QuICC/Enums/FieldIds.hpp"
#include "QuICC/PhysicalNames/Temperature.hpp"
#include "QuICC/PhysicalNames/Velocity.hpp"
#include "QuICC/NonDimensional/Prandtl.hpp"
#include "QuICC/NonDimensional/Rayleigh.hpp"
#include "QuICC/NonDimensional/Lower1d.hpp"
#include "QuICC/NonDimensional/Upper1d.hpp"
#include "QuICC/Io/Variable/StateFileReader.hpp"
#include "QuICC/Io/Variable/StateFileWriter.hpp"
#include "QuICC/Io/Variable/VisualizationFileWriter.hpp"
//#include "QuICC/Io/Variable/Cartesian1DScalarEnergyWriter.hpp"
//#include "QuICC/Io/Variable/Cartesian1DTorPolEnergyWriter.hpp"
//#include "QuICC/Io/Variable/Cartesian1DNusseltDZWriter.hpp"
#include "QuICC/Generator/States/RandomScalarState.hpp"
#include "QuICC/Generator/States/RandomVectorState.hpp"
//#include "QuICC/Generator/States/CartesianExactScalarState.hpp"
//#include "QuICC/Generator/States/CartesianExactVectorState.hpp"
#include "QuICC/Generator/Visualizers/ScalarFieldVisualizer.hpp"
#include "QuICC/Generator/Visualizers/ScalarFieldTrivialVisualizer.hpp"
#include "QuICC/Generator/Visualizers/VectorFieldVisualizer.hpp"
#include "QuICC/SpectralKernels/MakeRandom.hpp"

namespace QuICC {

namespace Model {

namespace Boussinesq {

namespace Plane {

namespace RBC {

   VectorFormulation::Id IRBCModel::SchemeFormulation()
   {
      return VectorFormulation::TORPOL;
   }

   std::string IRBCModel::version() const
   {
      return std::string(gitHash);
   }

   void IRBCModel::addEquations(SharedSimulation spSim)
   {
      // Add transport equation
      spSim->addEquation<Equations::Boussinesq::Plane::RBC::Transport>(this->spBackend());
                                                           
      // Add Navier-Stokes equation
      spSim->addEquation<Equations::Boussinesq::Plane::RBC::Momentum>(this->spBackend());
   }

   void IRBCModel::addStates(SharedStateGenerator spGen)
   {
//      // Generate "exact" solutions (trigonometric or monomial)
//      if(false)
//      {
//         // Shared pointer to equation
//         Equations::SharedCartesianExactScalarState spScalar;
//         Equations::SharedCartesianExactVectorState spVector;
//
//         // Add vector exact initial state generator
//         spVector = spGen->addVectorEquation<Equations::CartesianExactVectorState>();
//         spVector->setIdentity(PhysicalNames::VELOCITY);
//         spVector->setStateType(Equations::CartesianExactStateIds::TORPOLTFF);
//
//         // Add scalar exact initial state generator
//         spScalar = spGen->addScalarEquation<Equations::CartesianExactScalarState>();
//         spScalar->setIdentity(PhysicalNames::TEMPERATURE);
//         spScalar->setStateType(Equations::CartesianExactStateIds::PLANFORMSQUARES);
//         spScalar->setModeOptions(1e0, 10.0, 1e0, 10.0);
//
//      // Generate random spectrum
//      } else
//      {
//         // Shared pointer to random initial state equation
//         Equations::SharedRandomScalarState spScalar;
//         Equations::SharedRandomVectorState spVector;
//
//         // Add scalar random initial state generator 
//         spVector = spGen->addEquation<Equations::RandomVectorState>();
//         spVector->setIdentity(PhysicalNames::VELOCITY);
//         spVector->setSpectrum(FieldComponents::Spectral::TOR, -1e-6, 1e-6, 1e4, 1e4, 1e4);
//         spVector->setSpectrum(FieldComponents::Spectral::POL, -1e-6, 1e-6, 1e4, 1e4, 1e4);
//
//         // Add scalar random initial state generator
//         spScalar = spGen->addEquation<Equations::RandomScalarState>();
//         spScalar->setIdentity(PhysicalNames::TEMPERATURE);
//         spScalar->setSpectrum(-1e-6, 1e-6, 1e4, 1e4, 1e4);
//      }

      // Add output file
      auto spOut = std::make_shared<Io::Variable::StateFileWriter>(spGen->ss().tag(), spGen->ss().has(SpatialScheme::Feature::RegularSpectrum));
      spOut->expect(PhysicalNames::Velocity::id());
      spOut->expect(PhysicalNames::Temperature::id());
      spGen->addHdf5OutputFile(spOut);
   }

   void IRBCModel::addVisualizers(SharedVisualizationGenerator spVis)
   {
      // Shared pointer to basic field visualizer
      Equations::SharedScalarFieldVisualizer spScalar;
      Equations::SharedScalarFieldTrivialVisualizer spSTrivial;
      Equations::SharedVectorFieldVisualizer spVector;

      // Add temperature field visualization
      spScalar = spVis->addEquation<Equations::ScalarFieldVisualizer>();
      spScalar->setFields(true, false);
      spScalar->setIdentity(PhysicalNames::Temperature::id());

      // Add mean temperature field visualization
      spSTrivial = spVis->addEquation<Equations::ScalarFieldTrivialVisualizer>();
      spSTrivial->setFields(true, false);
      spSTrivial->setIdentity(PhysicalNames::MeanTemperature::id());

      // Add fluctuating temperature field visualization
      spSTrivial = spVis->addEquation<Equations::ScalarFieldTrivialVisualizer>();
      spSTrivial->setFields(true, false);
      spSTrivial->setIdentity(PhysicalNames::FluctTemperature::id());

      // Add velocity fields visualization
      spVector = spVis->addEquation<Equations::VectorFieldVisualizer>();
      spVector->setFields(true, false, true);
      spVector->setIdentity(PhysicalNames::Velocity::id());

      // Add output file
      auto spOut = std::make_shared<Io::Variable::VisualizationFileWriter>(spVis->ss().tag());
      spOut->expect(PhysicalNames::Temperature::id());
      spOut->expect(PhysicalNames::MeanTemperature::id());
      spOut->expect(PhysicalNames::FluctTemperature::id());
      spOut->expect(PhysicalNames::Velocity::id());
      spVis->addHdf5OutputFile(spOut);
   }

   std::map<std::string, std::map<std::string,int> > IRBCModel::configTags() const
   {
      std::map<std::string,int> onOff;
      onOff.emplace("enable", 1);

      std::map<std::string,std::map<std::string,int> > tags;
      // kinetic
      tags.emplace("kinetic_energy", onOff);
      // temperature
      tags.emplace("temperature_energy", onOff);
      tags.emplace("temperature_nusselt", onOff);

      return tags;
   }

   void IRBCModel::addAsciiOutputFiles(SharedSimulation spSim)
   {
//      // Create temperature energy writer
//      this->enableAsciiFile<Io::Variable::Cartesian1DScalarEnergyWriter>("temperature_energy", "temperature", PhysicalNames::Temperature::id(), spSim);
//
//      // Create kinetic energy writer
//      this->enableAsciiFile<Io::Variable::CartesianTorPolEnergyWriter>("kinetic_energy", "kinetic", PhysicalNames::Velocity::id(), spSim);
//
//      // Create nusselt number writer
//      this->enableAsciiFile<Io::Variable::Cartesian1DNusseltDZWriter>("temperature_nusselt", "temperature_", PhysicalNames::Temperature::id(), spSim);
   }

}
}
}
}
}
