/**
 * @file IRBCVisualization.hpp
 * @brief Implementation of the Boussinesq Rayleigh-Benard in a plane layer
 * (toroidal/poloidal formulation)
 */

#ifndef QUICC_MODEL_BOUSSINESQ_PLANE_RBC_IRBCVISUALIZATION_HPP
#define QUICC_MODEL_BOUSSINESQ_PLANE_RBC_IRBCVISUALIZATION_HPP

// System includes
//
#include <string>

// Project includes
//
#include "QuICC/Generator/VisualizationGenerator.hpp"
#include "QuICC/Model/IVisualizationGeneratorBuilder.hpp"

namespace QuICC {

namespace Model {

namespace Boussinesq {

namespace Plane {

namespace RBC {

/**
 * @brief Implementation of the Boussinesq Rayleigh-Benard in a plane layer
 * (toroidal/poloidal formulation)
 */
class IRBCVisualization : public IVisualizationGeneratorBuilder<VisualizationGenerator>
{
public:
   /**
    * @brief Constructor
    */
   IRBCVisualization() = default;

   /**
    * @brief Destructor
    */
   virtual ~IRBCVisualization() = default;

   /// Formulation used for vector fields
   virtual VectorFormulation::Id SchemeFormulation() override;

   /**
    * @brief Version string
    */
   std::string version() const final;

   /**
    * @brief Add the visualization generation equations
    *
    * @param spGen   Shared visualization generator
    */
   virtual void addVisualizers(SharedVisualizationGenerator spVis) override;

protected:
private:
};

} // namespace RBC
} // namespace Plane
} // namespace Boussinesq
} // namespace Model
} // namespace QuICC

#endif // QUICC_MODEL_BOUSSINESQ_PLANE_RBC_IRBCVISUALIZATION_HPP
