#include "K_function.h"

int main(int argc, char**argv)
{
	model our_model;
	init_paremeters(argc, argv, our_model);
	load_Network(our_model);
	compute_K_function(our_model);
	output_result(our_model);
}