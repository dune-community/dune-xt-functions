// This file is part of the dune-xt-functions project:
//   https://github.com/dune-community/dune-xt-functions
// Copyright 2009-2018 dune-xt-functions developers and contributors. All rights reserved.
// License: Dual licensed as BSD 2-Clause License (http://opensource.org/licenses/BSD-2-Clause)
//      or  GPL-2.0+ (http://opensource.org/licenses/gpl-license)
//          with "runtime exception" (http://www.dune-project.org/license.html)
// Authors:
//   Felix Schindler (2013 - 2017)
//   Rene Milk       (2014 - 2016, 2018)
//   Tobias Leibner  (2014)

#ifndef DUNE_XT_FUNCTIONS_SPE10_MODEL1_HH
#define DUNE_XT_FUNCTIONS_SPE10_MODEL1_HH

#include <iostream>
#include <memory>

#include <dune/xt/common/color.hh>
#include <dune/xt/common/configuration.hh>
#include <dune/xt/common/exceptions.hh>
#include <dune/xt/common/fvector.hh>
#include <dune/xt/common/string.hh>
#include <dune/xt/common/type_traits.hh>
#include <dune/xt/data/paths.hh>

#include "../checkerboard.hh"

namespace Dune {
namespace XT {
namespace Exceptions {

class spe10_data_file_missing : public Dune::IOError
{};

} // namespace Exceptions
namespace Functions {
namespace Spe10 {
namespace internal {

static const size_t model1_x_elements = 100;
static const size_t model1_y_elements = 1;
static const size_t model1_z_elements = 20;
static const double model_1_length_x = 762.0;
static const double model_1_length_y = 7.62;
static const double model_1_length_z = 15.24;
static const double model1_min_value = 0.001;
static const double model1_max_value = 998.915;

template <class EntityImp, class DomainFieldImp, class RangeFieldImp, size_t r, size_t rC>
class Model1Base : public CheckerboardFunction<EntityImp, DomainFieldImp, 2, RangeFieldImp, r, rC>
{
  typedef CheckerboardFunction<EntityImp, DomainFieldImp, 2, RangeFieldImp, r, rC> BaseType;

public:
  typedef typename BaseType::EntityType EntityType;
  typedef typename BaseType::LocalfunctionType LocalfunctionType;

  typedef typename BaseType::DomainType DomainType;
  typedef typename BaseType::RangeFieldType RangeFieldType;
  typedef typename BaseType::RangeType RangeType;

  static const bool available = true;

  static std::string static_id()
  {
    return LocalizableFunctionInterface<EntityImp, DomainFieldImp, 2, RangeFieldImp, 1, 1>::static_id()
           + ".spe10.model1";
  } // ... static_id(...)

private:
  static std::vector<RangeType> read_values_from_file(const std::string& filename,
                                                      const RangeFieldType& min,
                                                      const RangeFieldType& max,
                                                      const RangeType& unit_range)

  {
    if (!(max > min))
      DUNE_THROW(Dune::RangeError, "max (is " << max << ") has to be larger than min (is " << min << ")!");
    const RangeFieldType scale = (max - min) / (internal::model1_max_value - internal::model1_min_value);
    const RangeFieldType shift = min - scale * internal::model1_min_value;
    // read all the data from the file
    std::ifstream datafile(filename);
    if (datafile.is_open()) {
      static const size_t entriesPerDim = model1_x_elements * model1_y_elements * model1_z_elements;
      // create storage (there should be exactly 6000 values in the file, but we onyl read the first 2000)
      std::vector<RangeType> data(entriesPerDim, unit_range);
      double tmp = 0;
      size_t counter = 0;
      while (datafile >> tmp && counter < entriesPerDim)
        data[counter++] *= (tmp * scale) + shift;
      datafile.close();
      if (counter != entriesPerDim)
        DUNE_THROW(Dune::IOError,
                   "wrong number of entries in '" << filename << "' (are " << counter << ", should be " << entriesPerDim
                                                  << ")!");
      return data;
    } else
      DUNE_THROW(Exceptions::spe10_data_file_missing, "could not open '" << filename << "'!");
  } // ... read_values_from_file(...)

public:
  static Common::Configuration default_config(const std::string sub_name = "")
  {
    Common::Configuration config;
    config["filename"] = Data::spe10_model1_filename();
    config["lower_left"] = "[0.0 0.0]";
    config["upper_right"] =
        "[" + Common::to_string(internal::model_1_length_x) + " " + Common::to_string(internal::model_1_length_z) + "]";
    config.set("min_value", internal::model1_min_value);
    config.set("max_value", internal::model1_max_value);
    config["name"] = static_id();
    if (sub_name.empty())
      return config;
    else {
      Common::Configuration tmp;
      tmp.add(config, sub_name);
      return tmp;
    }
  } // ... default_config(...)

