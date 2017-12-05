// This file is part of the dune-xt-functions project:
//   https://github.com/dune-community/dune-xt-functions
// Copyright 2009-2017 dune-xt-functions developers and contributors. All rights reserved.
// License: Dual licensed as BSD 2-Clause License (http://opensource.org/licenses/BSD-2-Clause)
//      or  GPL-2.0+ (http://opensource.org/licenses/gpl-license)
//          with "runtime exception" (http://www.dune-project.org/license.html)
// Authors:
//   Felix Schindler (2016 - 2017)

#include "config.h"

#if HAVE_DUNE_PYBINDXI

#include <string>
#include <vector>

#include <dune/common/parallel/mpihelper.hh>

#if HAVE_DUNE_FEM
#include <dune/fem/misc/mpimanager.hh>
#endif

#include <dune/pybindxi/pybind11.h>
#include <dune/pybindxi/stl.h>

#include <dune/xt/common/bindings.hh>
#include <dune/xt/grid/grids.bindings.hh>

#include "interfaces.pbh"
#include "constant.pbh"
#include "checkerboard.pbh"
#include "ESV2007.bindings.hh"
#include "indicator.hh"
#include "expression.pbh"
#include "spe10.pbh"


template <class G>
void addbind_for_Grid(pybind11::module& m)
{
  const auto grid_id = Dune::XT::Grid::bindings::grid_name<G>::value();

  auto i_1_1 = Dune::XT::Functions::bind_LocalizableFunctionInterface<G, 1, 1>(m, grid_id);
  auto i_2_1 = Dune::XT::Functions::bind_LocalizableFunctionInterface<G, 2, 1>(m, grid_id);
  auto i_3_1 = Dune::XT::Functions::bind_LocalizableFunctionInterface<G, 3, 1>(m, grid_id);
  auto i_4_1 = Dune::XT::Functions::bind_LocalizableFunctionInterface<G, 4, 1>(m, grid_id);

  auto i_d_d = Dune::XT::Functions::bind_LocalizableFunctionInterface<G, G::dimension, G::dimension>(m, grid_id);

  Dune::XT::Functions::bind_combined_LocalizableFunction<G,
                                                         G::dimension,
                                                         Dune::XT::Functions::internal::Combination::difference,
                                                         1,
                                                         1,
                                                         1,
                                                         1>(m, grid_id);
  Dune::XT::Functions::
      addbind_LocalizableFunctionInterface_combined_op<G,
                                                       G::dimension,
                                                       Dune::XT::Functions::internal::Combination::difference,
                                                       1,
                                                       1,
                                                       1,
                                                       1>(i_1_1);
  Dune::XT::Functions::bind_combined_LocalizableFunction<G,
                                                         G::dimension,
                                                         Dune::XT::Functions::internal::Combination::difference,
                                                         2,
                                                         1,
                                                         2,
                                                         1>(m, grid_id);
  Dune::XT::Functions::
      addbind_LocalizableFunctionInterface_combined_op<G,
                                                       G::dimension,
                                                       Dune::XT::Functions::internal::Combination::difference,
                                                       2,
                                                       1,
                                                       2,
                                                       1>(i_2_1);
  Dune::XT::Functions::bind_combined_LocalizableFunction<G,
                                                         G::dimension,
                                                         Dune::XT::Functions::internal::Combination::difference,
                                                         3,
                                                         1,
                                                         3,
                                                         1>(m, grid_id);
  Dune::XT::Functions::
      addbind_LocalizableFunctionInterface_combined_op<G,
                                                       G::dimension,
                                                       Dune::XT::Functions::internal::Combination::difference,
                                                       3,
                                                       1,
                                                       3,
                                                       1>(i_3_1);
  Dune::XT::Functions::bind_combined_LocalizableFunction<G,
                                                         G::dimension,
                                                         Dune::XT::Functions::internal::Combination::difference,
                                                         4,
                                                         1,
                                                         4,
                                                         1>(m, grid_id);
  Dune::XT::Functions::
      addbind_LocalizableFunctionInterface_combined_op<G,
                                                       G::dimension,
                                                       Dune::XT::Functions::internal::Combination::difference,
                                                       4,
                                                       1,
                                                       4,
                                                       1>(i_4_1);
  Dune::XT::Functions::bind_combined_LocalizableFunction<G,
                                                         G::dimension,
                                                         Dune::XT::Functions::internal::Combination::difference,
                                                         2,
                                                         2,
                                                         2,
                                                         2>(m, grid_id);
  Dune::XT::Functions::
      addbind_LocalizableFunctionInterface_combined_op<G,
                                                       G::dimension,
                                                       Dune::XT::Functions::internal::Combination::difference,
                                                       G::dimension,
                                                       G::dimension,
                                                       G::dimension,
                                                       G::dimension>(i_d_d);


  Dune::XT::Functions::
      bind_combined_LocalizableFunction<G, G::dimension, Dune::XT::Functions::internal::Combination::sum, 1, 1, 1, 1>(
          m, grid_id);
  Dune::XT::Functions::addbind_LocalizableFunctionInterface_combined_op<G,
                                                                        G::dimension,
                                                                        Dune::XT::Functions::internal::Combination::sum,
                                                                        1,
                                                                        1,
                                                                        1,
                                                                        1>(i_1_1);
  Dune::XT::Functions::
      bind_combined_LocalizableFunction<G, G::dimension, Dune::XT::Functions::internal::Combination::sum, 2, 1, 2, 1>(
          m, grid_id);
  Dune::XT::Functions::addbind_LocalizableFunctionInterface_combined_op<G,
                                                                        G::dimension,
                                                                        Dune::XT::Functions::internal::Combination::sum,
                                                                        2,
                                                                        1,
                                                                        2,
                                                                        1>(i_2_1);
  Dune::XT::Functions::
      bind_combined_LocalizableFunction<G, G::dimension, Dune::XT::Functions::internal::Combination::sum, 3, 1, 3, 1>(
          m, grid_id);
  Dune::XT::Functions::addbind_LocalizableFunctionInterface_combined_op<G,
                                                                        G::dimension,
                                                                        Dune::XT::Functions::internal::Combination::sum,
                                                                        3,
                                                                        1,
                                                                        3,
                                                                        1>(i_3_1);
  Dune::XT::Functions::
      bind_combined_LocalizableFunction<G, G::dimension, Dune::XT::Functions::internal::Combination::sum, 4, 1, 4, 1>(
          m, grid_id);
  Dune::XT::Functions::addbind_LocalizableFunctionInterface_combined_op<G,
                                                                        G::dimension,
                                                                        Dune::XT::Functions::internal::Combination::sum,
                                                                        4,
                                                                        1,
                                                                        4,
                                                                        1>(i_4_1);
  Dune::XT::Functions::
      bind_combined_LocalizableFunction<G, G::dimension, Dune::XT::Functions::internal::Combination::sum, 2, 2, 2, 2>(
          m, grid_id);
  Dune::XT::Functions::addbind_LocalizableFunctionInterface_combined_op<G,
                                                                        G::dimension,
                                                                        Dune::XT::Functions::internal::Combination::sum,
                                                                        G::dimension,
                                                                        G::dimension,
                                                                        G::dimension,
                                                                        G::dimension>(i_d_d);

  Dune::XT::Functions::bind_combined_LocalizableFunction<G,
                                                         G::dimension,
                                                         Dune::XT::Functions::internal::Combination::product,
                                                         1,
                                                         1,
                                                         1,
                                                         1>(m, grid_id);
  Dune::XT::Functions::
      addbind_LocalizableFunctionInterface_combined_op<G,
                                                       G::dimension,
                                                       Dune::XT::Functions::internal::Combination::product,
                                                       1,
                                                       1,
                                                       1,
                                                       1>(i_1_1);
  Dune::XT::Functions::bind_combined_LocalizableFunction<G,
                                                         G::dimension,
                                                         Dune::XT::Functions::internal::Combination::product,
                                                         1,
                                                         1,
                                                         2,
                                                         1>(m, grid_id);
  Dune::XT::Functions::
      addbind_LocalizableFunctionInterface_combined_op<G,
                                                       G::dimension,
                                                       Dune::XT::Functions::internal::Combination::product,
                                                       1,
                                                       1,
                                                       2,
                                                       1>(i_1_1);
  Dune::XT::Functions::bind_combined_LocalizableFunction<G,
                                                         G::dimension,
                                                         Dune::XT::Functions::internal::Combination::product,
                                                         1,
                                                         1,
                                                         3,
                                                         1>(m, grid_id);
  Dune::XT::Functions::
      addbind_LocalizableFunctionInterface_combined_op<G,
                                                       G::dimension,
                                                       Dune::XT::Functions::internal::Combination::product,
                                                       1,
                                                       1,
                                                       3,
                                                       1>(i_1_1);
  Dune::XT::Functions::bind_combined_LocalizableFunction<G,
                                                         G::dimension,
                                                         Dune::XT::Functions::internal::Combination::product,
                                                         1,
                                                         1,
                                                         4,
                                                         1>(m, grid_id);
  Dune::XT::Functions::
      addbind_LocalizableFunctionInterface_combined_op<G,
                                                       G::dimension,
                                                       Dune::XT::Functions::internal::Combination::product,
                                                       1,
                                                       1,
                                                       4,
                                                       1>(i_1_1);
  Dune::XT::Functions::bind_combined_LocalizableFunction<G,
                                                         G::dimension,
                                                         Dune::XT::Functions::internal::Combination::product,
                                                         1,
                                                         1,
                                                         2,
                                                         2>(m, grid_id);
  Dune::XT::Functions::
      addbind_LocalizableFunctionInterface_combined_op<G,
                                                       G::dimension,
                                                       Dune::XT::Functions::internal::Combination::product,
                                                       1,
                                                       1,
                                                       2,
                                                       2>(i_1_1);

  Dune::XT::Functions::bind_ConstantFunction<G, G::dimension, 1, 1>(m, grid_id);
  Dune::XT::Functions::bind_ConstantFunction<G, G::dimension, 2, 1>(m, grid_id);
  Dune::XT::Functions::bind_ConstantFunction<G, G::dimension, 3, 1>(m, grid_id);
  Dune::XT::Functions::bind_ConstantFunction<G, G::dimension, 4, 1>(m, grid_id);
  Dune::XT::Functions::bind_ConstantFunction<G, G::dimension, 2, 2>(m, grid_id);

  Dune::XT::Functions::bind_CheckerboardFunction<G, G::dimension, 1, 1>(m, grid_id);
  //  Dune::XT::Functions::bind_CheckerboardFunction<G, G::dimension, 2, 1>(m, grid_id);
  //  Dune::XT::Functions::bind_CheckerboardFunction<G, G::dimension, 3, 1>(m, grid_id);
  //  Dune::XT::Functions::bind_CheckerboardFunction<G, G::dimension, 4, 1>(m, grid_id);

  Dune::XT::Functions::bindings::ESV2007::CutoffFunction<G>::bind(m);

  Dune::XT::Functions::bind_ExpressionFunction<G, G::dimension, 1, 1>(m, grid_id);
  Dune::XT::Functions::bind_ExpressionFunction<G, G::dimension, 2, 1>(m, grid_id);
  Dune::XT::Functions::bind_ExpressionFunction<G, G::dimension, 3, 1>(m, grid_id);
  Dune::XT::Functions::bind_ExpressionFunction<G, G::dimension, 4, 1>(m, grid_id);

  Dune::XT::Functions::bind_Spe10Model1Function<G, G::dimension, 1, 1>(m, grid_id);
} // ... addbind_for_Grid(...)


PYBIND11_PLUGIN(_functions)
{
  namespace py = pybind11;
  using namespace pybind11::literals;
  using namespace Dune::XT;

  py::module m("_functions", "dune-xt-functions");
  DUNE_XT_COMMON_BINDINGS_INITIALIZE(m, "dune.xt.functions");

#if HAVE_DUNE_ALUGRID
  using G = ALU_2D_SIMPLEX_CONFORMING;
  addbind_for_Grid<G>(m);

  using E = typename G::template Codim<0>::Entity;
  using D = typename G::ctype;
  static const constexpr size_t d = G::dimension;
  using R = double;
  static const constexpr size_t r = 1;
  static const constexpr size_t rC = 1;

  using type = Functions::IndicatorFunction<E, D, d, R, r, rC>;
  using InterfaceType = Functions::LocalizableFunctionInterface<E, D, d, R, r, rC>;
  using bound_type = pybind11::class_<type, InterfaceType>;

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
#endif

  return m.ptr();
}


#endif // HAVE_DUNE_PYBINDXI
