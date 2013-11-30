#include <memory>

#include "gene/classic/operators.hpp"

namespace gene { namespace classic { namespace operators {

BitFlipMutation::BitFlipMutation (float percentageOfMutation)
  : percentageOfMutation_(percentageOfMutation)
{
  // do nothing
}

std::unique_ptr<Genotipe>
BitFlipMutation::mutate(const Genotipe&)
{
  //TODO
}

NPointCrossover::NPointCrossover (std::size_t numberOfPoints)
  : numberOfPoints_(numberOfPoints)
{
  // do nothing
}

std::unique_ptr<Genotipe>
NPointCrossover::combine(const Genotipe&, const Genotipe&)
{
  //TODO
}

}}}
