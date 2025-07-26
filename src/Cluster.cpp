#include "KClusterAlgorithm.h"

KClusterAlgorithm::Cluster::Cluster(int number_of_dimensions) : number_of_dimensions(number_of_dimensions), position(new double[number_of_dimensions])
{ }

KClusterAlgorithm::Cluster::~Cluster()
{ }

