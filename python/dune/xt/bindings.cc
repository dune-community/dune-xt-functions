// This file is part of the dune-xt-functions project:
//   https://github.com/dune-community/dune-xt-functions
// Copyright 2009-2018 dune-xt-functions developers and contributors. All rights reserved.
// License: Dual licensed as BSD 2-Clause License (http://opensource.org/licenses/BSD-2-Clause)
//      or  GPL-2.0+ (http://opensource.org/licenses/gpl-license)
//          with "runtime exception" (http://www.dune-project.org/license.html)
// Authors:
//   Felix Schindler (2016 - 2017)
//   Rene Milk       (2018)

#include "config.h"

#include <string>
#include <vector>

#include <dune/common/parallel/mpihelper.hh>

#include <dune/pybindxi/pybind11.h>
#include <dune/pybindxi/stl.h>

#include <dune/xt/common/python.hh>
#include <python/dune/xt/common/exceptions.bindings.hh>
#include <python/dune/xt/grid/grids.bindings.hh>

#include <python/dune/xt/functions/interfaces.hh>
#include <python/dune/xt/functions/constant.hh>
#include <python/dune/xt/functions/checkerboard.hh>
#include <python/dune/xt/functions/ESV2007.bindings.hh>
#include <python/dune/xt/functions/expression.hh>
#include <python/dune/xt/functions/spe10.hh>
#include <python/dune/xt/functions/indicator.hh>

#include <python/dune/xt/grid/available_types.hh>

