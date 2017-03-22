from dune.xt.codegen import typeid_to_typedef_name

dimDomain = (1, 3)

def make_alu():
    geometry = ['cube', 'simplex']
    refinement = ['nonconforming', 'conforming']
    perms = itertools.product(dimDomain, refinement
    fmt = 'Dune::ALUGrid<{d},{d},Dune::{g},Dune::{r}>'
    return [fmt.format(d=d, g=g, r=r) for d, g, r in itertools.product([d for d in dimDomain if d != 1], geometry, refinement)
            if  g != 'cube' or r == 'nonconforming']

def if_active(val, guard):
    return val if cache[guard] else []

grid_alu = if_active(make_alu, 'dune-alugrid')
grid_yasp = ['Dune::YaspGrid<{d},Dune::EquidistantOffsetCoordinates<double,{d}>>'.format(d=d) for d in dimDomain]
grid_ug = if_active(['Dune::UGGrid<{d}>'.format(d) for d in dimDomain if d != 1], 'dune-uggrid')
grid_alberta = if_active(['Dune::AlbertaGrid<{d},{dimDomain}>'.format(d) for d in dimDomain], 'HAVE_ALBERTA')

grids = grid_yasp + grid_alu + grid_ug + grid_alberta
names = [typeid_to_typedef_name(g) for g in grids]
entities = ['{}::Codim<0>::Entity'.format(g) for g in grids]

