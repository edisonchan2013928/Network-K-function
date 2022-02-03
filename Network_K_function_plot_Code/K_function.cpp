#include "K_function.h"

void call_RQS(model& our_model)
{
	//Preprocessing
	int e_index;
	int node_u, node_v;
	our_model.multiple_K_values[our_model.cur_data_index][our_model.cur_tau_index] = 0;
	Edge*edge_set = our_model.edge_set;
	Dijkstra_preprocess(our_model);

	for (int wide_e_index = 0; wide_e_index < our_model.num_edges; wide_e_index++)
	{
		our_model.cur_edge_index = wide_e_index;
		vector<Point>& PS = edge_set[wide_e_index].PS_vec[our_model.cur_data_index];
		for (int p = 0; p < (int)PS.size(); p++)
		{
			our_model.cur_point_index = p;
			Dijkstra(our_model, our_model.sp_node_vec_first, our_model.access_edge_list_first, our_model.is_scan_edge_list_first);
			for (int e = 0; e < (int)our_model.access_edge_list_first.size(); e++)
			{
				e_index = our_model.access_edge_list_first[e];

				if (wide_e_index != e_index)
				{
					node_u = edge_set[e_index].n1;
					node_v = edge_set[e_index].n2;
					if (our_model.sp_node_vec_first[node_u].cur_sp_value > our_model.tau_vec[our_model.cur_tau_index] &&
						our_model.sp_node_vec_first[node_v].cur_sp_value > our_model.tau_vec[our_model.cur_tau_index])
						continue;

					//call point-edge_count function
					our_model.multiple_K_values[our_model.cur_data_index][our_model.cur_tau_index] += point_edge_count(our_model, e_index, node_u, node_v);
				}
				else
					our_model.multiple_K_values[our_model.cur_data_index][our_model.cur_tau_index] += point_edge_count_same_edge(our_model, p, e_index);
			}

			clear_Dijkstra(edge_set, our_model.sp_node_vec_first, our_model.access_edge_list_first, our_model.is_scan_edge_list_first);
		}
	}
}

void call_SPS(model& our_model)
{
	//Preprocessing
	int e_index;
	int node_u, node_v;
	our_model.multiple_K_values[our_model.cur_data_index][our_model.cur_tau_index] = 0;
	Edge*edge_set = our_model.edge_set;
	//int edges_totalNum;
	int num_edges_two_sets;
	Dijkstra_preprocess(our_model);

	for (int wide_e_index = 0; wide_e_index < our_model.num_edges; wide_e_index++)
	{
		our_model.cur_edge_index = wide_e_index;
		vector<Point>& PS = edge_set[wide_e_index].PS_vec[our_model.cur_data_index];
		if (PS.size() == 0)
			continue;

		our_model.is_first = true;
		Dijkstra(our_model, our_model.sp_node_vec_first, our_model.access_edge_list_first, our_model.is_scan_edge_list_first);
		our_model.is_first = false;
		Dijkstra(our_model, our_model.sp_node_vec_second, our_model.access_edge_list_second, our_model.is_scan_edge_list_second);
		num_edges_two_sets = (int)our_model.access_edge_list_first.size() + (int)our_model.access_edge_list_second.size();

		for (int e = 0; e < num_edges_two_sets; e++)
		{
			if (e < (int)our_model.access_edge_list_first.size())
			{
				e_index = our_model.access_edge_list_first[e];
				our_model.detect_DoubleScan_edge_list[e_index] = true;
			}
			else
			{
				e_index = our_model.access_edge_list_second[e - our_model.access_edge_list_first.size()];
				if (our_model.detect_DoubleScan_edge_list[e_index] == true)
					continue;
				else
					our_model.detect_DoubleScan_edge_list[e_index] = true;
			}

			if (wide_e_index != e_index)
			{
				node_u = edge_set[e_index].n1;
				node_v = edge_set[e_index].n2;
				if (our_model.sp_node_vec_first[node_u].cur_sp_value > our_model.tau_vec[our_model.cur_tau_index] &&
					our_model.sp_node_vec_first[node_v].cur_sp_value > our_model.tau_vec[our_model.cur_tau_index] &&
					our_model.sp_node_vec_second[node_u].cur_sp_value > our_model.tau_vec[our_model.cur_tau_index] &&
					our_model.sp_node_vec_second[node_v].cur_sp_value > our_model.tau_vec[our_model.cur_tau_index])
					continue;

				//SPS and CA methods
				if (our_model.method == 2 || our_model.method == 3)
				{
					for (int p = 0; p < (int)PS.size(); p++)
					{
						our_model.cur_point_index = p;
						our_model.multiple_K_values[our_model.cur_data_index][our_model.cur_tau_index] += point_edge_count(our_model, e_index, node_u, node_v);
					}
				}

				//NS method
				if (our_model.method == 4)
					our_model.multiple_K_values[our_model.cur_data_index][our_model.cur_tau_index] += edge_edge_count(our_model, wide_e_index, e_index);
			}
			else
			{
				//SPS and CA methods
				if (our_model.method == 2 || our_model.method == 3)
				{
					for (int p = 0; p < (int)PS.size(); p++)
						our_model.multiple_K_values[our_model.cur_data_index][our_model.cur_tau_index] += point_edge_count_same_edge(our_model, p, e_index);
				}

				//NS method
				if (our_model.method == 4)
					our_model.multiple_K_values[our_model.cur_data_index][our_model.cur_tau_index] += edge_edge_count_same_edge(our_model, e_index);
			}
		}

		clear_double_scan_edgeList(our_model.access_edge_list_first, our_model.detect_DoubleScan_edge_list);
		clear_Dijkstra(edge_set, our_model.sp_node_vec_first, our_model.access_edge_list_first, our_model.is_scan_edge_list_first);
		clear_double_scan_edgeList(our_model.access_edge_list_second, our_model.detect_DoubleScan_edge_list);
		clear_Dijkstra(edge_set, our_model.sp_node_vec_second, our_model.access_edge_list_second, our_model.is_scan_edge_list_second);
	}
}