template <class G>
void addbind_for_Grid(pybind11::module& m)
{
  using namespace Dune::XT::Functions;
  const auto grid_id = Dune::XT::Grid::bindings::grid_name<G>::value();
  constexpr const auto diff = internal::Combination::difference;
  constexpr const auto sum = internal::Combination::sum;
  constexpr const auto prod = internal::Combination::product;
  constexpr const auto g_dim = G::dimension;
  Dune::XT::Common::bindings::try_register(m, [&](pybind11::module& mod) {
    auto i_1_1 = bind_LocalizableFunctionInterface<G, 1, 1>(mod, grid_id);
    addbind_LocalizableFunctionInterface_combined_op<G, g_dim, prod, 1, 1, 2, 1>(i_1_1);
    addbind_LocalizableFunctionInterface_combined_op<G, g_dim, prod, 1, 1, 3, 1>(i_1_1);
    addbind_LocalizableFunctionInterface_combined_op<G, g_dim, prod, 1, 1, 4, 1>(i_1_1);
    addbind_LocalizableFunctionInterface_combined_op<G, g_dim, prod, 1, 1, 2, 2>(i_1_1);
  });
  Dune::XT::Common::bindings::try_register(m, [&](pybind11::module& mod) {
    auto i_2_1 = bind_LocalizableFunctionInterface<G, 2, 1>(mod, grid_id);
    addbind_LocalizableFunctionInterface_combined_op<G, g_dim, diff, 2, 1, 2, 1>(i_2_1);
    addbind_LocalizableFunctionInterface_combined_op<G, g_dim, sum, 2, 1, 2, 1>(i_2_1);
  });
  Dune::XT::Common::bindings::try_register(m, [&](pybind11::module& mod) {
    auto i_3_1 = bind_LocalizableFunctionInterface<G, 3, 1>(mod, grid_id);
    addbind_LocalizableFunctionInterface_combined_op<G, g_dim, diff, 3, 1, 3, 1>(i_3_1);
    addbind_LocalizableFunctionInterface_combined_op<G, g_dim, sum, 3, 1, 3, 1>(i_3_1);
  });
  Dune::XT::Common::bindings::try_register(m, [&](pybind11::module& mod) {
    auto i_4_1 = bind_LocalizableFunctionInterface<G, 4, 1>(mod, grid_id);
    addbind_LocalizableFunctionInterface_combined_op<G, g_dim, diff, 4, 1, 4, 1>(i_4_1);
    addbind_LocalizableFunctionInterface_combined_op<G, g_dim, sum, 4, 1, 4, 1>(i_4_1);
    //! this generates multiple binds for the same type
    //! auto i_d_d = bind_LocalizableFunctionInterface<G, g_dim, g_dim>(m, grid_id);
  });
  Dune::XT::Common::bindings::try_register(
      m, [&](pybind11::module& mod) { auto i_d_d = bind_LocalizableFunctionInterface<G, 2, 2>(mod, grid_id); });
  Dune::XT::Common::bindings::try_register(
      m, [&](pybind11::module& mod) { bind_combined_LocalizableFunction<G, g_dim, diff, 1, 1, 1, 1>(mod, grid_id); });
  Dune::XT::Common::bindings::try_register(
      m, [&](pybind11::module& mod) { bind_combined_LocalizableFunction<G, g_dim, diff, 2, 1, 2, 1>(mod, grid_id); });
  Dune::XT::Common::bindings::try_register(
      m, [&](pybind11::module& mod) { bind_combined_LocalizableFunction<G, g_dim, diff, 3, 1, 3, 1>(mod, grid_id); });
  Dune::XT::Common::bindings::try_register(
      m, [&](pybind11::module& mod) { bind_combined_LocalizableFunction<G, g_dim, diff, 4, 1, 4, 1>(mod, grid_id); });
  Dune::XT::Common::bindings::try_register(m, [&](pybind11::module& mod) {
    bind_combined_LocalizableFunction<G, g_dim, diff, 2, 2, 2, 2>(mod, grid_id);
    //  addbind_LocalizableFunctionInterface_combined_op<G, g_dim, diff, g_dim, g_dim, g_dim, g_dim>(i_d_d);
  });
  Dune::XT::Common::bindings::try_register(
      m, [&](pybind11::module& mod) { bind_combined_LocalizableFunction<G, g_dim, sum, 1, 1, 1, 1>(mod, grid_id); });
  Dune::XT::Common::bindings::try_register(
      m, [&](pybind11::module& mod) { bind_combined_LocalizableFunction<G, g_dim, sum, 2, 1, 2, 1>(mod, grid_id); });
  Dune::XT::Common::bindings::try_register(
      m, [&](pybind11::module& mod) { bind_combined_LocalizableFunction<G, g_dim, sum, 3, 1, 3, 1>(mod, grid_id); });
  Dune::XT::Common::bindings::try_register(
      m, [&](pybind11::module& mod) { bind_combined_LocalizableFunction<G, g_dim, sum, 4, 1, 4, 1>(mod, grid_id); });
  Dune::XT::Common::bindings::try_register(m, [&](pybind11::module& mod) {
    bind_combined_LocalizableFunction<G, g_dim, sum, 2, 2, 2, 2>(mod, grid_id);
    //  addbind_LocalizableFunctionInterface_combined_op<G, g_dim, sum, g_dim, g_dim, g_dim, g_dim>(i_d_d);
  });
  Dune::XT::Common::bindings::try_register(
      m, [&](pybind11::module& mod) { bind_combined_LocalizableFunction<G, g_dim, prod, 1, 1, 1, 1>(mod, grid_id); });
  Dune::XT::Common::bindings::try_register(
      m, [&](pybind11::module& mod) { bind_combined_LocalizableFunction<G, g_dim, prod, 1, 1, 2, 1>(mod, grid_id); });
  Dune::XT::Common::bindings::try_register(
      m, [&](pybind11::module& mod) { bind_combined_LocalizableFunction<G, g_dim, prod, 1, 1, 3, 1>(mod, grid_id); });
  Dune::XT::Common::bindings::try_register(
      m, [&](pybind11::module& mod) { bind_combined_LocalizableFunction<G, g_dim, prod, 1, 1, 4, 1>(mod, grid_id); });
  Dune::XT::Common::bindings::try_register(
      m, [&](pybind11::module& mod) { bind_combined_LocalizableFunction<G, g_dim, prod, 1, 1, 2, 2>(mod, grid_id); });
  Dune::XT::Common::bindings::try_register(
      m, [&](pybind11::module& mod) { bind_ConstantFunction<G, g_dim, 1, 1>(mod, grid_id); });
  Dune::XT::Common::bindings::try_register(
      m, [&](pybind11::module& mod) { bind_ConstantFunction<G, g_dim, 2, 1>(mod, grid_id); });
  Dune::XT::Common::bindings::try_register(
      m, [&](pybind11::module& mod) { bind_ConstantFunction<G, g_dim, 3, 1>(mod, grid_id); });
  Dune::XT::Common::bindings::try_register(
      m, [&](pybind11::module& mod) { bind_ConstantFunction<G, g_dim, 4, 1>(mod, grid_id); });
  Dune::XT::Common::bindings::try_register(
      m, [&](pybind11::module& mod) { bind_ConstantFunction<G, g_dim, 2, 2>(mod, grid_id); });
  Dune::XT::Common::bindings::try_register(m, [&](pybind11::module& mod) {
    bind_CheckerboardFunction<G, g_dim, 1, 1>(mod, grid_id);
    //  bind_CheckerboardFunction<G, g_dim, 2, 1>(m, grid_id);
    //  bind_CheckerboardFunction<G, g_dim, 3, 1>(m, grid_id);
    //  bind_CheckerboardFunction<G, g_dim, 4, 1>(m, grid_id);
  });
  Dune::XT::Common::bindings::try_register(
      m, [&](pybind11::module& mod) { bindings::ESV2007::CutoffFunction<G>::bind(mod); });
  Dune::XT::Common::bindings::try_register(
      m, [&](pybind11::module& mod) { bind_ExpressionFunction<G, g_dim, 1, 1>(mod, grid_id); });
  Dune::XT::Common::bindings::try_register(
      m, [&](pybind11::module& mod) { bind_ExpressionFunction<G, g_dim, 2, 1>(mod, grid_id); });
  Dune::XT::Common::bindings::try_register(
      m, [&](pybind11::module& mod) { bind_ExpressionFunction<G, g_dim, 3, 1>(mod, grid_id); });
  Dune::XT::Common::bindings::try_register(
      m, [&](pybind11::module& mod) { bind_ExpressionFunction<G, g_dim, 4, 1>(mod, grid_id); });
  Dune::XT::Common::bindings::try_register(
      m, [&](pybind11::module& mod) { bind_Spe10Model1Function<G, g_dim, 1, 1>(mod, grid_id); });
  Dune::XT::Common::bindings::try_register(
      m, [&](pybind11::module& mod) { bind_IndicatorFunction<G, g_dim, 1, 1>(mod, grid_id); });

} // ... addbind_for_Grid(...)


template <class Tuple = Dune::XT::Grid::bindings::AvailableTypes>
void all_grids(pybind11::module& m)
{
  addbind_for_Grid<typename Tuple::head_type>(m);
  all_grids<typename Tuple::tail_type>(m);
} // ... addbind_for_Grid(...)


template <>
void all_grids<boost::tuples::null_type>(pybind11::module&)
{}


PYBIND11_MODULE(_functions, m)
{
  namespace py = pybind11;
  using namespace pybind11::literals;

  py::module::import("dune.xt.common");
  py::module::import("dune.xt.grid");

  all_grids(m);
  Dune::XT::Common::bindings::add_initialization(m, "dune.xt.functions");
}
