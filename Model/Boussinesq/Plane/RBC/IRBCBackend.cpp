/**
 * @file IRBCBackend.cpp
 * @brief Source of the interface for model backend
 */

// System includes
//
#include <stdexcept>

// Project includes
//
#include "Model/Boussinesq/Plane/RBC/IRBCBackend.hpp"
#include "QuICC/Bc/Name/FixedFlux.hpp"
#include "QuICC/Bc/Name/FixedTemperature.hpp"
#include "QuICC/Bc/Name/NoSlip.hpp"
#include "QuICC/Bc/Name/StressFree.hpp"
#include "QuICC/Enums/FieldIds.hpp"
#include "QuICC/Equations/CouplingIndexType.hpp"
#include "QuICC/Model/ICppModelBackend.hpp"
#include "QuICC/ModelOperator/Boundary.hpp"
#include "QuICC/ModelOperator/ExplicitLinear.hpp"
#include "QuICC/ModelOperator/ExplicitNextstep.hpp"
#include "QuICC/ModelOperator/ExplicitNonlinear.hpp"
#include "QuICC/ModelOperator/ImplicitLinear.hpp"
#include "QuICC/ModelOperator/Stencil.hpp"
#include "QuICC/ModelOperator/Time.hpp"
#include "QuICC/ModelOperatorBoundary/FieldToRhs.hpp"
#include "QuICC/ModelOperatorBoundary/SolverHasBc.hpp"
#include "QuICC/ModelOperatorBoundary/SolverNoTau.hpp"
#include "QuICC/ModelOperatorBoundary/Stencil.hpp"
#include "QuICC/NonDimensional/Lower1d.hpp"
#include "QuICC/NonDimensional/Prandtl.hpp"
#include "QuICC/NonDimensional/Rayleigh.hpp"
#include "QuICC/NonDimensional/Upper1d.hpp"
#include "QuICC/PhysicalNames/Temperature.hpp"
#include "QuICC/PhysicalNames/Velocity.hpp"
#include "QuICC/Resolutions/Tools/IndexCounter.hpp"
#include "QuICC/SparseSM/Chebyshev/LinearMap/Boundary/D1.hpp"
#include "QuICC/SparseSM/Chebyshev/LinearMap/Boundary/D2.hpp"
#include "QuICC/SparseSM/Chebyshev/LinearMap/Boundary/ICondition.hpp"
#include "QuICC/SparseSM/Chebyshev/LinearMap/Boundary/Operator.hpp"
#include "QuICC/SparseSM/Chebyshev/LinearMap/Boundary/Value.hpp"
#include "QuICC/SparseSM/Chebyshev/LinearMap/Id.hpp"
#include "QuICC/SparseSM/Chebyshev/LinearMap/Stencil/D1.hpp"
#include "QuICC/SparseSM/Chebyshev/LinearMap/Stencil/Value.hpp"
#include "QuICC/SparseSM/Chebyshev/LinearMap/Stencil/ValueD1.hpp"
#include "QuICC/SparseSM/Chebyshev/LinearMap/Stencil/ValueD2.hpp"

