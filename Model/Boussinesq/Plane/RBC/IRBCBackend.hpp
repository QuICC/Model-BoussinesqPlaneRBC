/**
 * @file IRBCBackend.hpp
 * @brief Base model backend for RBC model
 */

#ifndef QUICC_MODEL_BOUSSINESQ_PLANE_RBC_IRBCBACKEND_HPP
#define QUICC_MODEL_BOUSSINESQ_PLANE_RBC_IRBCBACKEND_HPP

// System includes
//
#include <string>
#include <vector>
#include <map>
#include <memory>

// Project includes
//
#include "QuICC/Model/IModelBackend.hpp"

namespace QuICC {

namespace Model {

namespace Boussinesq {

namespace Plane {

namespace RBC {

   /**
    * @brief Base model backed for RBC model
    */
   class IRBCBackend: public IModelBackend
   {
      public:
         /**
          * @brief Constructor
          */
         IRBCBackend();

         /**
          * @brief Destructor
          */
         virtual ~IRBCBackend() = default;

         /**
          * @brief Get vector of names for the physical fields
          */
         virtual std::vector<std::string> fieldNames() const override;

         /**
          * @brief Get vector of names for the nondimensional parameters
          */
         virtual std::vector<std::string> paramNames() const override;

         /**
          * @brief Get vector of bools about periodic box
          */
         virtual std::vector<bool> isPeriodicBox() const override;

         /**
          * @brief Get automatically computed parameters based on input parameters
          *
          * @param cfg  Input parameters
          */
         virtual std::map<std::string,MHDFloat> automaticParameters(const std::map<std::string,MHDFloat>& cfg) const override;

      protected:
         /**
          * @brief Number of boundary conditions
          *
          * @fId  Field ID
          */
         int nBc(const SpectralFieldId& fId) const;

         /**
          * @brief Get operator block information
          *
          * @param tN      Tau radial size
          * @param gN      Galerkin radial truncation
          * @param shift   Shift in each direction due to Galerkin basis
          * @param fId     ID of the field
          * @param res     Resolution object
          * @param l       Harmonic degree
          * @param bcs     Boundary conditions
          */
         void blockInfo(int& tN, int& gN, ArrayI& shift, int& rhs, const SpectralFieldId& fId, const Resolution& res, const MHDFloat k1, const MHDFloat k2, const BcMap& bcs) const;

         /**
          * @brief Apply tau line for boundary condition
          *
          * @param mat     Input/Output matrix to apply tau line to
          * @param rowId   ID of field of equation
          * @param colId   ID of field 
          * @param l       Harmonic degree
          * @param res     Resolution object
          * @param bcs     Boundary conditions
          * @param nds     Nondimensional parameters
          * @param isSplitOperator  Is second operator of split 4th order system?
          */
         void applyTau(SparseMatrix& mat, const SpectralFieldId& rowId, const SpectralFieldId& colId, const int k1, const int k2, const Resolution& res, const BcMap& bcs, const NonDimensional::NdMap& nds, const bool isSplitOperator) const;

         /**
          * @brief Boundary condition stencil
          *
          * @param mat        Input/Output matrix to store galerkin stencil
          * @param fID        Field ID
          * @param l          Harmonic degree
          * @param res        Resolution object
          * @param makeSquare Truncate operator to make square
          * @param bcs        Boundary conditions
          * @param nds        Nondimensional parameters
          */
         virtual void stencil(SparseMatrix& mat, const SpectralFieldId& fId, const int k1, const int k2, const Resolution& res, const bool makeSquare, const BcMap& bcs, const NonDimensional::NdMap& nds) const;

         /**
          * @brief Apply galerkin stencil for boundary condition
          *
          * @param mat     Input/Output matrix to apply stencil to
          * @param rowId   ID of field of equation
          * @param colId   ID of field 
          * @param l       Harmonic degree
          * @param res     Resolution object
          * @param bcs     Boundary conditions
          * @param nds     Nondimensional parameters
          */
         void applyGalerkinStencil(SparseMatrix& decMat, const SpectralFieldId& rowId, const SpectralFieldId& colId, const int k1, const int k2, const Resolution& res, const BcMap& bcs, const NonDimensional::NdMap& nds) const;

      private:
   };

} // RBC
} // Plane
} // Boussinesq
} // Model
} // QuICC

#endif // QUICC_MODEL_BOUSSINESQ_PLANE_RBC_IRBCBACKEND_HPP
