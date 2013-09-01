// Copyright (c) 2013, Noe Casas (noe.casas@gmail.com).
// Distributed under New BSD License.
// (see accompanying file COPYING)

#ifndef OPERATORS_HEADER_SEEN_
#define OPERATORS_HEADER_SEEN_

#include "individual.hpp"

namespace gene { namespace operators
{
  struct Mutation
  {
    virtual Genotipe mutate(const Genotipe&) = 0;
    virtual ~Mutation() { /* do nothing */ }
  };

  struct Crossover
  {
    virtual Genotipe combine(const Genotipe&, const Genotipe&) = 0;
    virtual ~Crossover() { /* do nothing */ }
  };
  
}}
#endif
