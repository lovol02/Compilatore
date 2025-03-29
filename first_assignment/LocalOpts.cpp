//=============================================================================
// FILE:
//    LocalOpts.cpp
//
// DESCRIPTION:
//    Visits all functions in a module and prints their names. Strictly speaking, 
//    this is an analysis pass (i.e. //    the functions are not modified). However, 
//    in order to keep things simple there's no 'print' method here (every analysis 
//    pass should implement it).
//
// USAGE:
//    New PM
//      opt -load-pass-plugin=<path-to>libLocalOpts.so -passes="local-opts" `\`
//        -disable-output <input-llvm-file>
//
//
// License: MIT
//=============================================================================
#include "llvm/IR/LegacyPassManager.h"
#include "llvm/Passes/PassBuilder.h"
#include "llvm/Passes/PassPlugin.h"
#include "llvm/Support/raw_ostream.h"
#include "llvm/IR/IRBuilder.h"
#include "llvm/IR/Function.h"
#include "llvm/IR/Instructions.h"
#include "llvm/IR/Constants.h"
using namespace llvm;


//-----------------------------------------------------------------------------
// LocalOpts implementation
//-----------------------------------------------------------------------------
// No need to expose the internals of the pass to the outside world - keep
// everything in an anonymous namespace.


bool runOnBasicBlock(BasicBlock &B) {

    std::vector<Instruction*> ToErase;
    for (Instruction &I : B ){
      
      //se e' istruzione di mul
      if(I.getOpcode() == Instruction::Mul){
        outs()<<"find a mul\n";
        auto *Iter1 = I.op_begin();
        /*
        %5 = mul nsw i32 %4, 2 has first operator as %4 = add i32 %1, %1, cause %4 is an intermedia result of calculation. 
        This is unsolveable, cause once solved, it means the intermediate calculation are ignored
        
        */
        /*
        for (auto *Iter = I.op_begin(); Iter != I.op_end(); ++Iter) {
          Value *Operand = *Iter; 
          //outs()<<*Operand<<"\n";
          if (Instruction *inst=dyn_cast<Instruction>(Operand)){
              outs()<<"inside the operand"<<*inst<<"\n";
          }
          else if(ConstantInt *C=dyn_cast<ConstantInt>(Operand)){
            if(C->isOne()){
              outs()<<"added an mul operation to remove\n";
              ToErase.push_back(&I);
            }
          }
        }
        */
        Value *Operand = *Iter1;
        Value *Operand1 = *(++Iter1);
        int index=2,flag=1;
        ConstantInt *C;
        if (ConstantInt *Ctemp = dyn_cast<ConstantInt>(Operand1)) {
          index=0;
          C = dyn_cast<ConstantInt>(Operand1);
          //outs()<<*C<<"\n";
        }
        else if (ConstantInt *Ctemp = dyn_cast<ConstantInt>(Operand)) {
          index=1;
          C = dyn_cast<ConstantInt>(Operand);
          //outs()<<*C<<"\n";
        }
        if(index!=2){
            if(C->isOne()){
              outs()<<"added an mul operation to remove\n";
              I.replaceAllUsesWith(I.getOperand(index));
              ToErase.push_back(&I);
            }else if(C->getValue().isPowerOf2()){
              if(C->getValue().exactLogBase2()>0){
                Value *v=ConstantInt::get(I.getOperand(index)->getType(),C->getValue().exactLogBase2()); 
                Instruction *NewInst = BinaryOperator::Create(
                  Instruction::Shl, I.getOperand(index), v);
                NewInst->insertAfter(&I);
                I.replaceAllUsesWith(NewInst);
                ToErase.push_back(&I);
              }

            }else if((C->getValue()+1).isPowerOf2()){
              if( (C->getValue()+1).exactLogBase2()>0){
                Value *v=ConstantInt::get(I.getOperand(index)->getType(),(C->getValue()+1).exactLogBase2()); 
                Instruction *NewInst = BinaryOperator::Create(
                  Instruction::Shl, I.getOperand(index), v);
                Instruction *NewInst1 = BinaryOperator::Create(
                Instruction::Sub, NewInst, I.getOperand(index));
                NewInst->insertAfter(&I);
                NewInst1->insertAfter(NewInst);
                I.replaceAllUsesWith(NewInst1);
                ToErase.push_back(&I);
              }
            }else if((C->getValue()-1).isPowerOf2()){
              if( (C->getValue()-1).exactLogBase2()>0){
                Value *v=ConstantInt::get(I.getOperand(index)->getType(),(C->getValue()-1).exactLogBase2()); 
                Instruction *NewInst = BinaryOperator::Create(
                  Instruction::Shl, I.getOperand(index), v);
                Instruction *NewInst1 = BinaryOperator::Create(
                Instruction::Add, NewInst, I.getOperand(index));
                NewInst->insertAfter(&I);
                NewInst1->insertAfter(NewInst);
                I.replaceAllUsesWith(NewInst1);
                ToErase.push_back(&I);
              }
            }
        }
        /*
        //se il secondo variabile e' costante
        if (ConstantInt *Ctemp = dyn_cast<ConstantInt>(Operand1)) {

          if(C->isOne()){
            outs()<<"added an mul operation to remove\n";
            I->replaceAllUsesWith(I.getOperand(0));
            ToErase.push_back(&I);
          }
          
          if(C->getValue().isPowerOf2()){
            //added this code cause my C->getValue().isPowerOf2() doesn't cover the case =0, but in the reference it should do it
            if(C->getValue().exactLogBase2()>0){
              Value *v=ConstantInt::get(I.getOperand(0)->getType(),C->getValue().exactLogBase2()); 
              Instruction *NewInst = BinaryOperator::Create(
                Instruction::Shl, I.getOperand(0), v);
              NewInst->insertAfter(&I);
              I.replaceAllUsesWith(NewInst);
              ToErase.push_back(&I);
            }

          }else if ((C->getValue()+1).isPowerOf2()){
            outs()<<"This operation is one in less respect the power of 2, so replace it.\n";
            //added this code cause my C->getValue().isPowerOf2() doesn't cover the case =0, but in the reference it should do it
            if( (C->getValue()+1).exactLogBase2()>0){
              Value *v=ConstantInt::get(I.getOperand(0)->getType(),(C->getValue()+1).exactLogBase2()); 
              Instruction *NewInst = BinaryOperator::Create(
                Instruction::Shl, I.getOperand(0), v);
              Instruction *NewInst1 = BinaryOperator::Create(
              Instruction::Sub, NewInst, I.getOperand(0));
              NewInst->insertAfter(&I);
              NewInst1->insertAfter(NewInst);
              I.replaceAllUsesWith(NewInst1);
              ToErase.push_back(&I);
            }
          }
          else if((C->getValue()-1).isPowerOf2()){
            outs()<<"This operation is more in less respect the power of 2, so replace it.\n";
            //added this code cause my C->getValue().isPowerOf2() doesn't cover the case =0, but in the reference it should do it
            if( (C->getValue()-1).exactLogBase2()>0){
              Value *v=ConstantInt::get(I.getOperand(0)->getType(),(C->getValue()-1).exactLogBase2()); 
              Instruction *NewInst = BinaryOperator::Create(
                Instruction::Shl, I.getOperand(0), v);
              Instruction *NewInst1 = BinaryOperator::Create(
              Instruction::Add, NewInst, I.getOperand(0));
              NewInst->insertAfter(&I);
              NewInst1->insertAfter(NewInst);
              I.replaceAllUsesWith(NewInst1);
              ToErase.push_back(&I);
            }
          }
        }
        //se il primo variabile e' costante
        else if (ConstantInt *C = dyn_cast<ConstantInt>(Operand)) {
          index=1;
          if(C->isOne()){
            outs()<<"added an mul operation to remove\n";
            I->replaceAllUsesWith(I.getOperand(1));
            ToErase.push_back(&I);
          }
          if(C->getValue().isPowerOf2()){
            //added this code cause my C->getValue().isPowerOf2() doesn't cover the case =0, but in the reference it should do it
            if(C->getValue().exactLogBase2()>0){
              Value *v=ConstantInt::get(I.getOperand(1)->getType(),C->getValue().exactLogBase2()); 
              Instruction *NewInst = BinaryOperator::Create(
                Instruction::Shl, I.getOperand(1), v);
              NewInst->insertAfter(&I);
              I.replaceAllUsesWith(NewInst);
              ToErase.push_back(&I);
            }

          }else if ((C->getValue()+1).isPowerOf2()){
            outs()<<"This operation is one in less respect the power of 2, so replace it.\n";
            //added this code cause my C->getValue().isPowerOf2() doesn't cover the case =0, but in the reference it should do it
            if( (C->getValue()+1).exactLogBase2()>0){
              Value *v=ConstantInt::get(I.getOperand(1)->getType(),(C->getValue()+1).exactLogBase2()); 
              Instruction *NewInst = BinaryOperator::Create(
                Instruction::Shl, I.getOperand(1), v);
              Instruction *NewInst1 = BinaryOperator::Create(
              Instruction::Sub, NewInst, I.getOperand(1));
              NewInst->insertAfter(&I);
              NewInst1->insertAfter(NewInst);
              I.replaceAllUsesWith(NewInst);
              ToErase.push_back(&I);
            }
          }
          else if((C->getValue()-1).isPowerOf2()){
            outs()<<"This operation is more in less respect the power of 2, so replace it.\n";
            //added this code cause my C->getValue().isPowerOf2() doesn't cover the case =0, but in the reference it should do it
            if( (C->getValue()-1).exactLogBase2()>0){
              Value *v=ConstantInt::get(I.getOperand(1)->getType(),(C->getValue()-1).exactLogBase2()); 
              Instruction *NewInst = BinaryOperator::Create(
                Instruction::Shl, I.getOperand(1), v);
              Instruction *NewInst1 = BinaryOperator::Create(
              Instruction::Add, NewInst, I.getOperand(1));
              NewInst->insertAfter(&I);
              NewInst1->insertAfter(NewInst);
              I.replaceAllUsesWith(NewInst);
              ToErase.push_back(&I);
            }
          }
         }
         */
      }

      //se e' divisione, si fa un LShr solamente se divendente e' esponente di 2
      if(I.getOpcode() == Instruction::SDiv){
        auto *Iter1 = I.op_begin();
        Value *Operand1 = *(++Iter1);
        //se il secondo variabile e' costante
        if (ConstantInt *C = dyn_cast<ConstantInt>(Operand1)) {
          if(C->getValue().isPowerOf2()){
            //added this code cause my C->getValue().isPowerOf2() doesn't cover the case =0, but in the reference it should do it
            if(C->getValue().exactLogBase2()>0){
              Value *v=ConstantInt::get(I.getOperand(0)->getType(),C->getValue().exactLogBase2()); 
              Instruction *NewInst = BinaryOperator::Create(
                Instruction::LShr, I.getOperand(0), v);
              NewInst->insertAfter(&I);
              I.replaceAllUsesWith(NewInst);
              ToErase.push_back(&I);
            }
          }
        }
      }

      //se e' istruzione di add
      if(I.getOpcode() == Instruction::Add){
        outs()<<I<<"\n";
        auto *Iter = I.op_begin();
        Value *Operand = *Iter;
        Value *Operand1 = *(++Iter);
        ConstantInt *C;
        Instruction *Inst;
        int f=0;
        if (ConstantInt *Ctemp =dyn_cast<ConstantInt>(Operand1)){

          if(Instruction *Insttemp = dyn_cast<Instruction>(Operand)){
            f=1;
            Inst = dyn_cast<Instruction>(Operand);
            C=dyn_cast<ConstantInt>(Operand1);
          }
          if(Ctemp->isZero()){
            outs()<<"added an add operation to remove\n";
            //replace con variable
            I.replaceAllUsesWith(I.getOperand(0));
            ToErase.push_back(&I);
            f=0;
          }
          /*
          //controllo se e' addizione di costante, che in teoria non si succede
          if(ConstantInt *Ctemp2 = dyn_cast<ConstantInt>(Operand)){
            if(Ctemp->isZero() && Ctemp2->isZero()){
              ToErase.push_back(&I);
            }
          }
          */
          
        }
        else if (ConstantInt *Ctemp =dyn_cast<ConstantInt>(Operand)){
          if(Instruction *Insttemp = dyn_cast<Instruction>(Operand1)){
            f=2;
            Inst = dyn_cast<Instruction>(Operand1);
            C=dyn_cast<ConstantInt>(Operand);
          }
          if(Ctemp->isZero()){
            outs()<<"added an add operation to remove\n";
            //replace con variable
            I.replaceAllUsesWith(I.getOperand(1));
            ToErase.push_back(&I);
            f=0;
          }
          /*
          //controllo se e' addizione di costante, che in teoria non si succede
          if(ConstantInt *Ctemp2 = dyn_cast<ConstantInt>(Operand1)){
            if(Ctemp->isZero() && Ctemp2->isZero()){
              ToErase.push_back(&I);
            }
          }
          */
        }
        
        if(f){
          outs()<<"?\n";
          if( Inst->getOpcode() == Instruction::Sub ){
            auto *Iter1 = Inst->op_begin();
            Value *Operand = *Iter1;
            Value *Operand1 = *(++Iter1);
            ConstantInt *C1;
            int index=0;
            int f1=0;
            if(ConstantInt *C1temp = dyn_cast<ConstantInt>(Operand1)){
              C1= dyn_cast<ConstantInt>(Operand1);
              index=0;
              f1=1;
            }
            else if(ConstantInt *C1temp = dyn_cast<ConstantInt>(Operand)){
              C1= dyn_cast<ConstantInt>(Operand);
              index=1;
              f1=2;
            }
            if(f1>0){
              if(C1->getValue() == C->getValue()){
                /*
                Value *v;
                Instruction *NewInst;
                if(f==1){
                  v=ConstantInt::get(I.getOperand(1)->getType(),0);
                }
                else if (f==2){
                  v=ConstantInt::get(I.getOperand(0)->getType(),0);
                }
                if(f1==1){
                  NewInst = BinaryOperator::Create(
                    Instruction::Add, Inst->getOperand(0), v);
                }
                if(f1==2){
                  NewInst = BinaryOperator::Create(
                    Instruction::Add, Inst->getOperand(1), v);
                }
                NewInst->insertAfter(&I);
                */
                I.replaceAllUsesWith(Inst->getOperand(index));
                ToErase.push_back(&I);
              }
            }
          }
        }
      } 

if(I.getOpcode() == Instruction::Sub){
        outs()<<I<<"\n";
        auto *Iter = I.op_begin();
        Value *Operand = *Iter;
        Value *Operand1 = *(++Iter);
        ConstantInt *C;
        Instruction *Inst;
        int f=0;
        if (ConstantInt *Ctemp =dyn_cast<ConstantInt>(Operand1)){

          if(Instruction *Insttemp = dyn_cast<Instruction>(Operand)){
            f=1;
            Inst = dyn_cast<Instruction>(Operand);
            C=dyn_cast<ConstantInt>(Operand1);
          }
          if(Ctemp->isZero()){
            outs()<<"added an add operation to remove\n";
            //replace con variable
            I.replaceAllUsesWith(I.getOperand(0));
            ToErase.push_back(&I);
            f=0;
          }
          
        }
        else if (ConstantInt *Ctemp =dyn_cast<ConstantInt>(Operand)){
          if(Instruction *Insttemp = dyn_cast<Instruction>(Operand1)){
            f=2;
            Inst = dyn_cast<Instruction>(Operand1);
            C=dyn_cast<ConstantInt>(Operand);
          }
          if(Ctemp->isZero()){
            outs()<<"added an add operation to remove\n";
            //replace con variable
            I.replaceAllUsesWith(I.getOperand(1));
            ToErase.push_back(&I);
            f=0;
          }
        }
        
        if(f){
          outs()<<"?\n";
          if( Inst->getOpcode() == Instruction::Add ){
            auto *Iter1 = Inst->op_begin();
            Value *Operand = *Iter1;
            Value *Operand1 = *(++Iter1);
            ConstantInt *C1;
            int index=0;
            int f1=0;
            if(ConstantInt *C1temp = dyn_cast<ConstantInt>(Operand1)){
              C1= dyn_cast<ConstantInt>(Operand1);
              index=0;
              f1=1;
            }
            else if(ConstantInt *C1temp = dyn_cast<ConstantInt>(Operand)){
              C1= dyn_cast<ConstantInt>(Operand);
              index=1;
              f1=2;
            }
            if(f1>0){
              if(C1->getValue() == C->getValue()){
                I.replaceAllUsesWith(Inst->getOperand(index));
                ToErase.push_back(&I);
              }
            }
          }
        }
      }
    }
    
    outs()<<"starting remove\n";
    for (Instruction *I : ToErase) {
      outs()<<"removing:"<<*I<<"\n";
      I->eraseFromParent();
    }  
    

    return true;
  }


