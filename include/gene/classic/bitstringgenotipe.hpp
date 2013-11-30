// Copyright (c) 2013, Noe Casas (noe.casas@gmail.com).
// Distributed under New BSD License.
// (see accompanying file COPYING)

#ifndef BITSTRING_GENOTIPE_HEADER_SEEN___
#define BITSTRING_GENOTIPE_HEADER_SEEN___

#include <memory>
#include <vector>
#include <string>
#include <stdexcept>
#include <boost/noncopyable.hpp>

namespace gene { namespace classic
{

/******************************************************************************
 * PoD representing a chromosome.
 * Immutable sequence of encoded genes.
 *****************************************************************************/
struct Chromosome : boost::noncopyable
{
  const std::vector<uint8_t> encodedGenes;
};

/******************************************************************************
 * PoD representing the genotype of an individual.
 * Immutable sequence of chromosomes.
 *****************************************************************************/
struct Genotipe : boost::noncopyable
{
  const std::vector<std::unique_ptr<Chromosome>> chromosomes;
};

}
