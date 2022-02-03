#include "count.h"

double point_edge_count_same_edge(model& our_model, int p_index, int e_index)
{
	double edge_K_value = 0;
	double delta;
	vector<Point>& PS = our_model.edge_set[e_index].PS;

	//RQS and SPS methods
	if (our_model.method == 1 || our_model.method == 2)
	{
		for (int p = 0; p < (int)PS.size(); p++)
		{
			if (p_index == p)
				continue;

			delta = fabs(PS[p_index].dist_n1 - PS[p].dist_n1);
			if (delta <= our_model.tau)
				edge_K_value++;
		}
	}
	
	//CA method
	if (our_model.method == 3)
	{
		if (PS.size() == 0)
			return 0;

		double dist_left = PS[p_index].dist_n1 - our_model.tau;
		double dist_right = PS[p_index].dist_n2 - our_model.tau;

		int left_index = edge_binary_search_from_n1(PS, dist_left, 0, PS.size() - 1);
		int right_index = edge_binary_search_from_n2(PS, dist_right, 0, PS.size() - 1);

		if (left_index != -1 && right_index != -1)
			return right_index - left_index;
		if (left_index == -1 && right_index != -1)
			return right_index;
		if (left_index != -1 && right_index == -1)
			return PS.size() - left_index - 1;
		if (left_index == -1 && right_index == -1)
			return PS.size() - 1;
	}

	return edge_K_value;
}

double point_edge_count(model& our_model, int e_index, int node_u, int node_v)
{
	double delta;
	double K_value = 0;
	vector<Point>& PS = our_model.edge_set[e_index].PS;

	if (our_model.method == 1) //RQS
	{
		for (int p = 0; p < (int)PS.size(); p++)
		{
			delta = min(our_model.sp_node_vec_first[node_u].cur_sp_value + PS[p].dist_n1,
				our_model.sp_node_vec_first[node_v].cur_sp_value + PS[p].dist_n2);
			if (delta <= our_model.tau)
				K_value++;
		}
	}
	
	if (our_model.method == 2) //SPS
	{
		double delta_a;
		double delta_b;
		vector<Point>& PS_wide_e = our_model.edge_set[our_model.cur_edge_index].PS;
		int p_wide_e = our_model.cur_point_index;

		for (int p = 0; p < (int)PS.size(); p++)
		{
			delta_a = min(PS_wide_e[p_wide_e].dist_n1 + our_model.sp_node_vec_first[node_u].cur_sp_value + PS[p].dist_n1,
				PS_wide_e[p_wide_e].dist_n1 + our_model.sp_node_vec_first[node_v].cur_sp_value + PS[p].dist_n2);
			delta_b = min(PS_wide_e[p_wide_e].dist_n2 + our_model.sp_node_vec_second[node_u].cur_sp_value + PS[p].dist_n1,
				PS_wide_e[p_wide_e].dist_n2 + our_model.sp_node_vec_second[node_v].cur_sp_value + PS[p].dist_n2);
			delta = min(delta_a, delta_b);

			if (delta <= our_model.tau)
				K_value++;
		}
	}
	
	if (our_model.method == 3) //CA
	{
		vector<Point>& PS_wide_e = our_model.edge_set[our_model.cur_edge_index].PS;
		int p_wide_e = our_model.cur_point_index;
		double dist_p_i_u; //Equation 13
		double dist_p_i_v; //Equation 14
		int left_pos, right_pos;

		if ((int)PS.size() == 0)
			return 0;

		//code here
		dist_p_i_u = min(PS_wide_e[p_wide_e].dist_n1 + our_model.sp_node_vec_first[node_u].cur_sp_value,
			PS_wide_e[p_wide_e].dist_n2 + our_model.sp_node_vec_second[node_u].cur_sp_value);
		dist_p_i_v = min(PS_wide_e[p_wide_e].dist_n1 + our_model.sp_node_vec_first[node_v].cur_sp_value,
			PS_wide_e[p_wide_e].dist_n2 + our_model.sp_node_vec_second[node_v].cur_sp_value);

		if (dist_p_i_u > our_model.tau && dist_p_i_v > our_model.tau) //Case 1
			return 0;
		if (dist_p_i_u <= our_model.tau && dist_p_i_v > our_model.tau) //Case 2
		{
			left_pos = edge_binary_search_from_n1(PS, our_model.tau - dist_p_i_u, 0, PS.size() - 1);
			if (left_pos == -1)
				return 0;
			else
				return left_pos + 1;
		}
		if (dist_p_i_u > our_model.tau && dist_p_i_v <= our_model.tau) //Case 3
		{
			right_pos = edge_binary_search_from_n2(PS, our_model.tau - dist_p_i_v, 0, PS.size() - 1);
			if (right_pos == -1)
				return 0;
			else
				return PS.size() - right_pos;
		}
		if (dist_p_i_u <= our_model.tau && dist_p_i_v <= our_model.tau) //Case 4
		{
			left_pos = edge_binary_search_from_n1(PS, our_model.tau - dist_p_i_u, 0, PS.size() - 1);
			right_pos = edge_binary_search_from_n2(PS, our_model.tau - dist_p_i_v, 0, PS.size() - 1);

			if (left_pos == -1 && right_pos == -1)
				return 0;
			if (left_pos != -1 && right_pos == -1)
				return left_pos + 1;
			if (left_pos == -1 && right_pos != -1)
				return PS.size() - right_pos;
			if (left_pos != -1 && right_pos != -1)
				return min(PS.size() - right_pos + left_pos + 1, PS.size());
		}
	}

	return K_value;
}

