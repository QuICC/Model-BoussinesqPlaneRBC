/** 
 * @file PhysicalModel.hpp
 * @brief Implementation of the Boussinesq Rayleigh-Benard in a plane layer (toroidal/poloidal formulation)
 */

#ifndef QUICC_MODEL_BOUSSINESQ_PLANE_RBC_PHYSICALMODEL_HPP
#define QUICC_MODEL_BOUSSINESQ_PLANE_RBC_PHYSICALMODEL_HPP

// System includes
//
#include <string>

// Project includes
//
#include "QuICC/Model/Boussinesq/Plane/RBC/IRBCModel.hpp"
#include "QuICC/SpatialScheme/3D/TFF.hpp"

namespace QuICC {

namespace Model {

namespace Boussinesq {

namespace Plane {

namespace RBC {

namespace Explicit {

   /**
    * @brief Implementation of the Boussinesq Rayleigh-Benard in a plane layer (toroidal/poloidal formulation)
    */
   class PhysicalModel: public IRBCModel
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

         /// Python model script module name
         virtual std::string PYMODULE() override;

         /**
          * @brief Initialize specialized backend
          */
         void init() final;

      protected:

      private:
   };

} // Explicit
} // RBC
} // Plane
} // Boussinesq
} // Model
} // QuICC

#endif // QUICC_MODEL_BOUSSINESQ_PLANE_RBC_PHYSICALMODEL_HPP
