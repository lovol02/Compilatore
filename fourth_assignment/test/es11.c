void prova(){
    int a = 100,N=5;
    int array1[20],array2[20];
    if(N>0){
        for(int i=N;i>0;i--){
            array1[i]=i;
        }
    }
    if(N>0){
        for(int i=N;i>0;i--){
            array2[i]=array1[i-1];
        }
    }

}