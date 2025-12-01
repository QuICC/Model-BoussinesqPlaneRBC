/**
 * @file IRBCState.hpp
 * @brief Implementation of the Boussinesq Rayleigh-Benard in a plane layer
 * (toroidal/poloidal formulation)
 */

#ifndef QUICC_MODEL_BOUSSINESQ_PLANE_RBC_IRBCSTATE_HPP
#define QUICC_MODEL_BOUSSINESQ_PLANE_RBC_IRBCSTATE_HPP

// System includes
//
#include <string>

// Project includes
//
#include "QuICC/Generator/StateGenerator.hpp"
#include "QuICC/Model/IStateGeneratorBuilder.hpp"

namespace QuICC {

namespace Model {

namespace Boussinesq {

namespace Plane {

namespace RBC {

/**
 * @brief Implementation of the Boussinesq Rayleigh-Benard in a plane layer
 * (toroidal/poloidal formulation)
 */
class IRBCState : public IStateGeneratorBuilder<StateGenerator>
{
public:
   /**
    * @brief Constructor
    */
   IRBCState() = default;

   /**
    * @brief Destructor
    */
   virtual ~IRBCState() = default;

   /// Formulation used for vector fields
   virtual VectorFormulation::Id SchemeFormulation() override;

   /**
    * @brief Version string
    */
   std::string version() const final;

   /**
    * @brief Add the initial state generation equations
    *
    * @param spGen   Shared generator object
    */
   virtual void addStates(SharedStateGenerator spGen) override;

protected:
private:
};

} // namespace RBC
} // namespace Plane
} // namespace Boussinesq
} // namespace Model
} // namespace QuICC

#endif // QUICC_MODEL_BOUSSINESQ_PLANE_RBC_IRBCSTATE_HPP
