// This file is part of the dune-xt-functions project:
//   https://github.com/dune-community/dune-xt-functions
// Copyright 2009-2018 dune-xt-functions developers and contributors. All rights reserved.
// License: Dual licensed as BSD 2-Clause License (http://opensource.org/licenses/BSD-2-Clause)
//      or  GPL-2.0+ (http://opensource.org/licenses/gpl-license)
//          with "runtime exception" (http://www.dune-project.org/license.html)
// Authors:
//   Felix Schindler (2017)
//   Rene Milk       (2018)
//   Tobias Leibner  (2017)

#ifndef DUNE_XT_FUNCTIONS_INTERFACES_LOCAL_FUNCTIONS_HH
#define DUNE_XT_FUNCTIONS_INTERFACES_LOCAL_FUNCTIONS_HH

#include <type_traits>
#include <vector>

#include <dune/common/fmatrix.hh>
#include <dune/common/fvector.hh>

#include <dune/geometry/quadraturerules.hh>
#include <dune/geometry/referenceelements.hh>

#include <dune/xt/common/parameter.hh>
#include <dune/xt/grid/entity.hh>

namespace Dune {
namespace XT {
namespace Functions {


template <class RangeFieldType, size_t dimRange, size_t dimRangeCols>
struct RangeTypeSelector
{
  typedef Dune::FieldMatrix<RangeFieldType, dimRange, dimRangeCols> type;
};

template <class RangeFieldType, size_t dimRange>
struct RangeTypeSelector<RangeFieldType, dimRange, 1>
{
  typedef Dune::FieldVector<RangeFieldType, dimRange> type;
};

template <size_t dimDomain, class RangeFieldType, size_t dimRange, size_t dimRangeCols>
struct JacobianRangeTypeSelector
{
  typedef Dune::FieldVector<Dune::FieldMatrix<RangeFieldType, dimRange, dimDomain>, dimRangeCols> type;
};

template <size_t dimDomain, class RangeFieldType, size_t dimRange>
struct JacobianRangeTypeSelector<dimDomain, RangeFieldType, dimRange, 1>
{
  typedef Dune::FieldMatrix<RangeFieldType, dimRange, dimDomain> type;
};

template <size_t dimDomain, class RangeFieldImp, size_t dimRange, size_t dimRangeCols>
struct RangeColumnHelper
{
  typedef typename RangeTypeSelector<RangeFieldImp, dimRange, dimRangeCols>::type RangeType;
  typedef typename RangeTypeSelector<RangeFieldImp, dimRange, 1>::type ColRangeType;
  typedef typename JacobianRangeTypeSelector<dimDomain, RangeFieldImp, dimRange, dimRangeCols>::type JacobianRangeType;
  typedef typename JacobianRangeTypeSelector<dimDomain, RangeFieldImp, dimRange, 1>::type ColJacobianRangeType;

  static void set_col(size_t col, RangeType& range, const ColRangeType& range_col)
  {
    for (size_t ii = 0; ii < dimRange; ++ii)
      range[ii][col] = range_col[ii];
  }

  static ColRangeType get_col(size_t col, const RangeType& range)
  {
    ColRangeType range_col;
    for (size_t ii = 0; ii < dimRange; ++ii)
      range_col[ii] = range[ii][col];
    return range_col;
  }

  static void set_col_jacobian(size_t col, JacobianRangeType& jacobian, const ColJacobianRangeType& jacobian_col)
  {
    jacobian[col] = jacobian_col;
  }

  static const ColJacobianRangeType& get_col_jacobian(size_t col, const JacobianRangeType& jacobian)
  {
    return jacobian[col];
  }
};

template <size_t dimDomain, class RangeFieldImp, size_t dimRange>
struct RangeColumnHelper<dimDomain, RangeFieldImp, dimRange, 1>
{
  typedef typename RangeTypeSelector<RangeFieldImp, dimRange, 1>::type RangeType;
  typedef RangeType ColRangeType;
  typedef typename JacobianRangeTypeSelector<dimDomain, RangeFieldImp, dimRange, 1>::type JacobianRangeType;
  typedef JacobianRangeType ColJacobianRangeType;

  static void set_col(size_t col, RangeType& range, const ColRangeType& range_col)
  {
    assert(col == 0);
    range = range_col;
  }

  static ColRangeType get_col(size_t col, const RangeType& range)
  {
    assert(col == 0);
    return range;
  }

