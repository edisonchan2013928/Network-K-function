# Network-K-function

The folders "Network_K_function_Code" and "Network_K_function_plot_Code" are used to test the efficiency of the algorithms in our paper "Fast Network K-function-based Spatial Analysis" (which is accepted in VLDB 2022).

In the folder "Network_K_function_Code", the code can reproduce the results in Section 4.2 (i.e., Experiments for Computing a Single Network K-function). The "call_net_K.sh" file shows how to compile and run our code (with examples) to compute network K-functions.

In the folder "Network_K_function_plot_Code", the code can reproduce the results in Section 4.3 (i.e., Experiments for Generating a Network K-function Plot) and Section 4.4 (i.e., Case Study: Understanding the Cluster Properties for Crime Events in Chicago). The "call_net_K_plot.sh" file shows how to compile and run our code (with examples) to generate network K-function plots.

Furthermore, please preprocess the dataset such that it follows our file structure ("file_structure.txt") in order to successfully call our code. We also attach the small dataset, called "Testing_network", which is the correct example of our file structure. We have also attached the python code in the folder "extract_road_network" to extract road networks in the OpenStreetMap.
