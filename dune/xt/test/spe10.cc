// This file is part of the dune-xt-functions project:
//   https://github.com/dune-community/dune-xt-functions
// The copyright lies with the authors of this file (see below).
// License: BSD 2-Clause License (http://opensource.org/licenses/BSD-2-Clause)
// Authors:
//   Rene Milk (2015)

#include <dune/xt/test/main.hxx>
#include "functions.hh"

#include <memory>

#include <dune/common/exceptions.hh>

#include <dune/xt/functions/interfaces.hh>
#include <dune/xt/functions/spe10/model1.hh>
#include <dune/xt/functions/spe10/model2.hh>

#include <dune/grid/yaspgrid.hh>
#if HAVE_ALUGRID
#include <dune/grid/alugrid.hh>
#endif // HAVE_ALUGRID

using namespace Dune;
using namespace Dune::XT;

struct Spe10Model2FunctionTest : public FunctionTest<TESTFUNCTIONTYPE>
{
  void check() const
  {
    const FunctionType zero;
  }
};


TEST_F(Spe10Model2FunctionTest, provides_required_methods)
{
  this->check();
}
