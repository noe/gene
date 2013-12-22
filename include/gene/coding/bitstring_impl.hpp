// Copyright (c) 2013, Noe Casas (noe.casas@gmail.com).
// Distributed under New BSD License.
// (see accompanying file COPYING)

namespace gene { namespace bitstring {

/*****************************************************************************/
BitFlipMutation::BitFlipMutation (float bitMutationProbability)
  : percentageOfMutation_(bitMutationProbability)
{
  //TODO
}

/*****************************************************************************/
std::unique_ptr<Genotype> BitFlipMutation::mutate(const Genotype&)
{
  //TODO
}

/*****************************************************************************/
OnePointCrossover::OnePointCrossover ()
{
  //TODO
}

/*****************************************************************************/
std::unique_ptr<Genotype> OnePointCrossover::combine(const Genotype&, const Genotype&)
{
  //TODO
}

}}
