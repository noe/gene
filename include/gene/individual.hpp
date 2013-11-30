// Copyright (c) 2013, Noe Casas (noe.casas@gmail.com).
// Distributed under New BSD License.
// (see accompanying file COPYING)

#ifndef INDIVIDUAL_HEADER_SEEN___
#define INDIVIDUAL_HEADER_SEEN___

#include <memory>
#include <string>
#include <stdexcept>

namespace gene {

/******************************************************************************
 * Interface abstracting a factory of Individuals.
 * Implementations of IndividualFactory shall create Indidividuals from
 * their Genotype.
 *****************************************************************************/
template<typename Individual, typename Genotype>
struct IndividualFactory
{
  virtual std::string description() const = 0;

  virtual std::unique_ptr<Individual> create(const Genotype&)
                                 throw(std::invalid_argument) = 0;

  virtual ~IndividualFactory() { }
};

}
#endif
