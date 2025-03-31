//This is first assignment for course "compilatore"
//In the preparation phase we need to use command cmake to make our optimizer available.
//Then every time have the modification on file LocalOpts.cpp, we have to use command make to renew it.

• export LLVM_DIR=<installation/dir/of/llvm/19>/bin
• mkdir build
• cd build
• cmake -DLT_LLVM_INSTALL_DIR=$LLVM_DIR <source/dir/test/pass>/
• make

//In execution phase
//Generate the LLVM code:

clang -Xclang -disable-O0-optnone -S -emit-llvm -O0 -c InputFileName.c -o OutPutFilename1.ll

//mem2reg to remove the operation like alloca,load,store from our llvm file, to be bettter for optimization.

opt -p mem2reg OutPutFilename1.ll -S -o OutPutFilename1.ll

//Start optimization
//Optimize the code in all three way(Algeraic Identity, Strength reduction, Multi instruction optimization) and transform it to Bytecode format.

opt -load-pass-plugin ../build/libLocalOpts.so -passes=local-opts  OutPutFilename1.ll  -o OutPutFilename2.bc

//Then retransform them into llvm file

lvm-dis prova.optimized.bc -o prova.optimized.ll


<<<<<<< HEAD
//Or directly use command 

opt -load-pass-plugin ../build/libLocalOpts.so -passes=local-opts  OutPutFilename1.ll -S -o OutPutFilename2.ll

//Optimize the code only with "Algebraic identity", in this case support add,sub,mul and div.
// x+0=0+x -> x, x-0 ->x, x*1=1*x -> x, x/1 ->x 

opt -load-pass-plugin ../build/libLocalOpts.so -passes=Algebraic-Identity provam2r.ll -S -o provaAlgebraicIdentityOpt.ll

//Optimize the code only with "Strength Reduction",in this case support mul in case the factor is power of 2( or near to the exponent of 2), and for division it support only if the dividend is power of 2.
// x*15 -> (x<<4)-1, x*17 -> (x<<4)+1, x/8 ->(x>>3)

opt -load-pass-plugin ../build/libLocalOpts.so -passes=Strength-Reduction provam2r.ll -S -o provaStrengthReductionOpt.ll

//Optimize the code in multi instruction in (add, sub,division,multiplication) case.
// a=x+1 b=a-1 -> a=x+1 b=x, a=x*4 b=a/4 -> b=x, a=x/3 b=a*3 -> b=x.

opt -load-pass-plugin ../build/libLocalOpts.so -passes=MultiInst-Opt  provam2r.ll -S -o provaMultiInstOpt.ll




=======
>>>>>>> 16c1a24a7b3f71f2e56bae49f81d7ccf565363d7
