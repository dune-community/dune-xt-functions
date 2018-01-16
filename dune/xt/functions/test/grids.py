from dune.xt.codegen import typeid_to_typedef_name

dimDomain = (1, 3)

def _make_alu():
    geometry = ['cube', 'simplex']
    refinement = ['nonconforming', 'conforming']
    perms = itertools.product(dimDomain, refinement)
    fmt = 'Dune::ALUGrid<{d},{d},Dune::{g},Dune::{r}>'
    return [fmt.format(d=d, g=g, r=r) for d, g, r in itertools.product([d for d in dimDomain if d != 1], geometry, refinement)
            if  g != 'cube' or r == 'nonconforming']

def _if_active(val, guard, cache):
    return val if cache[guard] else []

def types(cache):
    grid_alu = _if_active(_make_alu, 'dune-alugrid', cache)
    grid_yasp = ['Dune::YaspGrid<{d},Dune::EquidistantOffsetCoordinates<double,{d}>>'.format(d=d) for d in dimDomain]
    grid_ug = _if_active(['Dune::UGGrid<{}>'.format(d) for d in dimDomain if d != 1], 'dune-uggrid', cache)
    grid_alberta = _if_active(['Dune::AlbertaGrid<{d},{d}>'.format(d=d) for d in dimDomain], 'ALBERTA_FOUND', cache)
    return grid_yasp + grid_ug + grid_alberta #+grid_alu +

def names(grids):
    return [typeid_to_typedef_name(g) for g in grids]

def entities(grids):
    return ['{}::Codim<0>::Entity'.format(g) for g in grids]

