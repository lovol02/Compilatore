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
#include "llvm/IR/Dominators.h"


using namespace llvm;


//-----------------------------------------------------------------------------
// LocalOpts implementation
//-----------------------------------------------------------------------------
// No need to expose the internals of the pass to the outside world - keep
// everything in an anonymous namespace.

std::vector<Instruction*> LoopInvariantInsts;


bool isLoopInvariant(Instruction *I, Loop &L) {
    // isa check the type of Instruction
    if (isa<PHINode>(I)) {
      return false;
    }
    if (isa<BranchInst>(I)) {
        return false;
    }
    for (const auto &Op : I->operands()) {
        if (isa<Instruction>(Op)) {
            Instruction *OpInst = cast<Instruction>(Op);
            if (L.contains(OpInst->getParent())) {
              // The operand is inside the loop
              int flag=0;
              for(Instruction *Inst : LoopInvariantInsts){
                if (Op == Inst){
                  flag=1;
                  continue;
                }
              }
              if(!flag){
                return false;
              }
              //method recursive for above code.
              /*
              if(!isLoopInvariant(OpInst,L)){
                //And the operand isn't the loop invariant, so the instruction is not loop-invariant
                return false;
              } 
              */
            }
        }
    }
    LoopInvariantInsts.push_back(I);
    return true;
}


std::vector<BasicBlock*> getLoopExitBlocks(Loop *L) {
    std::vector<BasicBlock*> ExitBlocks;

    for (BasicBlock *BB : L->blocks()) {
        for (BasicBlock *Succ : successors(BB)) {
            if (!L->contains(Succ)) {
                // This is an exit block (BB has a successor outside the loop)
                ExitBlocks.push_back(BB);
                break; // Don't add the same BB multiple times
            }
        }
    }

    return ExitBlocks;
}
bool IsDominateExit(Instruction *I,std::vector<BasicBlock*> exitBB,DominatorTree &DT){
  outs()<<"Begin dominance verification!\n";
  for(auto BB: exitBB){
    //outs()<<*BB<<"\n"<<I->getParent()<<"\n";
    if(!DT.dominates(I->getParent(),BB)){
      outs()<<"The block belonged by instruction don't dominate all exit\n";
      return false;
    }
  }
  outs()<<"The block belonged by instruction dominate all exit\n";
  return true;
}

bool IsDeadCode(Instruction *I,Loop *L){
  bool flag=false;
      for (User *U : I->users()) {
        if (Instruction *UserInst = dyn_cast<Instruction>(U)) {
          //outs()<<*UserInst<<"\n";
          if (!L->contains(UserInst->getParent())) {
              // The instruction is used outside the loop → not dead
              outs()<<"This Instruction isn't a dead code\n";
              return false;
          }
          //Unable to use a vector like LoopInvariantInsts, cause the UserInst is Deadcode or no will be reveal after the its own identification, 
          //but it will be after the current instruction, unlike the LoopInvariantInsts, will be outside of loop or before current instruction.
          if(!IsDeadCode(UserInst,L)){
            return false;
          }
        }
    }
    outs()<<"This Instruction is a dead code\n";
    return true;
}

