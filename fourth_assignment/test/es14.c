void prova(){
    int a = 100;
    int array1[20],array2[20];
    array1[0]=0;

    for(int i=0;i<20;i++){
        array2[i]=array1[i];
        
    }
    for(int i=0;i<20;i++){
        array1[i+1]=i;
    }
}