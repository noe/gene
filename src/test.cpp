#include "gene/coding/dna.hpp"
#include "gene/policies.hpp"
#include "gene/algorithm.hpp"

#include <utility>

int main(void)
{
  std::vector<gene::coding::dna::Chromosome> chromosomes;
  gene::coding::dna::Genotype g(std::move(chromosomes));
  return 0;
}
