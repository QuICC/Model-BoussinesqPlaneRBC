/**
 * @file PhysicalModel.hpp
 * @brief Implementation of the Boussinesq Rayleigh-Benard in a plane layer
 * (toroidal/poloidal formulation)
 */

#ifndef QUICC_MODEL_BOUSSINESQ_PLANE_RBC_PHYSICALMODEL_HPP
#define QUICC_MODEL_BOUSSINESQ_PLANE_RBC_PHYSICALMODEL_HPP

// System includes
//
#include <string>

// Project includes
//
#include "QuICC/SpatialScheme/3D/TFF.hpp"
#include "QuICC/Model/PyModelBackend.hpp"
#include "Model/Boussinesq/Plane/RBC/Explicit/ModelBackend.hpp"

namespace QuICC {

namespace Model {

namespace Boussinesq {

namespace Plane {

namespace RBC {

namespace Explicit {

/**
 * @brief Implementation of the Boussinesq Rayleigh-Benard in a plane layer
 * (toroidal/poloidal formulation)
 */
template <typename TBuilder> class PhysicalModel : public TBuilder
{
public:
   /// Typedef for the spatial scheme used
   typedef SpatialScheme::TFF SchemeType;

   /**
    * @brief Constructor
    */
   PhysicalModel() = default;

   /**
    * @brief Destructor
    */
   virtual ~PhysicalModel() = default;

   /**
    * @brief Initialize specialized backend
    */
   void init() final;

protected:
private:
};

template <typename TBuilder> void PhysicalModel<TBuilder>::init()
{
   TBuilder::init();
#ifdef QUICC_MODEL_BOUSSINESQPLANERBC_EXPLICIT_BACKEND_CPP

   this->mpBackend = std::make_shared<ModelBackend>();
#else
   std::string pyModule = "boussinesq.plane.rbc.explicit.physical_model";
   std::string pyClass = "PhysicalModel";

   this->mpBackend =
      std::make_shared<PyModelBackend>(pyModule, pyClass);
#endif
}

} // namespace Explicit
} // namespace RBC
} // namespace Plane
} // namespace Boussinesq
} // namespace Model
} // namespace QuICC

#endif // QUICC_MODEL_BOUSSINESQ_PLANE_RBC_PHYSICALMODEL_HPP
