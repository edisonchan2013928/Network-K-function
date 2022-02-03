#pragma once
#ifndef NETWORK_H
#define NETWORK_H

#include "Library.h"

const double inf = 999999999999999;
const double eps = 0.00000000001;

struct Point
{
	double dist_n1;
	double dist_n2;
	int edge_index;
};

struct Edge
{
	int n1;
	int n2;
	double length;
	vector<Point> PS;
};

struct sp_node
{
	int node_index;
	double cur_sp_value;
	bool is_opt;
};

//This is the minimum heap for Dijkstra's algorithm
struct comparePriority
{
	bool operator()(sp_node& n1, sp_node& n2)
	{
		return n1.cur_sp_value > n2.cur_sp_value;
	}
};

struct model
{
	int num_nodes;
	int num_edges;
	char*network_fileName; //file Name
	int method; //method
	double tau; //tau
	double K_value; //K-function value

	vector<vector<int>> Network; //road network
	Edge*edge_set; //edge set

	//Used in the Shortest path algorithm
	vector<sp_node> sp_node_vec_first;
	vector<sp_node> sp_node_vec_second;
	vector<int> access_edge_list_first; //store the edges that are within the distance tau from the data point/node 
	vector<int> access_edge_list_second; //store the edges that are within the distance tau from the node
	bool*is_scan_edge_list_first; //avoid to push the same edge in access_edge_list_first
	bool*is_scan_edge_list_second; //avoid to push the same edge in access_edge_list_second

	int cur_edge_index;
	int cur_node_index;
	int cur_point_index;
	
	//Used in SPS
	//********************************//
	bool is_first;
	bool*detect_DoubleScan_edge_list;
	//********************************//
};

void init_paremeters(int argc, char**argv, model& our_model);
void load_Network(model& our_model);
void output_result(model& our_model);

#endif