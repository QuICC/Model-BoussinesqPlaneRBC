/**
 * @file IRBCModel.hpp
 * @brief Implementation of the Boussinesq Rayleigh-Benard in a plane layer
 * (toroidal/poloidal formulation)
 */

#ifndef QUICC_MODEL_BOUSSINESQ_PLANE_RBC_IRBCMODEL_HPP
#define QUICC_MODEL_BOUSSINESQ_PLANE_RBC_IRBCMODEL_HPP

// System includes
//
#include <string>

// Project includes
//
#include "QuICC/Simulation/Simulation.hpp"
#include "QuICC/Model/IModelBuilder.hpp"

namespace QuICC {

namespace Model {

namespace Boussinesq {

namespace Plane {

namespace RBC {

/**
 * @brief Implementation of the Boussinesq Rayleigh-Benard in a plane layer
 * (toroidal/poloidal formulation)
 */
class IRBCModel : public IModelBuilder<Simulation>
{
public:
   /**
    * @brief Constructor
    */
   IRBCModel() = default;

   /**
    * @brief Destructor
    */
   virtual ~IRBCModel() = default;

   /// Formulation used for vector fields
   virtual VectorFormulation::Id SchemeFormulation() override;

   /**
    * @brief Version string
    */
   std::string version() const final;

   /**
    * @brief Add the required equations
    *
    * @param spSim   Shared simulation object
    */
   virtual void addEquations(SharedSimulation spSim) override;

   /**
    * @brief Add the required ASCII output files
    *
    * @param spSim   Shared simulation object
    */
   virtual void addAsciiOutputFiles(SharedSimulation spSim) override;

   /**
    * @brief XML configuration tags
    */
   virtual std::map<std::string, std::map<std::string, int>>
   configTags() const override;

protected:
private:
};

} // namespace RBC
} // namespace Plane
} // namespace Boussinesq
} // namespace Model
} // namespace QuICC

#endif // QUICC_MODEL_BOUSSINESQ_PLANE_RBC_IRBCMODEL_HPP
