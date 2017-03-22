import grids

dimRange = [1, 3]
dimRangeCols = [1, 3]

functions = ['Dune::XT::Functions::ConstantFunction<{e}, double, {e}::dimension, double, {dr}, {drc}>'.format(e=e, dd=dd, dr=dr, drc=drc)
             for e, dr, drc in itertools.product(grids.entities, dimRange, dimRangeCols)]
