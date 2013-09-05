// Copyright (c) 2013, Noe Casas (noe.casas@gmail.com).
// Distributed under New BSD License.
// (see accompanying file COPYING)

#ifndef INDIVIDUAL_HEADER_SEEN___
#define INDIVIDUAL_HEADER_SEEN___

#include <memory>
#include <vector>
#include <string>
#include <boost/noncopyable.hpp>

namespace gene
{

/******************************************************************************
 * PoD representing a chromosome.
 * Immutable sequence of encoded genes.
 *****************************************************************************/
struct Chromosome : boost::noncopyable
{
  const std::vector<uint8_t> encodedGenes1;
  const std::vector<uint8_t> encodedGenes2;
};

/******************************************************************************
 * PoD representing the genotype of an individual.
 * Immutable sequence of chromosomes.
 *****************************************************************************/
struct Genotipe : boost::noncopyable
{
  const std::vector<std::unique_ptr<Chromosome>>;
}

/******************************************************************************
 * Interface abstracting a factory of Individuals.
 * Implementations of IndividualFactory shall create Indidividuals from
 * their Genotipe.
 *****************************************************************************/
template<Individual> struct IndividualFactory
{
  virtual std::string description() const () = 0;

  virtual std::unique_ptr<Individual> create(const Genotipe&)
                                 throw(std::invalid_argument) = 0;

  virtual ~IndividualFactory() { /* do nothing */ }
};

}
#endif
