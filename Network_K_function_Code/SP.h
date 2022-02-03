#pragma once
#ifndef SP_H
#define SP_H

#include "Network.h"

typedef priority_queue<sp_node, vector<sp_node>, comparePriority> PQ;

void Dijkstra_preprocess(model& our_model);
void clear_Dijkstra(Edge*edge_set, vector<sp_node>& sp_node_vec, vector<int>& access_edge_list, bool*is_scan_edge_list);
void clear_double_scan_edgeList(vector<int>& access_edge_list, bool*detect_DoubleScan_edge_list);
void init_Dijkstra(model& our_model, PQ& pq);
void Dijkstra(model& our_model, vector<sp_node>& sp_node_vec, vector<int>& access_edge_list, bool*is_scan_edge_list);

#endif