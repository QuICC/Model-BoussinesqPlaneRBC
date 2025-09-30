/**
 * @file PhysicalModel.cpp
 * @brief Source of the Boussinesq Rayleigh-Benard convection in a plane layer
 * (toroidal/poloidal formulation) model
 */

// System includes
//

// Project includes
//
#include "Model/Boussinesq/Plane/RBC/IRBCModel.hpp"
#include "Model/Boussinesq/Plane/RBC/Momentum.hpp"
#include "Model/Boussinesq/Plane/RBC/Transport.hpp"
#include "Model/Boussinesq/Plane/RBC/gitHash.hpp"
#include "QuICC/Io/Variable/Cartesian1DScalarEnergyWriter.hpp"
#include "QuICC/Io/Variable/Cartesian1DTorPolEnergyWriter.hpp"
#include "QuICC/PhysicalNames/Temperature.hpp"
#include "QuICC/PhysicalNames/Velocity.hpp"

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
   spSim->addEquation<Equations::Boussinesq::Plane::RBC::Transport>(
      this->spBackend());

   // Add Navier-Stokes equation
   spSim->addEquation<Equations::Boussinesq::Plane::RBC::Momentum>(
      this->spBackend());
}

std::map<std::string, std::map<std::string, int>> IRBCModel::configTags() const
{
   std::map<std::string, int> onOff;
   onOff.emplace("enable", 1);

   std::map<std::string, int> offOn;
   onOff.emplace("enable", 0);

   std::map<std::string, std::map<std::string, int>> tags;
   // kinetic
   tags.emplace("kinetic_energy", onOff);
   // temperature
   tags.emplace("temperature_energy", onOff);
   tags.emplace("temperature_nusselt", offOn);

   return tags;
}

void IRBCModel::addAsciiOutputFiles(SharedSimulation spSim)
{
   // Create temperature energy writer
   this->enableAsciiFile<Io::Variable::Cartesian1DScalarEnergyWriter>(
      "temperature_energy", "temperature", PhysicalNames::Temperature::id(),
      spSim);

   // Create kinetic energy writer
   this->enableAsciiFile<Io::Variable::Cartesian1DTorPolEnergyWriter>(
      "kinetic_energy", "kinetic", PhysicalNames::Velocity::id(), spSim);

   //      // Create nusselt number writer
   //      this->enableAsciiFile<Io::Variable::Cartesian1DNusseltDZWriter>("temperature_nusselt",
   //      "temperature_", PhysicalNames::Temperature::id(), spSim);
}

} // namespace RBC
} // namespace Plane
} // namespace Boussinesq
} // namespace Model
} // namespace QuICC
