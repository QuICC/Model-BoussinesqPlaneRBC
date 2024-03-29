/**
 * @file ModelBackend.hpp
 * @brief Model backend
 */

#ifndef QUICC_MODEL_BOUSSINESQ_PLANE_RBC_EXPLICIT_MODELBACKEND_HPP
#define QUICC_MODEL_BOUSSINESQ_PLANE_RBC_EXPLICIT_MODELBACKEND_HPP

// System includes
//
#include <string>
#include <vector>
#include <map>
#include <memory>

// Project includes
//
#include "Model/Boussinesq/Plane/RBC/IRBCBackend.hpp"

namespace QuICC {

namespace Model {

namespace Boussinesq {

namespace Plane {

namespace RBC {

namespace Explicit {

   /**
    * @brief Interface for model backend
    */
   class ModelBackend: public IRBCBackend
   {
      public:
         /**
          * @brief Constructor
          */
         ModelBackend();

         /**
          * @brief Destructor
          */
         virtual ~ModelBackend() = default;

         /**
          * @brief Get equation information
          *
          * @param info Equation information
          * @param fId  Field ID
          * @param res  Resolution object
          */
         virtual void equationInfo(EquationInfo& info, const SpectralFieldId& fId, const Resolution& res) const override;

         /**
          * @brief Get operator information
          *
          * @param info       Equation information
          * @param fId        Field ID
          * @param res        Resolution object
          * @param coupling   Equation/Field coupling information
          * @param bcs        Boundary conditions
          */
         virtual void operatorInfo(OperatorInfo& info, const SpectralFieldId& fId, const Resolution& res, const Equations::Tools::ICoupling& coupling, const BcMap& bcs) const override;

         /**
          * @brief Build model matrix
          *
          * @param rModelMatrix  Input/Output matrix to fill with operators
          * @param opId          Type of model matrix
          * @param imRange       Coupled fields
          * @param matIdx        Matrix index
          * @param bcType        Boundary condition scheme (Tau vs Galerkin)
          * @param res           Resolution object
          * @param eigs          Indexes of other dimensions
          * @param bcs           Boundary conditions
          * @param nds           Nondimensional parameters
          */
         virtual void modelMatrix(DecoupledZSparse& rModelMatrix, const std::size_t opId, const Equations::CouplingInformation::FieldId_range imRange, const int matIdx, const std::size_t bcType, const Resolution& res, const std::vector<MHDFloat>& eigs, const BcMap& bcs, const NonDimensional::NdMap& nds) const override;

         /**
          * @brief Build galerkin stencil
          *
          * @param mat     Input/Output matrix to fill with stencil
          * @param fId     Field ID
          * @param matIdx  Matrix index
          * @param res     Resolution object
          * @param eigs          Indexes of other dimensions
          * @param makeSquare Truncate stencil to obtain square matrix?
          * @param bcs           Boundary conditions
          * @param nds           Nondimensional parameters
          */
         virtual void galerkinStencil(SparseMatrix& mat, const SpectralFieldId& fId, const int matIdx, const Resolution& res, const std::vector<MHDFloat>& eigs, const bool makeSquare, const BcMap& bcs, const NonDimensional::NdMap& nds) const override;

         /**
          * @brief Build explicit block
          *
          * @param rModelMatrix  Input/Output matrix to fill with operators
          * @param fId           Equation field ID
          * @param opId          Type of explicit operator
          * @param fieldId       Coupled field ID
          * @param matIdx        Matrix index
          * @param res           Resolution object
          * @param eigs          Indexes of other dimensions
          * @param bcs           Boundary conditions
          * @param nds           Nondimensional parameters
          */
         virtual void explicitBlock(DecoupledZSparse& mat, const SpectralFieldId& fId, const std::size_t opId,  const SpectralFieldId fieldId, const int matIdx, const Resolution& res, const std::vector<MHDFloat>& eigs, const BcMap& bcs, const NonDimensional::NdMap& nds) const override;

      protected:
         /**
          * @brief Set field coupling in implicit model matrix
          *
          * @param fId  Field ID
          */
         SpectralFieldIds implicitFields(const SpectralFieldId& fId) const;

         /**
          * @brief Set field coupling in explicit nonlinear terms
          *
          * @param fId  Field ID
          */
         SpectralFieldIds explicitNonlinearFields(const SpectralFieldId& fId) const;

         /**
          * @brief Get operator information
          *
          * @param tN      Tau radial size
          * @param gN      Galerkin radial truncation
          * @param shift   Shift in each direction due to Galerkin basis
          * @param rhs     Numer of RHS
          * @param fId     ID of the field
          * @param res     Resolution object
          * @param eigs    Indexes of other dimensions
          * @param bcs     Boundary conditions
          */
         void blockSize(int& tN, int& gN, ArrayI& shift, int& rhs, const SpectralFieldId& fId, const Resolution& res, const std::vector<MHDFloat>& eigs, const BcMap& bcs) const;

         /**
          * @brief Build implicit matrix block
          *
          * @param decMat  Input/Output matrix to fill with operators
          * @param rowId   ID of field of equation
          * @param colId   ID of field 
          * @param matIdx        Matrix index
          * @param res           Resolution object
          * @param eigs          Indexes of other dimensions
          * @param nds           Nondimensional parameters
          * @param isSplitOperator Is second operator of split 4th order equation
          */
         void implicitBlock(DecoupledZSparse& decMat, const SpectralFieldId& rowId, const SpectralFieldId& colId, const int matIdx, const Resolution& res, const std::vector<MHDFloat>& eigs, const NonDimensional::NdMap& nds, const bool isSplitOperator) const;

         /**
          * @brief Build time matrix block
          *
          * @param decMat  Input/Output matrix to fill with operators
          * @param fieldId   ID of field 
          * @param matIdx        Matrix index
          * @param res           Resolution object
          * @param eigs          Indexes of other dimensions
          * @param nds           Nondimensional parameters
          */
         void timeBlock(DecoupledZSparse& decMat, const SpectralFieldId& fieldId, const int matIdx, const Resolution& res, const std::vector<MHDFloat>& eigs, const NonDimensional::NdMap& nds) const;

         /**
          * @brief Build inhomogeneous boundary value for split equation
          *
          * @param decMat  Input/Output matrix to fill with operators
          * @param fieldId   ID of field 
          * @param matIdx        Matrix index
          * @param res           Resolution object
          * @param eigs          Indexes of other dimensions
          * @param nds           Nondimensional parameters
          */
         void splitBoundaryValueBlock(DecoupledZSparse& decMat, const SpectralFieldId& fieldId, const int matIdx, const Resolution& res, const std::vector<MHDFloat>& eigs, const NonDimensional::NdMap& nds) const;

      private:
         /**
          * @brief Truncate quasi-inverse operators?
          */
         const bool mcTruncateQI;

   };

} // Explicit
} // RBC
} // Plane
} // Boussinesq
} // Model
} // QuICC

#endif // QUICC_MODEL_BOUSSINESQ_PLANE_RBC_EXPLICIT_MODELBACKEND_HPP
