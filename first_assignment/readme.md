//This is first assignment for course "compilatore"
//In the preparation phase we need to use command cmake to make our optimizer available.
//Then every time have the modification on file LocalOpts.cpp, we have to use command make to renew it.
• export LLVM_DIR=<installation/dir/of/llvm/19>/bin
• mkdir build
• cd build
• cmake -DLT_LLVM_INSTALL_DIR=$LLVM_DIR <source/dir/test/pass>/
• make

//In execution phase
//Generate the LLVM code
clang -Xclang -disable-O0-optnone -S -emit-llvm -O0 -c InputFileName.c -o OutPutFilename1.ll
//mem2reg to remove the operation like alloca,load,store from our llvm file, to be bettter for optimization.
opt -p mem2reg OutPutFilename1.ll -o OutPutFilename1.ll
//Start optimization
//Optimize the code and transform it to Bytecode format
opt -load-pass-plugin ../build/libLocalOpts.so -passes=local-opts  OutPutFilename1.ll  -o OutPutFilename2.bc

//Then retransform them into llvm file
lvm-dis prova.optimized.bc -o prova.optimized.ll


prova