void call_ASPS(model& our_model)
{
	int e_index;
	int node_u, node_v;
	Edge*edge_set = our_model.edge_set;
	//int edges_totalNum;
	int num_edges_two_sets;
	Dijkstra_preprocess(our_model);

	for (int wide_e_index = 0; wide_e_index < our_model.num_edges; wide_e_index++)
	{
		our_model.cur_edge_index = wide_e_index;
		our_model.cur_tau_index = our_model.D - 1;

		our_model.is_first = true;
		Dijkstra(our_model, our_model.sp_node_vec_first, our_model.access_edge_list_first, our_model.is_scan_edge_list_first);
		our_model.is_first = false;
		Dijkstra(our_model, our_model.sp_node_vec_second, our_model.access_edge_list_second, our_model.is_scan_edge_list_second);
		num_edges_two_sets = (int)our_model.access_edge_list_first.size() + (int)our_model.access_edge_list_second.size();

		for (int e = 0; e < num_edges_two_sets; e++)
		{
			if (e < (int)our_model.access_edge_list_first.size())
			{
				e_index = our_model.access_edge_list_first[e];
				our_model.detect_DoubleScan_edge_list[e_index] = true;
			}
			else
			{
				e_index = our_model.access_edge_list_second[e - our_model.access_edge_list_first.size()];
				if (our_model.detect_DoubleScan_edge_list[e_index] == true)
					continue;
				else
					our_model.detect_DoubleScan_edge_list[e_index] = true;
			}

			node_u = edge_set[e_index].n1;
			node_v = edge_set[e_index].n2;

			for (int l = 0; l < our_model.L; l++)
			{
				our_model.cur_data_index = l;
				vector<Point>& PS = edge_set[wide_e_index].PS_vec[l];
				
				if (PS.size() == 0)
					continue;

				for (int t = our_model.D - 1; t >= 0; t--)
				{
					our_model.cur_tau_index = t;

					if (wide_e_index != e_index)
					{
						if (our_model.sp_node_vec_first[node_u].cur_sp_value > our_model.tau_vec[t] &&
							our_model.sp_node_vec_first[node_v].cur_sp_value > our_model.tau_vec[t] &&
							our_model.sp_node_vec_second[node_u].cur_sp_value > our_model.tau_vec[t] &&
							our_model.sp_node_vec_second[node_v].cur_sp_value > our_model.tau_vec[t])
							break;

						if (our_model.method == 5) //CA^{(ASPS)} method
						{
							for (int p = 0; p < (int)PS.size(); p++)
							{
								our_model.cur_point_index = p;
								our_model.multiple_K_values[l][t] += point_edge_count(our_model, e_index, node_u, node_v);
							}
						}

						if (our_model.method == 6) //NS^{(ASPS)} method
							our_model.multiple_K_values[l][t] += edge_edge_count(our_model, wide_e_index, e_index);
					}
					else
					{
						//CA^{(ASPS)} method
						if (our_model.method == 5)
						{
							for (int p = 0; p < (int)PS.size(); p++)
								our_model.multiple_K_values[l][t] += point_edge_count_same_edge(our_model, p, e_index);
						}

						//NS method
						if (our_model.method == 6)
							our_model.multiple_K_values[l][t] += edge_edge_count_same_edge(our_model, e_index);
					}

				}
			}

		}

		clear_double_scan_edgeList(our_model.access_edge_list_first, our_model.detect_DoubleScan_edge_list);
		clear_Dijkstra(edge_set, our_model.sp_node_vec_first, our_model.access_edge_list_first, our_model.is_scan_edge_list_first);
		clear_double_scan_edgeList(our_model.access_edge_list_second, our_model.detect_DoubleScan_edge_list);
		clear_Dijkstra(edge_set, our_model.sp_node_vec_second, our_model.access_edge_list_second, our_model.is_scan_edge_list_second);
	}
}

void compute_K_function(model& our_model)
{
	double run_time;

	auto start_s = chrono::high_resolution_clock::now();

	//Step 1: Compute shortest path distance
	//Step 2: Compute C_P^{(wide_e,e)}
	if (our_model.method >= 1 && our_model.method <= 4)
	{
		for (int l = 0; l < our_model.L; l++)
		{
			for (int t = 0; t < our_model.D; t++)
			{
				our_model.cur_data_index = l;
				our_model.cur_tau_index = t;

				if (our_model.method == 1)
					call_RQS(our_model);
				if (our_model.method == 2) //SPS
					call_SPS(our_model);
				if (our_model.method == 3) //CA (Use the SPS method)
					call_SPS(our_model);
				if (our_model.method == 4) //NS (Use the SPS method)
					call_SPS(our_model);
			}
		}
	}

	if (our_model.method >= 5 && our_model.method <= 6)
		call_ASPS(our_model);

	auto end_s = chrono::high_resolution_clock::now();

	run_time = (chrono::duration_cast<chrono::nanoseconds>(end_s - start_s).count()) / 1000000000.0;
	std::cout << "method " << our_model.method << ":" << run_time << endl;
}