//CA method
int edge_binary_search_from_n1(vector<Point>& PS, double dist_value, int left, int right)
{
	int middle;

	/*if (right - left == 1)
	{
		if (dist_value > PS[right].dist_n1)
			return right;
		if (dist_value > PS[left].dist_n1)
			return left;
		return -1;
	}*/

	if (left == right)
	{
		if (left == 0 && dist_value < PS[left].dist_n1)
			return -1;

		return left;
	}
		
	middle = (int)floor((left + right) / 2.0);

	if (dist_value < PS[middle].dist_n1)
		return edge_binary_search_from_n1(PS, dist_value, left, middle);
	if (dist_value > PS[middle].dist_n1)
		return edge_binary_search_from_n1(PS, dist_value, middle + 1, right);
	if (dist_value == PS[middle].dist_n1)
		return middle;

	cout << "Error" << endl; //Error
	exit(0);
}

int edge_binary_search_from_n2(vector<Point>& PS, double dist_value, int left, int right)
{
	int middle;

	/*if (right - left == 1)
	{
		if (dist_value > PS[left].dist_n2)
			return left;
		if (dist_value > PS[right].dist_n2)
			return right;
		return -1;
	}*/

	if (left == right)
	{
		if (right == PS.size() - 1 && dist_value < PS[right].dist_n2)
			return -1;

		return right;
	}

	middle = (int)ceil((left + right) / 2.0);
	if (dist_value < PS[middle].dist_n2)
		return edge_binary_search_from_n2(PS, dist_value, middle, right);
	if (dist_value > PS[middle].dist_n2)
		return edge_binary_search_from_n2(PS, dist_value, left, middle - 1);
	if (dist_value == PS[middle].dist_n2)
		return middle;

	cout << "Error" << endl; //Error
	exit(0);
}

/*void CA_preprocess(model& our_model)
{
	int counter;
	//augment the count values for each edge
	for (int e_index = 0; e_index < our_model.num_edges; e_index++)
	{
		counter = 0;
		vector<Point>& PS = our_model.edge_set[e_index].PS;

		for (int p = 0; p < (int)PS.size(); p++)
		{
			PS[p].count_n1 = counter;
			PS[p].count_n2 = (double)PS.size() - counter;
		}
	}
}*/


