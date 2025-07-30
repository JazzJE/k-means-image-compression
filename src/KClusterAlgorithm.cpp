#include "KClusterAlgorithm.h"
#include "Constants.h"

KClusterAlgorithm::KClusterAlgorithm() : number_of_color_channels(Constants::number_of_color_channels), clusters(nullptr), current_number_of_clusters(0)
{

}

KClusterAlgorithm::~KClusterAlgorithm()
{

}