namespace {


// New PM implementation
struct LocalOpts: PassInfoMixin<LocalOpts> {
 

  PreservedAnalyses run(Function &F, FunctionAnalysisManager &) {
    errs() << F.getName()<<"\n";
    bool Transformed = false;
    
    for (auto Iter = F.begin(); Iter != F.end(); ++Iter) {
      if (runOnBasicBlock(*Iter)) {
        Transformed = true;
      }
    }
    errs()<<"Is transformed"<<Transformed<<" \n";
    return PreservedAnalyses::all();
  }
  




  // Without isRequired returning true, this pass will be skipped for functions
  // decorated with the optnone LLVM attribute. Note that clang -O0 decorates
  // all functions with optnone.
  static bool isRequired() { return true; }
};

} // namespace

//-----------------------------------------------------------------------------
// New PM Registration
//-----------------------------------------------------------------------------
llvm::PassPluginLibraryInfo getLocalOptsPluginInfo() {
  return {LLVM_PLUGIN_API_VERSION, "LocalOpts", LLVM_VERSION_STRING,
          [](PassBuilder &PB) {
            PB.registerPipelineParsingCallback(
                [](StringRef Name, FunctionPassManager &FPM,
                   ArrayRef<PassBuilder::PipelineElement>) {
                  if (Name == "local-opts") {
                    FPM.addPass(LocalOpts());
                    return true;
                  }
                  return false;
                });
          }
  };
}

// This is the core interface for pass plugins. It guarantees that 'opt' will
// be able to recognize LocalOpts when added to the pass pipeline on the
// command line, i.e. via '-passes=local-opts'
extern "C" LLVM_ATTRIBUTE_WEAK ::llvm::PassPluginLibraryInfo
llvmGetPassPluginInfo() {
  return getLocalOptsPluginInfo();
}


