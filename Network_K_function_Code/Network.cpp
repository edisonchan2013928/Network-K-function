#include "Network.h"

void init_paremeters(int argc, char**argv, model& our_model)
{
	//testing
	/*our_model.network_fileName = (char*)"../../../Datasets/Testing/Testing_network";
	our_model.method = 4;
	our_model.tau = 6;*/
	our_model.network_fileName = argv[1];
	our_model.method = atoi(argv[2]);
	our_model.tau = atoi(argv[3]);
}

void load_Network(model& our_model)
{
	fstream network_file;
	int num_nodes;
	int num_edges;
	int num_points;
	Point pt;

	network_file.open(our_model.network_fileName);
	if (network_file.is_open() == false)
	{
		cout << "Cannot open this network file." << endl;
		exit(0);
	}

	network_file >> num_edges;
	our_model.num_edges = num_edges;
	//Process edge_set
	our_model.edge_set = new Edge[num_edges];
	for (int e = 0; e < num_edges; e++)
	{
		network_file >> our_model.edge_set[e].n1;
		network_file >> our_model.edge_set[e].n2;
		network_file >> our_model.edge_set[e].length;
		network_file >> num_points;

		for (int p = 0; p < num_points; p++)
		{
			pt.edge_index = e;
			network_file >> pt.dist_n1;
			network_file >> pt.dist_n2;
			our_model.edge_set[e].PS.push_back(pt);
		}
	}

	//Process the road network
	vector<int> edge_index_vec;
	int degree;
	int edge_index;
	network_file >> num_nodes;
	our_model.num_nodes = num_nodes;
	for (int n = 0; n < num_nodes; n++)
	{
		edge_index_vec.clear();

		network_file >> degree;
		for (int d = 0; d < degree; d++)
		{
			network_file >> edge_index;
			edge_index_vec.push_back(edge_index);
		}
		
		our_model.Network.push_back(edge_index_vec);
	}
	
	network_file.close();
}

void output_result(model& our_model)
{
	//Only focus on calculating a single K-function value
	cout << "The network K-function value is: " << our_model.K_value << endl;
}