/**
 * @file Momentum.hpp
 * @brief Implementation of the vector momentum equation for Rayleigh-Benard
 * convection in a plane layer (toroidal/poloidal formulation)
 */

#ifndef QUICC_EQUATIONS_BOUSSINESQ_PLANE_RBC_MOMENTUM_HPP
#define QUICC_EQUATIONS_BOUSSINESQ_PLANE_RBC_MOMENTUM_HPP

// System includes
//
#include <memory>

// Project includes
//
#include "QuICC/Equations/IVectorEquation.hpp"
#include "Types/Typedefs.hpp"

namespace QuICC {

namespace Equations {

namespace Boussinesq {

namespace Plane {

namespace RBC {

/**
 * @brief Implementation of the vector momentum equation for Rayleigh-Benard
 * convection in a plane layer (toroidal/poloidal formulation)
 */
class Momentum : public IVectorEquation
{
public:
   /**
    * @brief Simple constructor
    *
    * @param spEqParams    Shared equation parameters
    */
   Momentum(SharedEquationParameters spEqParams,
      SpatialScheme::SharedCISpatialScheme spScheme,
      std::shared_ptr<Model::IModelBackend> spBackend);

   /**
    * @brief Simple empty destructor
    */
   virtual ~Momentum() = default;

   /**
    * @brief Initialize nonlinear interaction kernel
    */
   virtual void initNLKernel(const bool force = false) override;

protected:
   /**
    * @brief Set variable requirements
    */
   virtual void setRequirements() override;

   /**
    * @brief Set the equation coupling information
    */
   virtual void setCoupling() override;

   /**
    * @brief Set the nonlinear integration components
    */
   virtual void setNLComponents() override;

private:
};

} // namespace RBC
} // namespace Plane
} // namespace Boussinesq
} // namespace Equations
} // namespace QuICC

#endif // QUICC_EQUATIONS_BOUSSINESQ_PLANE_RBC_MOMENTUM_HPP
