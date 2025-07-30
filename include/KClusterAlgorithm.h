class KClusterAlgorithm
{
private:
	
	const int number_of_color_channels;

	class Cluster
	{
	private:

		const int number_of_color_channels;
		double* const position;

	public:

		Cluster(int number_of_color_channels);
		~Cluster();
	};

	int current_number_of_clusters;
	Cluster* clusters;


public:

	KClusterAlgorithm();
	~KClusterAlgorithm();

	// use these to update the k-means algorithm to 

};