double edge_edge_count(model& our_model, int wide_e_index, int e_index)
{
	vector<Point>& PS_wide_e = our_model.edge_set[wide_e_index].PS;
	vector<Point>& PS = our_model.edge_set[e_index].PS;
	//vector<int> ell_au; vector<int> ell_av;
	//vector<int> ell_bu; vector<int> ell_bv;
	//double tau_au, tau_av, tau_bu, tau_bv;
	int pos_au, pos_av, pos_bu, pos_bv;
	int node_u, node_v;
	double cur_tau_au, cur_tau_av, cur_tau_bu, cur_tau_bv;

	if (PS_wide_e.size() == 0 || PS.size() == 0)
		return 0;

	node_u = our_model.edge_set[e_index].n1;
	node_v = our_model.edge_set[e_index].n2;

	//initialization of positions
	//****************************************************************************//
	bool au_end = false;
	bool bu_end = false;
	cur_tau_au = our_model.tau - our_model.sp_node_vec_first[node_u].cur_sp_value;
	cur_tau_bu = our_model.tau - our_model.sp_node_vec_second[node_u].cur_sp_value - our_model.edge_set[wide_e_index].length;
	pos_au = -1; pos_bu = -1;
	for (int ii = 0; ii < (int)PS.size(); ii++)
	{
		if (au_end == true && bu_end == true)
			break;

		if (au_end == false)
		{
			if (PS[ii].dist_n1 < cur_tau_au)
				pos_au++;
			else
				au_end = true;
		}
		
		if (bu_end == false)
		{
			if (PS[ii].dist_n1 < cur_tau_bu)
				pos_bu++;
			else
				bu_end = true;
		}
	}

	bool av_end = false;
	bool bv_end = false;
	cur_tau_av = our_model.tau - our_model.sp_node_vec_first[node_v].cur_sp_value;
	cur_tau_bv = our_model.tau - our_model.sp_node_vec_second[node_v].cur_sp_value - our_model.edge_set[wide_e_index].length;
	pos_av = -1; pos_bv = -1;
	for (int ii = PS.size() - 1; ii >= 0; ii--)
	{
		if (av_end == true && bv_end == true)
			break;

		if (av_end == false)
		{
			if (PS[ii].dist_n2 < cur_tau_av)
				pos_av++;
			else
				av_end = true;
		}
		
		if (bv_end == false)
		{
			if (PS[ii].dist_n2 < cur_tau_bv)
				pos_bv++;
			else
				bv_end = true;
		}
	}
	//****************************************************************************//

	//update
	double delta;
	double K_value = 0;
	for (int i = 0; i < (int)PS_wide_e.size(); i++)
	{
		if (i == 0)
			delta = PS_wide_e[i].dist_n1;
		else
			delta = PS_wide_e[i].dist_n1 - PS_wide_e[i - 1].dist_n1;

		cur_tau_au -= delta; cur_tau_av -= delta;
		cur_tau_bu += delta; cur_tau_bv += delta;

		for (int ii = pos_au; ii >= 0; ii--)
		{
			if (PS[ii].dist_n1 <= cur_tau_au)
				break;
			else
				pos_au--;
		}

		for (int ii = pos_av; ii >= 0; ii--)
		{
			if (PS[PS.size() - 1 - ii].dist_n2 <= cur_tau_av)
				break;
			else
				pos_av--;
		}

		for (int ii = pos_bu; ii < (int)PS.size() - 1; ii++)
		{
			if (PS[ii + 1].dist_n1 > cur_tau_bu)
				break;
			else
				pos_bu++;
		}

		for (int ii = pos_bv; ii < (int)PS.size() - 1; ii++)
		{
			if (PS[PS.size() - 2 - ii].dist_n2 > cur_tau_bv)
				break;
			else
				pos_bv++;
		}

		K_value += min((max(pos_au, pos_bu) + 1) + (max(pos_av, pos_bv) + 1), (int)PS.size());
	}

	return K_value;
}

double edge_edge_count_same_edge(model& our_model, int e_index)
{
	vector<Point>& PS = our_model.edge_set[e_index].PS;
	double delta;
	double K_value = 0;
	int left, right;

	if (PS.size() == 0)
		return 0;

	for (int i = 0; i < (int)PS.size(); i++)
	{
		if (i == 0) //initialization
		{
			left = -1;
			right = -1;
			for (int ii = 0; ii < (int)PS.size(); ii++)
			{
				if (PS[ii].dist_n1 - PS[i].dist_n1 <= our_model.tau)
					right++;
				else
					break;
			}
			K_value += right;
			continue;
		}

		//Update left and right
		delta = PS[i].dist_n1 - PS[i - 1].dist_n1;
		for (int ii = right + 1; ii < (int)PS.size(); ii++)
		{
			if (PS[ii].dist_n1 - PS[i].dist_n1 > our_model.tau)
				break;
			else
				right++;
		}

		for (int ii = left + 1; ii < (int)PS.size(); ii++)
		{
			if (PS[i].dist_n1 - PS[ii].dist_n1 <= our_model.tau)
				break;
			else
				left++;
		}
		K_value += (right - left - 1);
	}

	return K_value;
}