// Copyright (c) 2013, Noe Casas (noe.casas@gmail.com).
// Distributed under New BSD License.
// (see accompanying file COPYING)

#ifndef COMBINATION_HEADER_SEEN_
#define COMBINATION_HEADER_SEEN_

#include <memory>

namespace gene {

/****************************************************************************
 * Interface abstracting the combination of two Individuals.
 ***************************************************************************/
template<typename Genotype>
struct CombinationStrategy
{
  virtual std::unique_ptr<Genotype> combine(const Genotype&,
                                            const Genotype&) = 0;
  virtual ~CombinationStrategy() { }
};

/****************************************************************************
 * Strategy for evaluating the level of attraction between individuals.
 ***************************************************************************/
template<typename Individual>
struct AttractionMeter
{
  virtual attractionBetween(const Individual&, const Individual&) = 0;
  virtual ~AttractionMeter() { };
};

/****************************************************************************
 * Implementation of Combination that performs N point crossover.
 ***************************************************************************/
struct NPointCrossover : public Combination, boost::noncopyable
{
  NPointCrossover (std::size_t numberOfPoints);
  std::unique_ptr<Genotype> combine(const Genotype&, const Genotype&);
  private: const std::size_t numberOfPoints_;
};

}
#endif
