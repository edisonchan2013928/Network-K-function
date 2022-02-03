#include "SP.h"

inline void clearHeap(PQ& pq)
{
	int heapSize = (int)pq.size();
	for (int h = 0; h < heapSize; h++)
		pq.pop();
}

void Dijkstra_preprocess(model& our_model)
{
	sp_node tempNode;
	our_model.is_scan_edge_list_first = new bool[our_model.num_edges];
	our_model.is_scan_edge_list_second = new bool[our_model.num_edges];
	our_model.detect_DoubleScan_edge_list = new bool[our_model.num_edges]; //Used in SPS

	for (int n = 0; n < our_model.num_nodes; n++)
	{
		tempNode.node_index = n;
		tempNode.cur_sp_value = inf;
		tempNode.is_opt = false;

		our_model.sp_node_vec_first.push_back(tempNode);
		our_model.sp_node_vec_second.push_back(tempNode);
	}

	for (int e = 0; e < our_model.num_edges; e++)
	{
		our_model.is_scan_edge_list_first[e] = false;
		our_model.is_scan_edge_list_second[e] = false;
		our_model.detect_DoubleScan_edge_list[e] = false; //Used in SPS
	}
}

void clear_Dijkstra(Edge*edge_set, vector<sp_node>& sp_node_vec, vector<int>& access_edge_list, bool*is_scan_edge_list)
{
	int list_size = access_edge_list.size();
	int edge_index;

	for (int e = 0; e < list_size; e++)
	{
		edge_index = access_edge_list[e];
		sp_node_vec[edge_set[edge_index].n1].cur_sp_value = inf;
		sp_node_vec[edge_set[edge_index].n1].is_opt = false;
		sp_node_vec[edge_set[edge_index].n2].cur_sp_value = inf;
		sp_node_vec[edge_set[edge_index].n2].is_opt = false;
		is_scan_edge_list[edge_index] = false;
	}

	access_edge_list.clear();
}

void clear_double_scan_edgeList(vector<int>& access_edge_list, bool*detect_DoubleScan_edge_list)
{
	int list_size = access_edge_list.size();
	int edge_index;
	for (int e = 0; e < list_size; e++)
	{
		edge_index = access_edge_list[e];
		detect_DoubleScan_edge_list[edge_index] = false;
	}
}

void init_Dijkstra(model& our_model, PQ& pq)
{
	int cur_edge_index = our_model.cur_edge_index;
	int n1, n2; //refer to node a and node b

	if (our_model.method == 1) //RQS method
	{
		int cur_point_index = our_model.cur_point_index; //Used in RQS
		vector<Point>& PS = our_model.edge_set[cur_edge_index].PS;
		n1 = our_model.edge_set[cur_edge_index].n1;
		n2 = our_model.edge_set[cur_edge_index].n2;
		our_model.access_edge_list_first.push_back(cur_edge_index);
		our_model.is_scan_edge_list_first[cur_edge_index] = true;
		our_model.sp_node_vec_first[n1].cur_sp_value = PS[cur_point_index].dist_n1;
		our_model.sp_node_vec_first[n2].cur_sp_value = PS[cur_point_index].dist_n2;

		pq.push(our_model.sp_node_vec_first[n1]);
		pq.push(our_model.sp_node_vec_first[n2]);
	}
	if (our_model.method == 2 || our_model.method == 3 || our_model.method == 4) //SPS method or CA method
	{
		if (our_model.is_first == true)
		{
			n1 = our_model.edge_set[cur_edge_index].n1;
			our_model.access_edge_list_first.push_back(cur_edge_index);
			our_model.is_scan_edge_list_first[cur_edge_index] = true;
			our_model.sp_node_vec_first[n1].cur_sp_value = 0;

			pq.push(our_model.sp_node_vec_first[n1]);
		}
		else
		{
			n2 = our_model.edge_set[cur_edge_index].n2;
			our_model.access_edge_list_second.push_back(cur_edge_index);
			our_model.is_scan_edge_list_second[cur_edge_index] = true;
			our_model.sp_node_vec_second[n2].cur_sp_value = 0;

			pq.push(our_model.sp_node_vec_second[n2]);
		}
	}
}

void Dijkstra(model& our_model, vector<sp_node>& sp_node_vec, vector<int>& access_edge_list, bool*is_scan_edge_list)
{
	static PQ pq;
	int node_index, node_index_neighbor;
	int edge_index;
	double length;
	sp_node SP_node;

	init_Dijkstra(our_model, pq);

	while (pq.size() > 0)
	{
		SP_node = pq.top();
		pq.pop();

		//discard outdated node
		if (sp_node_vec[SP_node.node_index].cur_sp_value < SP_node.cur_sp_value - eps)
			continue;

		//discard node with value more than the the threshold tau
		if (sp_node_vec[SP_node.node_index].cur_sp_value > our_model.tau)
			continue;

		node_index = SP_node.node_index;
		sp_node_vec[node_index].is_opt = true;

		for (int e = 0; e < (int)our_model.Network[node_index].size(); e++)
		{
			//Relax (code here)
			edge_index = our_model.Network[node_index][e];
			if (is_scan_edge_list[edge_index] == false)
			{
				is_scan_edge_list[edge_index] = true;
				access_edge_list.push_back(edge_index);
			}

			if (node_index != our_model.edge_set[edge_index].n1)
				node_index_neighbor = our_model.edge_set[edge_index].n1;
			else
				node_index_neighbor = our_model.edge_set[edge_index].n2;

			if (sp_node_vec[node_index_neighbor].is_opt == true)
				continue;

			length = our_model.edge_set[edge_index].length;

			if (sp_node_vec[node_index_neighbor].cur_sp_value > sp_node_vec[node_index].cur_sp_value + length)
			{
				sp_node_vec[node_index_neighbor].cur_sp_value = sp_node_vec[node_index].cur_sp_value + length;
				pq.push(sp_node_vec[node_index_neighbor]);
			}
		}
	}
}