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
#include "llvm/Analysis/PostDominators.h"
#include "llvm/Analysis/DependenceAnalysis.h"
#include "llvm/Analysis/ScalarEvolution.h"

using namespace llvm;


//-----------------------------------------------------------------------------
// LocalOpts implementation
//-----------------------------------------------------------------------------
// No need to expose the internals of the pass to the outside world - keep
// everything in an anonymous namespace.

BasicBlock* IsGuardedBlock(Loop* L1){
  BasicBlock *guardBlock=nullptr;
  BasicBlock *Preheader= L1->getLoopPreheader();
  //verify if it is simplified loop, if has preheadedr true
  if(!Preheader){
    return guardBlock;
  }
  errs()<<"preheader: "<<*Preheader<<"\n";

  guardBlock = Preheader->getSinglePredecessor();
  
  return guardBlock;
}

bool AdjacencyCheckForGuardedLoop(Loop* L1, Loop* L2,BasicBlock* GuradedBlock){
  errs()<<"Its guarded loop\n";
  BasicBlock *preheader = L1->getLoopPreheader();
  BasicBlock *preheader1 = L2->getLoopPreheader();
  if((!preheader) || (!preheader1)){
    errs()<<"Two loops may haven't preheaders(One of them or both)\n";
    return false;
  }
  errs()<<"preheader1: "<<*preheader<<"\n";
  errs()<<"preheader2: "<<*preheader1<<"\n";
  if (BranchInst *BI = dyn_cast<BranchInst>(GuradedBlock->getTerminator())) {
    errs()<<"Its guarded loop " <<L1->getHeader()<<" its branch is "<<*BI<<"\n";
    if (BI->isConditional()) {
        BasicBlock *Succ0 = BI->getSuccessor(0);
        BasicBlock *Succ1 = BI->getSuccessor(1);
        errs()<<"successor1:\n"<<*Succ0<<"\n------\nsuccessor2:\n"<<*Succ1<<"\n------\n loop1 preheader: \n"<<*L1->getLoopPreheader()<<"\n------\nloop2 preheader: \n"<<*L2->getLoopPreheader()<<"\n------\nloop2 header: \n"<<*L2->getHeader()<<"\n";

        //successore non e' entry di loop(header), e' preheader di loop
        /*
          The branch from the guard (e.g., if) usually goes to the loop’s preheader, 
          not directly to the loop header — because the preheader is a compiler-inserted block 
          that runs once before the loop and helps organize clean loop entry.
        */
        if(preheader != Succ0){
          if(Succ0 != preheader1){
            return false;
          }
          else{
            return true;
          }        
        }
        if(preheader != Succ1){
          if(Succ1 != preheader1){
            return false;
          }
          else{
            return true;
          }          
        }
        errs()<<"both two successor not linked to the preheader of first loop, unknow error\n";
        return false;
    }
  }
  errs()<<"Can't find a branch instruction in guarded block\n";
  return false;

}

bool AdjacencyCheckForLoop(Loop* L1, Loop* L2){
  //verifico solamente se il non loop successore di L1 e' uguale al preheader di L2 or no, 
  //la viceversa va eseguito un altra volta con posizione inverso 
  errs()<<"Its not guarded loop "<<&L1<<"\n";
  BasicBlock *preheader = L1->getLoopPreheader();
  BasicBlock *preheader1 = L2->getLoopPreheader();
  if((!preheader) || (!preheader1)){
    errs()<<"Two loops may haven't preheaders(One of them or both)\n";
    return false;
  }
  errs()<<"preheader1: "<<*preheader<<"\n";
  errs()<<"preheader2: "<<*preheader1<<"\n";
  
  
  SmallVector<BasicBlock*, 4> exitBlocks;
  //retrieve all exitblocks
  L1->getExitBlocks(exitBlocks);
  for (BasicBlock *exit : exitBlocks) {
    errs()<<*exit<<"\n";
      if (exit == preheader1) {
          return true; // A way that L1's exit block is in the preheader of L2
      }
  }
  return false;
}


bool HaveSameLoopCount(Loop* L1, Loop* L2,ScalarEvolution &SE){

  const SCEV *TripCount1 = SE.getBackedgeTakenCount(L1);
  const SCEV *TripCount2 = SE.getBackedgeTakenCount(L2);

  // Check if both SCEV objects are constant values
  if (isa<SCEVConstant>(TripCount1) && isa<SCEVConstant>(TripCount2)) {
      const SCEVConstant *C1 = cast<SCEVConstant>(TripCount1);
      const SCEVConstant *C2 = cast<SCEVConstant>(TripCount2);
      // Compare the constant values directly
      if (C1->getValue()== C2->getValue()) {
          // The constants are equal
          errs() << "The backedge taken counts are equal constants.\n";
          return true;
      } 
  } 
  //I don't know when this is used, but the getBackedgeTakenCount will also return this type of value, 
  //by what Im knowing this refer the induction variable, and special for the the terminator which is unknow(not constant)
  else if (isa<SCEVAddRecExpr>(TripCount1) && isa<SCEVAddRecExpr>(TripCount2)) {
      const SCEVAddRecExpr *AddRec1 = cast<SCEVAddRecExpr>(TripCount1);
      const SCEVAddRecExpr *AddRec2 = cast<SCEVAddRecExpr>(TripCount2);
      errs()<<AddRec1<<"\n";
      // Compare the start and step values of the AddRec expressions
      if (AddRec1->getStart() == AddRec2->getStart() && AddRec1->getOperand(1) == AddRec2->getOperand(1)) {
          // The AddRecExprs are equal
          errs() << "The backedge taken counts are equal on AddRecExpr.\n";
          return true;
      }
  }
  
  return false;

}
bool HasDependence(Loop* FirstLoop, Loop* NextLoop, DependenceInfo& DI ){
  for(BasicBlock *BB : FirstLoop->getBlocks()){
    for(Instruction &Inst: *BB){
      for(BasicBlock* BB1: NextLoop->getBlocks()){
        for(Instruction &Inst1 : *BB1){
          //if Inst is dependente to Inst1
          auto Dep=DI.depends(&Inst1, &Inst, true);
          if(Dep){
            errs()<<"Find dependencies "<<"\n";
            errs()<<Inst<<"\n";
            errs()<<Inst1<<"\n";
            return true;
          }
        }
      }
    }
  }
  return false;
}

