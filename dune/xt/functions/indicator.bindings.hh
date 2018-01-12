// This file is part of the dune-xt-functions project:
//   https://github.com/dune-community/dune-xt-functions
// Copyright 2009-2017 dune-xt-functions developers and contributors. All rights reserved.
// License: Dual licensed as BSD 2-Clause License (http://opensource.org/licenses/BSD-2-Clause)
//      or  GPL-2.0+ (http://opensource.org/licenses/gpl-license)
//          with "runtime exception" (http://www.dune-project.org/license.html)
// Authors:
//   Felix Schindler (2017)

#ifndef DUNE_XT_FUNCTIONS_INDICATOR_BINDINGS_HH
#define DUNE_XT_FUNCTIONS_INDICATOR_BINDINGS_HH
#if HAVE_DUNE_PYBINDXI

#include <dune/pybindxi/pybind11.h>

#include <dune/xt/common/string.hh>
#include <dune/xt/common/fvector.pbh>
#include <dune/xt/common/fmatrix.pbh>
#include <dune/xt/grid/gridprovider/provider.hh>
#include <dune/xt/grid/dd/subdomains/grid.hh>
#include <dune/xt/grid/grids.bindings.hh>
#include <dune/xt/grid/type_traits.hh>

#include "interfaces.hh"
#include "indicator.hh"

namespace Dune {
namespace XT {
namespace Functions {
namespace bindings {


template <class G>
class IndicatorFunction
{
  static_assert(Grid::is_grid<G>::value, "");
  using E = typename G::template Codim<0>::Entity;
  using D = typename G::ctype;
  static const constexpr size_t d = G::dimension;
  using R = double;
  static const constexpr size_t r = 1;
  static const constexpr size_t rC = 1;

public:
  using type = Functions::IndicatorFunction<E, D, d, R, r, rC>;
  using InterfaceType = LocalizableFunctionInterface<E, D, d, R, r, rC>;
  using bound_type = pybind11::class_<type, InterfaceType>;

  static bound_type bind(pybind11::module& m)
  {
    using namespace pybind11::literals;

    const auto class_name = std::string("indicator_function");
    const auto suffix = std::string("to_") + Common::to_string(r) + "x" + Common::to_string(rC);
    const auto grid_name = Grid::bindings::grid_name<G>::value();

    bound_type c(m,
                 Common::to_camel_case(class_name + "_" + grid_name + "_" + suffix).c_str(),
                 "dune-xt-functions: IndicatorFunction");

    m.def((std::string("make_") + class_name + "_" + suffix).c_str(),
          [](const Grid::GridProvider<G>& /*grid_provider*/,
             const std::vector<std::pair<std::pair<Common::FieldVector<D, d>, Common::FieldVector<D, d>>, R>>&
                 domain_and_value_pairs,
             const std::string& name) { return type(domain_and_value_pairs, name); },
          "grid_provider"_a,
          "domain_and_value_pairs"_a,
          "name"_a = "indicator_function");
#if HAVE_DUNE_FEM
    m.def((std::string("make_") + class_name + "_" + suffix).c_str(),
          [](const Grid::GridProvider<G, Grid::DD::SubdomainGrid<G>>& /*grid_provider*/,
             const std::vector<std::pair<std::pair<Common::FieldVector<D, d>, Common::FieldVector<D, d>>, R>>&
                 domain_and_value_pairs,
             const std::string& name) { return type(domain_and_value_pairs, name); },
          "grid_provider"_a,
          "domain_and_value_pairs"_a,
          "name"_a = "indicator_function");
#endif // HAVE_DUNE_FEM

    return c;
  } // ... bind(...)
}; // class IndicatorFunction


} // namespace bindings
} // namespace Functions
} // namespace XT
} // namespace Dune

#endif // HAVE_DUNE_PYBINDXI
#endif // DUNE_XT_FUNCTIONS_INDICATOR_BINDINGS_HH
