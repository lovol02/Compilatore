void prova(){
    int a = 100;
    int array1[20],array2[20];
    array1[19]=19;

    for(int i=19;i>0;i--){
        array2[i]=array1[i];
        
    }
    for(int i=19;i>0;i--){
        array1[i-1]=i;
    }
}