// Copyright (c) 2013, Noe Casas (noe.casas@gmail.com).
// Distributed under New BSD License.
// (see accompanying file COPYING)

#ifndef OPERATORS_HEADER_SEEN_
#define OPERATORS_HEADER_SEEN_

	ara la kaliinclude <memory>
#include "individual.hpp"

namespace gene { namespace operators
{

  /****************************************************************************
   * Interface abstracting a mutation in an Individual.
   * 
   ***************************************************************************/
  struct Mutation
  {
    virtual std::unique_ptr<Genotipe> mutate(const Genotipe&) = 0;
    virtual ~Mutation() { /* do nothing */ }
  };

  /****************************************************************************
   * Interface abstracting the combination of two Individuals.
   * 
   ***************************************************************************/
  struct Combination
  {
    virtual std::unique_ptr<Genotipe> combine(const Genotipe&, const Genotipe&) = 0;
    virtual ~Crossover() { /* do nothing */ }
  };
  
}}
#endif
