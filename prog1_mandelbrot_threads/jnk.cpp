 int size_of_one = sizeof(args[0].output)/sizeof(args[0].output[0]);
    int size_of_two = sizeof(args[1].output)/sizeof(args[1].output[0]);


    int *data = (int*) malloc(sizeof(args[0].output) + sizeof(args[1].output));
    for(int i = 0; i < size_of_one; i++){
        data[i] = args[0].output[i];
    }

    int j = size_of_one -1;
    for(int i = 0; i < size_of_two; i++){
        data[j] = args[1].output[i];
        j++;
    }

    memcpy(output, data, sizeof(data));


    for(int i = 0; i < sizeof(output)/sizeof(output[0]); i++){
       std::cout << output[i] << std::endl;
    }