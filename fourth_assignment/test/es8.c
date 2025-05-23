void prova(){
    int a = 100,N=5,i=0;
    int array1[20],array2[20];
    if(N>0){
        i=0;
        do{
            array1[i]=i;
            i++;
        }while(i<N);
    }
    if(N>0){
        i=0;
        do{
            array2[i]=array1[i]+1;
            i++;
        }while(i<N);
    }

}