PHINode* getInductionVariable(Loop* L1){
  BasicBlock* header=L1->getHeader();
  for(Instruction &I : *header){
    if(PHINode *phi = dyn_cast<PHINode>(&I)){
      return phi;
    }
  }
  return nullptr;
}

namespace {



// New PM implementation
struct LocalOpts: PassInfoMixin<LocalOpts> {
 
  PreservedAnalyses run(Function &F, FunctionAnalysisManager &AM) {

    SmallVector<Loop *, 8> Worklist;
    DominatorTree &DT = AM.getResult<DominatorTreeAnalysis>(F);
    PostDominatorTree &PDT = AM.getResult<PostDominatorTreeAnalysis>(F);
    DependenceInfo &DI =AM.getResult<DependenceAnalysis>(F);
    ScalarEvolution &SE =AM.getResult<ScalarEvolutionAnalysis>(F);
    
    //adiacent 
    /*
    Due loop L0 e L1 sono adiacenti se non ci sono basic blocks
    aggiuntivi nel CFG tra l’uscita di L0 e l’entry di L1.

    Se i loop sono guarded il successore non loop del guard
    branch di L0 deve essere l’entry block di L1.
    Se i loop non sono guarded l’exit block di L0 deve essere il
    preheader di L1
    */
    LoopInfo &LI = AM.getResult<LoopAnalysis>(F);
    for (auto *L : LI){
      Worklist.push_back(L);
    }
    for (auto *Loop : Worklist) {
      for(auto *Loop1 : Worklist){
        if(Loop != Loop1){
          errs()<<"begin comparison\n";
          if(BasicBlock* guardedblock=IsGuardedBlock(Loop)){
            if(BasicBlock* guardedblock1=IsGuardedBlock(Loop1)){
              errs()<<"first guarded block:\n"<<*guardedblock<<"\nsecond guarded block:\n"<<*guardedblock1<<"\n";
              if(guardedblock == guardedblock1){
                errs()<<"find two guarded loop \n"<<*guardedblock<<"\nAND\n"<<*guardedblock1<<"\n";
                if(AdjacencyCheckForGuardedLoop(Loop,Loop1,guardedblock)){
                  errs()<<"find adjacency on "<<*Loop->getHeader()<<" and "<<*Loop1->getHeader()<<"\n";
                  errs()<<"Verifying the dominance\n";
                  if (DT.dominates(guardedblock, Loop1->getHeader()) && PDT.dominates(Loop1->getHeader(), guardedblock)) {
                      errs() <<*guardedblock <<" dominates"<<*Loop1->getHeader()<<"\n"<<*Loop1->getHeader()<<" post-dominates "<<*guardedblock<<"\n";
                    if(!HasDependence(Loop,Loop1,DI)){
                      errs()<<"There is not a dependence between two loop\n";

                    }
                    else
                    {
                      errs()<<"There is some dependences between two loop\n";
                    }
                  }
                }
              }
              errs()<<"The two guarded block aren't same\n";
            }
            errs()<<"One is guarded loop another one not\n";
          }
          else
          {
            if(AdjacencyCheckForLoop(Loop,Loop1)){
              errs()<<"find adjacency on "<<*Loop->getHeader()<<" and "<<*Loop1->getHeader()<<"\n";
              if (DT.dominates(Loop->getHeader(), Loop1->getHeader()) && PDT.dominates(Loop1->getHeader(), Loop->getHeader())) {
                errs() <<*Loop->getHeader() <<" dominates"<<*Loop1->getHeader()<<"\n"<<*Loop1->getHeader()<<" post-dominates "<<*Loop->getHeader()<<"\n";
                //Count the number of backedge, return a number n, which means the loop will run n+1 times 
                const SCEV *TripCount1 = SE.getBackedgeTakenCount(Loop);
                const SCEV *TripCount2 = SE.getBackedgeTakenCount(Loop1);
                //getExitCount(ExitBlock) is another version, we can know the specific number of loop executed for a loop to a specific exitblock
                errs()<<"count for loop1: "<<*TripCount1<<"\n count for loop2: "<<*TripCount2<<"\n";
                //if(*TripCount1->isEqual(*TripCount2)){
                //  errs()<<"Two loop have same trip count\n";
                if(HaveSameLoopCount(Loop,Loop1,SE)){
                  errs()<<"\n";
                }
                if(!HasDependence(Loop,Loop1,DI)){
                  errs()<<"There is not a dependence between two loop\n";
                  PHINode* inductionL1=getInductionVariable(Loop);
                  PHINode* inductionL2=getInductionVariable(Loop1);
                  inductionL2->replaceAllUsesWith(inductionL1);
                  errs()<<"The uses replaced\n";
                }

                
              }
            }

          }

        }
      }
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
              if (Name == "LoopFusion-pass") {
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


