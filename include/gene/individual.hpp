// Copyright (c) 2013, Noe Casas (noe.casas@gmail.com).
// Distributed under New BSD License.
// (see accompanying file COPYING)

#ifndef INDIVIDUAL_HEADER_SEEN___
#define INDIVIDUAL_HEADER_SEEN___

#include <memory>
#include <string>
#include <stdexcept>
#include <boost/noncopyable.hpp>

namespace gene {

/******************************************************************************
 * Interface abstracting a factory of Individuals.
 * Implementations of IndividualFactory shall create Indidividuals from
 * their Genotipe.
 *****************************************************************************/
template<typename Individual, typename Genotype>
struct IndividualFactory
{
  virtual std::string description() const = 0;

  virtual std::unique_ptr<Individual> create(const Genotipe&)
                                 throw(std::invalid_argument) = 0;

  virtual ~IndividualFactory() { /* do nothing */ }
};

}
#endif
