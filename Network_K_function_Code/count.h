#pragma once
#ifndef COUNT_H
#define COUNT_H

#include "Network.h"

double point_edge_count_same_edge(model& our_model, int p_index, int e_index);
double point_edge_count(model& our_model, int e_index, int node_u, int node_v);

//preprocessing step for the CA method
//void CA_preprocess(model& our_model);
int edge_binary_search_from_n2(vector<Point>& PS, double dist_value, int left, int right);
int edge_binary_search_from_n1(vector<Point>& PS, double dist_value, int left, int right);


double edge_edge_count(model& our_model, int wide_e_index, int e_index);
double edge_edge_count_same_edge(model& our_model, int e_index);

#endif