  static void set_col_jacobian(size_t col, JacobianRangeType& jacobian, const ColJacobianRangeType& jacobian_col)
  {
    assert(col == 0);
    jacobian = jacobian_col;
  }

  static const ColJacobianRangeType& get_col_jacobian(size_t col, const JacobianRangeType& jacobian)
  {
    assert(col == 0);
    return jacobian;
  }
};


/**
 *  \brief Interface for a set of globalvalued functions, which can be evaluated locally on one Entity.
 */
template <class EntityImp,
          class DomainFieldImp,
          size_t domainDim,
          class RangeFieldImp,
          size_t rangeDim,
          size_t rangeDimCols = 1>
class LocalfunctionSetInterface : public Common::ParametricInterface
{
  static_assert(EntityImp::dimension == domainDim, "Dimensions do not match!");

public:
  typedef EntityImp EntityType;

  typedef DomainFieldImp DomainFieldType;
  static const constexpr size_t dimDomain = domainDim;
  typedef Dune::FieldVector<DomainFieldType, dimDomain> DomainType;

  typedef RangeFieldImp RangeFieldType;
  static const constexpr size_t dimRange = rangeDim;
  static const constexpr size_t dimRangeCols = rangeDimCols;
  typedef typename RangeTypeSelector<RangeFieldType, dimRange, dimRangeCols>::type RangeType;
  typedef typename JacobianRangeTypeSelector<dimDomain, RangeFieldType, dimRange, dimRangeCols>::type JacobianRangeType;

  typedef EntityType E;
  typedef DomainFieldType D;
  static const constexpr size_t d = dimDomain;
  typedef RangeFieldType R;
  static const constexpr size_t r = dimRange;
  static const constexpr size_t rC = dimRangeCols;

  LocalfunctionSetInterface(const EntityType& ent)
    : entity_(ent)
  {
  }

  virtual ~LocalfunctionSetInterface() = default;

  virtual const EntityType& entity() const
  {
    return entity_;
  }

  /**
   * \name ´´These methods have to be implemented.''
   * \{
   **/
  virtual size_t size() const = 0;

  virtual size_t order(const Common::Parameter& = {}) const = 0;

  virtual void
  evaluate(const DomainType& /*xx*/, std::vector<RangeType>& /*ret*/, const Common::Parameter& /*mu*/ = {}) const = 0;

  virtual void jacobian(const DomainType& /*xx*/,
                        std::vector<JacobianRangeType>& /*ret*/,
                        const Common::Parameter& /*mu*/ = {}) const = 0;
  /* \} */

  /**
   * \name ´´These methods are provided by the interface.''
   * \{
   **/
  std::vector<RangeType> evaluate(const DomainType& xx, const Common::Parameter& mu = {}) const
  {
    std::vector<RangeType> ret(size(), RangeType(0));
    evaluate(xx, ret, mu);
    return ret;
  }

  std::vector<JacobianRangeType> jacobian(const DomainType& xx, const Common::Parameter& mu = {}) const
  {
    std::vector<JacobianRangeType> ret(size(), JacobianRangeType(0));
    jacobian(xx, ret, mu);
    return ret;
  }
  /* \} */

protected:
  bool is_a_valid_point(const DomainType&
#ifndef DUNE_XT_FUNCTIONS_DISABLE_CHECKS
                            xx
#else
/*xx*/
#endif
                        ) const
  {
#ifndef DUNE_XT_FUNCTIONS_DISABLE_CHECKS
    const auto& reference_element = ReferenceElements<DomainFieldType, dimDomain>::general(entity().type());
    return reference_element.checkInside(xx);
#else // DUNE_XT_FUNCTIONS_DISABLE_CHECKS
    return true;
#endif
  }

  void assert_valid_point(const DomainType&
#ifndef DUNE_XT_FUNCTIONS_DISABLE_CHECKS
                              xx
#else
/*xx*/
#endif
                          ) const
  {
#ifndef DUNE_XT_FUNCTIONS_DISABLE_CHECKS
    const auto& reference_element = ReferenceElements<DomainFieldType, dimDomain>::general(entity().type());
    const auto tolerance = XT::Common::FloatCmp::DefaultEpsilon<DomainFieldType>::value();
    const bool inside =
        Impl::template checkInside<DomainFieldType, dimDomain>(reference_element.type().id(), dimDomain, xx, tolerance);

    if (!inside) {
      auto pp = entity().geometry().global(xx);
      std::stringstream ent_str;
      const int max_precision = std::numeric_limits<DomainFieldType>::digits10 + 1;
      ent_str << std::setprecision(max_precision);
      ent_str << "point " << pp << " not inside entity ";
      XT::Grid::print_entity(entity(), Common::Typename<EntityType>::value(), ent_str);
      ent_str << "\ncheckInside tolerance " << tolerance;
      DUNE_THROW(XT::Common::Exceptions::debug_assertion, ent_str.str());
    }
#endif
  }

