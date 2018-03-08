// This file is part of the dune-xt-functions project:
//   https://github.com/dune-community/dune-xt-functions
// Copyright 2009-2017 dune-xt-functions developers and contributors. All rights reserved.
// License: Dual licensed as BSD 2-Clause License (http://opensource.org/licenses/BSD-2-Clause)
//      or  GPL-2.0+ (http://opensource.org/licenses/gpl-license)
//          with "runtime exception" (http://www.dune-project.org/license.html)
// Authors:
//   Felix Schindler (2017)

#ifndef DUNE_XT_FUNCTIONS_EXCEPTIONS_HH
#define DUNE_XT_FUNCTIONS_EXCEPTIONS_HH

#include <dune/xt/common/exceptions.hh>

namespace Dune {
namespace XT {
namespace Functions {
namespace Exceptions {


class wrong_input_given : public Common::Exceptions::wrong_input_given
{
};

class this_function_is_not_bound_to_an_element_yet : public Dune::InvalidStateException
{
};


} // namespace Exceptions
} // namespace Functions
} // namespace XT
} // namespace Dune

#endif // DUNE_XT_FUNCTIONS_EXCEPTIONS_HH