/** 
 * @file PhysicalModel.cpp
 * @brief Source of the Boussinesq Rayleigh-Benard convection in a plane layer (toroidal/poloidal formulation) model
 */

// System includes
//

// Project includes
//
#include "QuICC/Model/Boussinesq/Plane/RBC/Explicit/PhysicalModel.hpp"
#include "QuICC/Model/Boussinesq/Plane/RBC/Explicit/ModelBackend.hpp"
#include "QuICC/Model/PyModelBackend.hpp"

namespace QuICC {

namespace Model {

namespace Boussinesq {

namespace Plane {

namespace RBC {

namespace Explicit {

   std::string PhysicalModel::PYMODULE()
   {
      return "boussinesq.plane.rbc.explcit.physical_model";
   }

   void PhysicalModel::init()
   {
#ifdef QUICC_MODEL_BOUSSINESQPLANERBC_EXPLICIT_BACKEND_CPP
      IPhysicalModel<Simulation,StateGenerator,VisualizationGenerator>::init();

      this->mpBackend = std::make_shared<ModelBackend>();
#else
      IPhysicalPyModel<Simulation,StateGenerator,VisualizationGenerator>::init();

      this->mpBackend = std::make_shared<PyModelBackend>(this->PYMODULE(), this->PYCLASS());
#endif
   }

} // Explicit
} // RBC
} // Plane
} // Boussinesq
} // Model
} // QuICC
