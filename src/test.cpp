#include "gene/bitstring.hpp"
#include "gene/policies.hpp"
#include "gene/algorithm.hpp"

int main(void)
{
  gene::bitstring::Genotype g;
  gene::ConstantMutationRate<uint32_t> constantRate(0.5);
  gene::OnePointCrossover crossover;
  return 0;
}