  template <class DerivedType>
  static std::unique_ptr<DerivedType> create_derived(const Common::Configuration config, const std::string sub_name)
  {
    // get correct config
    const Common::Configuration cfg = config.has_sub(sub_name) ? config.sub(sub_name) : config;
    const Common::Configuration default_cfg = default_config();
    // create
    return Common::make_unique<DerivedType>(cfg.get("filename", default_cfg.get<std::string>("filename")),
                                            cfg.get("lower_left", default_cfg.get<DomainType>("lower_left")),
                                            cfg.get("upper_right", default_cfg.get<DomainType>("upper_right")),
                                            cfg.get("min_value", default_cfg.get<RangeFieldType>("min_value")),
                                            cfg.get("max_value", default_cfg.get<RangeFieldType>("max_value")),
                                            cfg.get("name", default_cfg.get<std::string>("name")));
  } // ... create(...)

  Model1Base(const std::string& filename,
             const DomainType& lowerLeft,
             const DomainType& upperRight,
             const RangeFieldType min,
             const RangeFieldType max,
             const std::string nm,
             const RangeType& unit_range)
    : BaseType(lowerLeft,
               upperRight,
               {model1_x_elements, model1_z_elements},
               read_values_from_file(filename, min, max, unit_range),
               nm)
  {}

  virtual std::string type() const override
  {
    return BaseType::static_id() + ".spe10.model1";
  }
}; // class Model1Base

} // namespace internal

// default, to allow for specialization
template <class E, class D, size_t d, class R, size_t r, size_t rC = 1>
class Model1Function : public LocalizableFunctionInterface<E, D, d, R, r, rC>
{
  Model1Function()
  {
    static_assert(AlwaysFalse<E>::value, "Not available for these dimensions!");
  }
};

/**
 * We read only the Kx values from file and scale the unit matrix atm.
 */
template <class EntityImp, class DomainFieldImp, class RangeFieldImp, size_t r>
class Model1Function<EntityImp, DomainFieldImp, 2, RangeFieldImp, r, r>
  : public internal::Model1Base<EntityImp, DomainFieldImp, RangeFieldImp, r, r>
{
  typedef internal::Model1Base<EntityImp, DomainFieldImp, RangeFieldImp, r, r> BaseType;
  typedef Model1Function<EntityImp, DomainFieldImp, 2, RangeFieldImp, r, r> ThisType;

public:
  using BaseType::dimDomain;
  using typename BaseType::DomainFieldType;
  using typename BaseType::RangeFieldType;
  using typename BaseType::RangeType;

  static std::unique_ptr<ThisType> create(const Common::Configuration config = BaseType::default_config(),
                                          const std::string sub_name = BaseType::static_id())
  {
    return BaseType::template create_derived<ThisType>(config, sub_name);
  } // ... create(...)

  Model1Function(const std::string& filename,
                 const Common::FieldVector<DomainFieldType, dimDomain>& lower_left,
                 const Common::FieldVector<DomainFieldType, dimDomain>& upper_right,
                 const RangeFieldType min = internal::model1_min_value,
                 const RangeFieldType max = internal::model1_max_value,
                 const std::string nm = BaseType::static_id())
    : BaseType(filename, lower_left, upper_right, min, max, nm, unit_matrix())
  {}

private:
  template <size_t d, bool anything = true>
  struct Call
  {
    static RangeType unit_matrix()
    {
      RangeType ret(0.0);
      for (size_t rr = 0; rr < r; ++rr)
        ret[rr][rr] = 1.0;
      return ret;
    }
  };

  template <bool anything>
  struct Call<1, anything>
  {
    static RangeType unit_matrix()
    {
      return RangeType(1);
    }
  };

  static RangeType unit_matrix()
  {
    return Call<r>::unit_matrix();
  }
}; // class Model1< ..., 2, ..., r, r >

} // namespace Spe10
} // namespace Functions
} // namespace XT
} // namespace Dune


#include "model1.lib.hh"


#endif // DUNE_XT_FUNCTIONS_SPE10_MODEL1_HH
