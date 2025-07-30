#include "KClusterAlgorithm.h"

KClusterAlgorithm::Cluster::Cluster(int number_of_color_channels) : number_of_color_channels(number_of_color_channels), position(new double[number_of_color_channels])
{ }

KClusterAlgorithm::Cluster::~Cluster()
{ }

