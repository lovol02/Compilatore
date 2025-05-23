# Fourth assignment
This is the Fourth assignment of compilatore  
This project occupied the loop fusion, the pass will detect the fusionability of two loop.  
The command to use:  
To create the build directory  
<pre>
• export LLVM_DIR=<installation/dir/of/llvm/19>
• mkdir build
• cd build
• cmake -DLT_LLVM_INSTALL_DIR=$LLVM_DIR <source/dir/test/pass>/
• make
</pre>  
To generate the llvm code, enter the <b>text</b> folder, and run following command:  
<pre>
clang -Xclang -disable-O0-optnone -S -emit-llvm -O0 -c Inputfilename.c -o Output.ll
</pre>  
mem to reg (remove load and store instruction)  
<pre>  
opt -p mem2reg filename.ll -o filename.bc  
llvm-dis filename.bc -o filenamem2r.ll  
</pre>  
To run the pass  
<pre>
opt -load-pass-plugin ../build/libLoopPass.so -passes=LoopFusion-pass  filenamem2r.ll -S -o filenameopt.ll  
</pre>  