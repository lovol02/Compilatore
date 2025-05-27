void prova(){
    int a = 100;
    int array1[20],array2[20];
    for(int i=19;i>=0;i--){
        array1[i]=i;
    }
    for(int i=19;i>=0;i--){
        array2[i]=array1[i-1]+1;
    }
}