#Compile our code
g++ -c Network.cpp -w -o Network.o
g++ -c SP.cpp -w -o SP.o
g++ -c count.cpp -w -o count.o
g++ -c K_function.cpp -w -o K_function.o
g++ main.cpp -O3 -o main Network.o SP.o count.o K_function.o

#Parameters in our code
#our_model.network_fileName = argv[1]; //Input the dataset name
#our_model.out_fileName = argv[2]; //Output the result file (for plotting the network K-function plot)
#our_model.method = atoi(argv[3]); //Select the method (method = 1: RQS, method = 2: SPS, method = 3: CA, method = 4: NS)
#our_model.L = atoi(argv[4]); //Number of randomly generated datasets
#our_model.D = atoi(argv[5]); //Number of distance thresholds
#our_model.start_tau = atof(argv[6]); //The initial threshold
#our_model.incr_tau = atof(argv[7]); //The incremental value

#In this code, we adopt D distance thresholds, which are: 
#(1) our_model.start_tau 
#(2) our_model.start_tau + our_model.incr_tau
# :
# :
#(D) our_model.start_tau + (D-1) * our_model.incr_tau

#Example for calling all methods using the Chicago dataset in the "dataset" directory with L = 4, D = 5, start_tau = 100, and incr_tau = 100:
L=5
D=4
start_tau=100
incr_tau=100
dir="./dataset/"
dataset="Chicago"

method=1 #RQS
result_file="Chicago_M"$method
./main $dir$dataset $result_file $method $L $D $start_tau $incr_tau

method=2 #SPS
result_file="Chicago_M"$method
./main $dir$dataset $result_file $method $L $D $start_tau $incr_tau

method=3 #CA
result_file="Chicago_M"$method
./main $dir$dataset $result_file $method $L $D $start_tau $incr_tau

method=4 #NS
result_file="Chicago_M"$method
./main $dir$dataset $result_file $method $L $D $start_tau $incr_tau

method=5 #CA^{(ASPS)}
result_file="Chicago_M"$method
./main $dir$dataset $result_file $method $L $D $start_tau $incr_tau

method=6 #NS^{(ASPS)}
result_file="Chicago_M"$method
./main $dir$dataset $result_file $method $L $D $start_tau $incr_tau