bool IsDominatingUsesInLoop(Instruction *I,Loop *L,DominatorTree &DT){
  for (User *U : I->users()) {
    if(isa<PHINode>(U)){
      continue;
    }
    if (Instruction *UserInst = dyn_cast<Instruction>(U)) {
        if (L->contains(UserInst->getParent())) {
            if (!DT.dominates(I->getParent(), UserInst->getParent())) {
              outs()<<*I<<" Non domina "<<*UserInst<<"\n";
                return false; 
            }
        }
    }
  }
  return true;
}
/*
//But never used... cause use mem2reg will remove all store and load intructions
bool IsUsedRedefined1(Instruction *I,Loop *L){
  outs()<<"begin\n";
    // The variable being assigned (you could check if it's a specific variable)
    if (auto *storeInst = dyn_cast<StoreInst>(I)) {
        Value *storedValue = storeInst->getValueOperand();
        Value *storedPointer = storeInst->getPointerOperand();
        for (auto *BB : L->getBlocks()) {
            for (auto &Instr : *BB) {
                if (auto *otherStoreInst = dyn_cast<StoreInst>(&Instr)) {
                    if (otherStoreInst->getPointerOperand() == storedPointer) {
                  
                        return false;
                    }
                }
            }
        }
    }
    return true;
}
//getName() always return " ", so unable to track it. Not used
bool IsUsedRedefined(Instruction *Inst,Loop *L){
  outs()<<"begin\n";
  auto* originalInst=dyn_cast<BinaryOperator>(Inst);
  for(auto* BB:L->blocks()){
    for(auto &I: *BB){
      if(auto *binOp = dyn_cast<BinaryOperator>(&I)){
        if((binOp->getName() == originalInst->getName()) && binOp != originalInst){
          outs()<<binOp->getName()<<"\n"<<originalInst->getName()<<"\n";
          outs()<<*originalInst<<"\n"<<*binOp<<"\n"<<(binOp->getName() == originalInst->getName())<<"\n"<<(binOp != originalInst)<<"\n";
          return false;
        } 
      }
    }
  }
  return true;
}
*/
bool IsMultiUsed(Instruction *Inst,Loop *L){
  for(User *U : Inst->users()){
    if(isa<PHINode>(U)){
      PHINode *PN = cast<PHINode>(U); 
      if(L->contains(PN->getParent())){
        outs()<<"Instructiton used in a PHINode and this Node is inside of loop\n";
        outs()<<*PN<<"\n";
        //A unkown bug, the variable assign in loop but not used will have a PHINode in the loop
        //once used PHINode will disappear, to avoid this case should to use another if to check the situation...
        int flag=0;
        for(unsigned i=0; i< PN->getNumIncomingValues();i++){
          BasicBlock* IncomingBB=PN->getIncomingBlock(i);
          outs()<<*IncomingBB<<"\n";
          if(L->contains(IncomingBB)){
            outs()<<"It contains\n";
            flag++;
          }
        }
        if(flag>1){
          return true;
        }
      }
    }
  }
  return false;
}

void MoveBeforeLoop(Instruction *I,Loop *L){
  if (auto *Preheader = L->getLoopPreheader()) {
    I->moveBefore(Preheader->getTerminator());
  }
}

namespace {



// New PM implementation
struct LocalOpts: PassInfoMixin<LocalOpts> {
 

PreservedAnalyses run(Loop &L, LoopAnalysisManager &LAM,
                        LoopStandardAnalysisResults &AR, LPMUpdater &) {
    DominatorTree &DT = AR.DT;
    std::vector<BasicBlock*> exitBlocks=getLoopExitBlocks(&L);
    std::vector<Instruction*> ToMove;
    for(auto *BB : L.blocks()){
      outs()<<"Inside in a loop \n";
      for(auto &I : *BB){
        outs()<<I<<"\n"; 
        //point1 verify if it is a Loop Invariant 
        if(isLoopInvariant(&I,L)){
          outs()<<"This instruction is loop invariant"<<"\n";
          //If the block instruction belong dominate all loop exit or instruction is a dead code
          if(IsDominateExit(&I,exitBlocks,DT) || IsDeadCode(&I,&L)){
            outs()<<"This instruction Dominate all exit loop or is a dead code\n";
            //If a viariable is assigned more than once in the loop
            if(!IsMultiUsed(&I,&L)){
              outs()<<"This instruction isn't multi defined\n";
              //If the blocks uses the variable is dominated by the block which defines the variable
              if(IsDominatingUsesInLoop(&I,&L,DT)){
                outs()<<"This instruction dominate all uses in the loop\n";
                ToMove.push_back(&I);
              }
            }
          }
        }
      }
    }

    for (Instruction *I : ToMove) {
      outs()<<"moving:"<<*I<<"\n";
      MoveBeforeLoop(I,&L);
    }  
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
              if (Name == "LICM-pass") {
                FPM.addPass(createFunctionToLoopPassAdaptor(LocalOpts()));
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