  const EntityType& entity_;
}; // class LocalfunctionSetInterface


/**
 *  \brief  Interface for functions, which can be evaluated locally on one Entity.
 */
template <class EntityImp,
          class DomainFieldImp,
          size_t domainDim,
          class RangeFieldImp,
          size_t rangeDim,
          size_t rangeDimCols = 1>
class LocalfunctionInterface
    : public LocalfunctionSetInterface<EntityImp, DomainFieldImp, domainDim, RangeFieldImp, rangeDim, rangeDimCols>
{
  typedef LocalfunctionSetInterface<EntityImp, DomainFieldImp, domainDim, RangeFieldImp, rangeDim, rangeDimCols>
      BaseType;

public:
  typedef EntityImp EntityType;

  typedef typename BaseType::DomainFieldType DomainFieldType;
  static const constexpr size_t dimDomain = BaseType::dimDomain;
  typedef typename BaseType::DomainType DomainType;
  typedef typename BaseType::RangeType RangeType;
  typedef typename BaseType::JacobianRangeType JacobianRangeType;

  LocalfunctionInterface(const EntityType& ent)
    : BaseType(ent)
  {
  }

  virtual ~LocalfunctionInterface()
  {
  }

  /**
   * \name ´´These methods have to be implemented in addition to the ones required from the BaseType.''
   * \{
   **/
  virtual void evaluate(const DomainType& /*xx*/, RangeType& /*ret*/, const Common::Parameter& /*mu*/ = {}) const = 0;

  virtual void
  jacobian(const DomainType& /*xx*/, JacobianRangeType& /*ret*/, const Common::Parameter& /*mu*/ = {}) const = 0;
  /* \} */

  /**
   * \name ´´These methods are provided by the interface to please LocalfunctionSetInterface.''
   * \{
   **/
  virtual size_t size() const override final
  {
    return 1;
  }

  virtual void
  evaluate(const DomainType& xx, std::vector<RangeType>& ret, const Common::Parameter& mu = {}) const override final
  {
    assert(ret.size() >= 1);
    evaluate(xx, ret[0], mu);
  }

  virtual void jacobian(const DomainType& xx,
                        std::vector<JacobianRangeType>& ret,
                        const Common::Parameter& mu = {}) const override final
  {
    assert(ret.size() >= 1);
    jacobian(xx, ret[0], mu);
  }
  /* \} */

  /**
   * \name ´´These methods are provided by the interface.''
   * \{
   **/
  RangeType evaluate(const DomainType& xx, const Common::Parameter& mu = {}) const
  {
    RangeType ret(0);
    evaluate(xx, ret, mu);
    return ret;
  }

  JacobianRangeType jacobian(const DomainType& xx, const Common::Parameter& mu = {}) const
  {
    JacobianRangeType ret(0);
    jacobian(xx, ret, mu);
    return ret;
  }

  //! evaluate at N quadrature points into vector of size >= N
  void evaluate(const Dune::QuadratureRule<DomainFieldType, dimDomain>& quadrature,
                std::vector<RangeType>& ret,
                const Common::Parameter& mu = {})
  {
    assert(ret.size() >= quadrature.size());
    std::size_t i = 0;
    for (const auto& point : quadrature)
      evaluate(point.position(), ret[i++], mu);
  }

  //! jacobian at N quadrature points into vector of size >= N
  void jacobian(const Dune::QuadratureRule<DomainFieldType, dimDomain>& quadrature,
                std::vector<JacobianRangeType>& ret,
                const Common::Parameter& mu = {})
  {
    assert(ret.size() >= quadrature.size());
    std::size_t i = 0;
    for (const auto& point : quadrature)
      jacobian(point.position(), ret[i++], mu);
  }
  /* \} */
}; // class LocalfunctionInterface


} // namespace Functions
} // namespace XT
} // namespace Dune

#endif // DUNE_XT_FUNCTIONS_INTERFACES_LOCAL_FUNCTIONS_HH
