# ~~~
# This file is part of the dune-xt-functions project:
#   https://github.com/dune-community/dune-xt-functions
# Copyright 2009-2018 dune-xt-functions developers and contributors. All rights reserved.
# License: Dual licensed as BSD 2-Clause License (http://opensource.org/licenses/BSD-2-Clause)
# Authors:
#   Rene Milk (2018)
# ~~~

import pytest
from dune.xt.common.test import load_all_submodule
from dune.xt.grid import types, provider
import dune.xt.grid as xtg
import dune.xt.functions as xtf


def test_load_all():
    import dune.xt.functions as xtc
    load_all_submodule(xtc)


    func = xtf.make_constant_function_1x1(grid, 1)
    func = xtf.make_constant_function_2x1(grid, 1)
