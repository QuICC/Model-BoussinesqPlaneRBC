/**
 * @file PhysicalModel.cpp
 * @brief Source of the Boussinesq Rayleigh-Benard convection in a plane layer
 * (toroidal/poloidal formulation) model
 */

// System includes
//

// Project includes
//
#include "Model/Boussinesq/Plane/RBC/IRBCVisualization.hpp"
#include "Model/Boussinesq/Plane/RBC/gitHash.hpp"
#include "QuICC/Io/Variable/VisualizationFileWriter.hpp"
#include "QuICC/PhysicalNames/Temperature.hpp"
#include "QuICC/PhysicalNames/Velocity.hpp"
#include "QuICC/Generator/Visualizers/ScalarFieldVisualizer.hpp"
#include "QuICC/Generator/Visualizers/VectorFieldVisualizer.hpp"

namespace QuICC {

namespace Model {

namespace Boussinesq {

namespace Plane {

namespace RBC {

VectorFormulation::Id IRBCVisualization::SchemeFormulation()
{
   return VectorFormulation::TORPOL;
}

std::string IRBCVisualization::version() const
{
   return std::string(gitHash);
}

void IRBCVisualization::addVisualizers(SharedVisualizationGenerator spVis)
{
   // Shared pointer to basic field visualizer
   Equations::SharedScalarFieldVisualizer spScalar;
   Equations::SharedVectorFieldVisualizer spVector;

   // Add temperature field visualization
   spScalar =
      spVis->addEquation<Equations::ScalarFieldVisualizer>(this->spBackend());
   spScalar->setFields(true, false);
   spScalar->setIdentity(PhysicalNames::Temperature::id());

   // Add velocity fields visualization
   spVector =
      spVis->addEquation<Equations::VectorFieldVisualizer>(this->spBackend());
   spVector->setFields(true, false, true);
   spVector->setIdentity(PhysicalNames::Velocity::id());

   // Add output file
   auto spOut = std::make_shared<Io::Variable::VisualizationFileWriter>(
      spVis->ss().tag());
   spOut->expect(PhysicalNames::Temperature::id());
   spOut->expect(PhysicalNames::Velocity::id());
   spVis->addHdf5OutputFile(spOut);
}

} // namespace RBC
} // namespace Plane
} // namespace Boussinesq
} // namespace Model
} // namespace QuICC
