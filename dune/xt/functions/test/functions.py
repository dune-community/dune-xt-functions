import grids
import itertools
from dune.xt.codegen import typeid_to_typedef_name


dimRange = [1, 3]
dimRangeCols = [1, 3]

grid_types = grids.types(cache)
fmt = 'Dune::XT::Functions::ConstantFunction<{e}, double, {e}::dimension, double, {dr}, {drc}>'
functions = [fmt.format(e=e, dr=dr, drc=drc)
             for e, dr, drc in itertools.product(grids.entities(grid_types), dimRange, dimRangeCols)]
names_and_types = [(typeid_to_typedef_name(f), f) for f in functions]
