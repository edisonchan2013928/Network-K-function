#Compile our code
g++ -c Network.cpp -w -o Network.o
g++ -c SP.cpp -w -o SP.o
g++ -c count.cpp -w -o count.o
g++ -c K_function.cpp -w -o K_function.o
g++ main.cpp -O3 -o main Network.o SP.o count.o K_function.o

#Parameters in our code
#our_model.network_fileName = argv[1]; //Input the dataset name
#our_model.method = atoi(argv[2]); //Select the method (method = 1: RQS, method = 2: SPS, method = 3: CA, method = 4: NS)
#our_model.tau = atoi(argv[3]); //Set the threshold value tau

#Example for calling all methods using the Chicago dataset in the "dataset" directory with tau = 1000m:
dir="./dataset/"
dataset="Chicago"
tau=1000

method=1 #RQS
./main $dir$dataset $method $tau

method=2 #SPS
./main $dir$dataset $method $tau

method=3 #CA
./main $dir$dataset $method $tau

method=4 #NS
./main $dir$dataset $method $tau