namespace QuICC {

namespace Model {

namespace Boussinesq {

namespace Plane {

namespace RBC {

std::vector<std::string> IRBCBackend::fieldNames() const
{
   std::vector<std::string> names = {PhysicalNames::Velocity().tag(),
      PhysicalNames::Temperature().tag()};

   return names;
}

std::vector<std::string> IRBCBackend::paramNames() const
{
   std::vector<std::string> names = {
      NonDimensional::Prandtl().tag(),
      NonDimensional::Rayleigh().tag(),
   };

   return names;
}

std::vector<bool> IRBCBackend::isPeriodicBox() const
{
   std::vector<bool> periodic = {false, true, true};

   return periodic;
}

std::map<std::string, MHDFloat> IRBCBackend::automaticParameters(
   const std::map<std::string, MHDFloat>& cfg) const
{
   std::map<std::string, MHDFloat> params;

   params.emplace(NonDimensional::Lower1d().tag(), 0);
   params.emplace(NonDimensional::Upper1d().tag(), 1.0);

   return params;
}

int IRBCBackend::nBc(const SpectralFieldId& fId) const
{
   int nBc = 0;

   if (fId == std::make_pair(PhysicalNames::Velocity::id(),
                 FieldComponents::Spectral::TOR) ||
       fId == std::make_pair(PhysicalNames::Temperature::id(),
                 FieldComponents::Spectral::SCALAR))
   {
      nBc = 2;
   }
   else if (fId == std::make_pair(PhysicalNames::Velocity::id(),
                      FieldComponents::Spectral::POL))
   {
      nBc = 4;
   }
   else
   {
      nBc = 0;
   }

   return nBc;
}

void IRBCBackend::applyTau(SparseMatrix& mat, const SpectralFieldId& rowId,
   const SpectralFieldId& colId, const int k1,
   std::shared_ptr<details::BlockOptions> opts, const Resolution& res,
   const BcMap& bcs, const NonDimensional::NdMap& nds,
   const bool isSplitOperator) const
{
   auto nN = res.counter().dimensions(Dimensions::Space::SPECTRAL, k1)(0);

   auto zi = nds.find(NonDimensional::Lower1d::id())->second->value();
   auto zo = nds.find(NonDimensional::Upper1d::id())->second->value();

   auto bcId = bcs.find(rowId.first)->second;

   namespace Boundary = SparseSM::Chebyshev::LinearMap::Boundary;
   typedef Boundary::ICondition::Position Position;

   Boundary::Operator bcOp(nN, nN, zi, zo);

   if (rowId == std::make_pair(PhysicalNames::Velocity::id(),
                   FieldComponents::Spectral::TOR) &&
       rowId == colId)
   {
      if (bcId == Bc::Name::NoSlip::id())
      {
         bcOp.addRow<Boundary::Value>(Position::TOP);
         bcOp.addRow<Boundary::Value>(Position::BOTTOM);
      }
      else if (bcId == Bc::Name::StressFree::id())
      {
         bcOp.addRow<Boundary::D1>(Position::TOP);
         bcOp.addRow<Boundary::D1>(Position::BOTTOM);
      }
      else
      {
         throw std::logic_error("Boundary conditions for Velocity Toroidal "
                                "component not implemented");
      }
   }
   else if (rowId == std::make_pair(PhysicalNames::Velocity::id(),
                        FieldComponents::Spectral::POL) &&
            rowId == colId)
   {
      if (this->useSplitEquation())
      {
         if (isSplitOperator)
         {
            bcOp.addRow<Boundary::Value>(Position::TOP);
            bcOp.addRow<Boundary::Value>(Position::BOTTOM);
         }
         else
         {
            if (bcId == Bc::Name::NoSlip::id())
            {
               bcOp.addRow<Boundary::D1>(Position::TOP);
               bcOp.addRow<Boundary::D1>(Position::BOTTOM);
            }
            else if (bcId == Bc::Name::StressFree::id())
            {
               bcOp.addRow<Boundary::D2>(Position::TOP);
               bcOp.addRow<Boundary::D2>(Position::BOTTOM);
            }
            else
            {
               throw std::logic_error("Boundary conditions for Velocity "
                                      "Poloidal component not implemented");
            }
         }
      }
      else
      {
         auto& o =
            *std::dynamic_pointer_cast<implDetails::BlockOptionsImpl>(opts);

         if (bcId == Bc::Name::NoSlip::id())
         {
            bcOp.addRow<Boundary::Value>(Position::TOP);
            bcOp.addRow<Boundary::Value>(Position::BOTTOM);
            if (!(o.k1 == 0 && o.k2 == 0))
            {
               bcOp.addRow<Boundary::D1>(Position::TOP);
               bcOp.addRow<Boundary::D1>(Position::BOTTOM);
            }
         }
         else if (bcId == Bc::Name::StressFree::id())
         {
            if (o.k1 == 0 && o.k2 == 0)
            {
               bcOp.addRow<Boundary::D1>(Position::TOP);
               bcOp.addRow<Boundary::D1>(Position::BOTTOM);
            }
            else
            {
               bcOp.addRow<Boundary::Value>(Position::TOP);
               bcOp.addRow<Boundary::Value>(Position::BOTTOM);
               bcOp.addRow<Boundary::D2>(Position::TOP);
               bcOp.addRow<Boundary::D2>(Position::BOTTOM);
            }
         }
         else
         {
            throw std::logic_error("Boundary conditions for Velocity Poloidal "
                                   "component not implemented");
         }
      }
   }
   else if (rowId == std::make_pair(PhysicalNames::Temperature::id(),
                        FieldComponents::Spectral::SCALAR) &&
            rowId == colId)
   {
      if (bcId == Bc::Name::FixedTemperature::id())
      {
         bcOp.addRow<Boundary::Value>(Position::TOP);
         bcOp.addRow<Boundary::Value>(Position::BOTTOM);
      }
      else if (bcId == Bc::Name::FixedFlux::id())
      {
         bcOp.addRow<Boundary::D1>(Position::TOP);
         bcOp.addRow<Boundary::D1>(Position::BOTTOM);
      }
      else
      {
         throw std::logic_error(
            "Boundary conditions for Temperature not implemented (" +
            std::to_string(bcId) + ")");
      }
   }

   mat += bcOp.mat();
}

void IRBCBackend::stencil(SparseMatrix& mat, const SpectralFieldId& fieldId,
   const int k1, const Resolution& res, const bool makeSquare, const BcMap& bcs,
   const NonDimensional::NdMap& nds) const
{
   auto nN = res.counter().dimensions(Dimensions::Space::SPECTRAL, k1)(0);

   auto zi = nds.find(NonDimensional::Lower1d::id())->second->value();
   auto zo = nds.find(NonDimensional::Upper1d::id())->second->value();

   auto bcId = bcs.find(fieldId.first)->second;

   namespace Stencil = SparseSM::Chebyshev::LinearMap::Stencil;
   int s = this->nBc(fieldId);
   if (fieldId == std::make_pair(PhysicalNames::Velocity::id(),
                     FieldComponents::Spectral::TOR))
   {
      if (bcId == Bc::Name::NoSlip::id())
      {
         Stencil::Value bc(nN, nN - s, zi, zo);
         mat = bc.mat();
      }
      else if (bcId == Bc::Name::StressFree::id())
      {
         Stencil::D1 bc(nN, nN - s, zi, zo);
         mat = bc.mat();
      }
      else
      {
         throw std::logic_error("Galerkin boundary conditions for Velocity "
                                "Toroidal component not implemented");
      }
   }
   else if (fieldId == std::make_pair(PhysicalNames::Velocity::id(),
                          FieldComponents::Spectral::POL))
   {
      if (bcId == Bc::Name::NoSlip::id())
      {
         Stencil::ValueD1 bc(nN, nN - s, zi, zo);
         mat = bc.mat();
      }
      else if (bcId == Bc::Name::StressFree::id())
      {
         Stencil::ValueD2 bc(nN, nN - s, zi, zo);
         mat = bc.mat();
      }
      else
      {
         throw std::logic_error("Galerin boundary conditions for Velocity "
                                "Poloidal component not implemented");
      }
   }
   else if (fieldId == std::make_pair(PhysicalNames::Temperature::id(),
                          FieldComponents::Spectral::SCALAR))
   {
      if (bcId == Bc::Name::FixedTemperature::id())
      {
         Stencil::Value bc(nN, nN - s, zi, zo);
         mat = bc.mat();
      }
      else if (bcId == Bc::Name::FixedFlux::id())
      {
         Stencil::D1 bc(nN, nN - s, zi, zo);
         mat = bc.mat();
      }
      else
      {
         throw std::logic_error(
            "Galerkin boundary conditions for Temperature not implemented");
      }
   }

   if (makeSquare)
   {
      SparseSM::Chebyshev::LinearMap::Id qId(nN - s, nN, zi, zo);
      mat = qId.mat() * mat;
   }
}

void IRBCBackend::applyGalerkinStencil(SparseMatrix& mat,
   const SpectralFieldId& rowId, const SpectralFieldId& colId, const int k1r,
   const int k1c, std::shared_ptr<details::BlockOptions> opts,
   const Resolution& res, const BcMap& bcs,
   const NonDimensional::NdMap& nds) const
{
   auto nNr = res.counter().dimensions(Dimensions::Space::SPECTRAL, k1r)(0);

   auto zi = nds.find(NonDimensional::Lower1d::id())->second->value();
   auto zo = nds.find(NonDimensional::Upper1d::id())->second->value();

   auto S = mat;
   this->stencil(S, colId, k1c, res, false, bcs, nds);

   auto s = this->nBc(rowId);
   SparseSM::Chebyshev::LinearMap::Id qId(nNr - s, nNr, zi, zo, 0, s);
   mat = qId.mat() * (mat * S);
}


} // namespace RBC
} // namespace Plane
} // namespace Boussinesq
} // namespace Model
} // namespace QuICC
