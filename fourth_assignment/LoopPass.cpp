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
#include "llvm/IR/Verifier.h"
using namespace llvm;


struct PairOfLoop {
    Loop *L1;
    Loop *L2;
};


//-----------------------------------------------------------------------------
// LocalOpts implementation
//-----------------------------------------------------------------------------
// No need to expose the internals of the pass to the outside world - keep
// everything in an anonymous namespace.

bool IsSimplifyForm(Loop* L1){
  BasicBlock *Preheader= L1->getLoopPreheader();
  BasicBlock *header=L1->getHeader();
  int backedgeCount = L1->getNumBackEdges();
  SmallVector<BasicBlock*, 4> exitBlocks;
  L1->getExitBlocks(exitBlocks);
  for (BasicBlock *exit : exitBlocks) {
      for (auto *pred : predecessors(exit)) {
          if (!L1->contains(pred)) {
              return false;
          }
      }
  }
  if(header && Preheader && backedgeCount <2){
    return true;
  }
  return false;
}

BasicBlock* IsGuardedBlock(Loop* L1,SmallVector<Loop *, 8> Worklist){
  BasicBlock *guardBlock=nullptr;
  BasicBlock *Preheader= L1->getLoopPreheader();

  if(!Preheader){
    errs()<<"Not find the header of loop\n";
    return guardBlock;
  }
  errs()<<"preheader: "<<*Preheader<<"\n";

  guardBlock = Preheader->getSinglePredecessor();
  if(guardBlock){
    for(Loop* L : Worklist){
      if(L->getHeader() == guardBlock){
        return IsGuardedBlock(L,Worklist);
      }
    }
    if(BranchInst* BI= dyn_cast<BranchInst>(guardBlock->getTerminator())){
      if(BI->isConditional()){
        errs()<<"Guard block\n";
        return guardBlock;
        
      }
    }

  }


  return nullptr;
}
bool isEqual(BasicBlock* guard1,BasicBlock* guard2){
  ICmpInst *Icmp1,*Icmp2;
  for(auto& Inst: *guard1){
    if(dyn_cast<ICmpInst>(&Inst)){
      Icmp1=dyn_cast<ICmpInst>(&Inst);
    }
  }
  for(auto& Inst: *guard2){
    if(dyn_cast<ICmpInst>(&Inst)){
      Icmp2=dyn_cast<ICmpInst>(&Inst);
    }
  }
  errs()<<*Icmp1<<"\n"<<*Icmp2<<"\n";
  if(Icmp1->getPredicate() == Icmp2->getPredicate() )
  {
    if(Icmp1->getOperand(0) == Icmp2->getOperand(0) && Icmp1->getOperand(1) && Icmp2->getOperand(1)){
      return true;
    }
  }
  return false;
}
bool AdjacencyCheckForGuardedLoop(Loop* L1, Loop* L2,BasicBlock* GuardedBlock,BasicBlock* GuardedBlock1){
  errs()<<"Its guarded loop\n";
  BasicBlock *preheader = L1->getLoopPreheader();
  //Not used
  BasicBlock *preheader1 = L2->getLoopPreheader();
  if((!preheader) || (!preheader1)){
    errs()<<"Two loops may haven't preheaders(One of them or both)\n";
    return false;
  }
  errs()<<"preheader1: "<<*preheader<<"\n";
  errs()<<"preheader2: "<<*preheader1<<"\n";
  if (BranchInst *BI = dyn_cast<BranchInst>(GuardedBlock->getTerminator())) {
    if (BI->isConditional()) {
      for(int i=0;i<BI->getNumSuccessors();i++){
        BasicBlock *Succ = BI->getSuccessor(i);
        if(preheader != Succ){
          if(Succ != GuardedBlock1){
            continue;
          }
          else{
            return true;
          }        
        }
      }
        /**
        BasicBlock *Succ0 = BI->getSuccessor(0);
        BasicBlock *Succ1 = BI->getSuccessor(1);
        errs()<<"successor1:\n"<<*Succ0<<"\n------\nsuccessor2:\n"<<*Succ1<<"\n------\n loop1 preheader: \n"<<*L1->getLoopPreheader()<<"\n------\nloop2 preheader: \n"<<*L2->getLoopPreheader()<<"\n------\nloop2 header: \n"<<*L2->getHeader()<<"\n";

        //successore non e' entry di loop(header), e' preheader di loop
        
        //  The branch from the guard (e.g., if) usually goes to the loop’s preheader, 
        //  not directly to the loop header — because the preheader is a compiler-inserted block 
        //  that runs once before the loop and helps organize clean loop entry.
        
        if(preheader != Succ0){
          if(Succ0 != GuardedBlock1){
            return false;
          }
          else{
            return true;
          }        
        }
        if(preheader != Succ1){
          if(Succ1 != GuardedBlock1){
            return false;
          }
          else{
            return true;
          }          
        }
        errs()<<"both two successor not linked to the preheader of first loop, unknow error\n";
        **/
        return false;
    }
  }
  errs()<<"Can't find a branch instruction in guarded block\n";
  return false;

}

bool AdjacencyCheckForLoop(Loop* L1, Loop* L2){
  //verifico solamente se il non loop successore di L1 e' uguale al preheader di L2 or no, 
  //la viceversa va eseguito un altra volta con posizione inverso 
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
  if(TripCount1==TripCount2){
    errs()<<"equal"<<"\n";
    return true;
  }
  /*
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
  */
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

BasicBlock *findLatch(Loop *L) {
    BasicBlock *Header = L->getHeader();

    for (BasicBlock *Pred : predecessors(Header)) {
        if (L->contains(Pred)) {
            // This predecessor inside the loop is the latch block
            return Pred;
        }
    }
    return nullptr;
}

bool HasNegativeDependence(Loop* FirstLoop, Loop* NextLoop,DependenceInfo &DI,ScalarEvolution &SE){
  SmallVector<Instruction *, 8> StoreInsts;
  SmallVector<Instruction *, 8> LoadInsts;
  //The following two block only take the storeInst in the first loop and the loadInst of the second, 
  //to the if im accessing the location which is still not write
  for(BasicBlock *BB : FirstLoop->getBlocks()){
    for(Instruction &Inst: *BB){
      if(isa<StoreInst>(&Inst)){
        StoreInsts.push_back(&Inst);
      }
    }
  }
  
  for(BasicBlock* BB1: NextLoop->getBlocks()){
    for(Instruction &Inst1 : *BB1){
        if(isa<LoadInst>(&Inst1)){
          LoadInsts.push_back(&Inst1);
        }
    }
  }
  int stepVal=0;
  //Is induction variable creasing or decreasing?
  PHINode* inductionL1=getInductionVariable(FirstLoop);
  const SCEV *S = SE.getSCEV(inductionL1);
  if (const SCEVAddRecExpr *AR = dyn_cast<SCEVAddRecExpr>(S)) {
    const SCEV *Step = AR->getStepRecurrence(SE);
    if (const SCEVConstant *C = dyn_cast<SCEVConstant>(Step)) {
    stepVal = C->getAPInt().getSExtValue();
    }
  }
  errs()<<"The induction variable is "<<stepVal<<"\n";
  /*
  For case store first then use it
  */
  
  for(Instruction *I : StoreInsts){
    //auto *storeInst = dyn_cast<StoreInst>(I);
    //errs()<<"store operand pointer: "<<*storeInst->getPointerOperand()<<"\n";
    for(Instruction *I1 : LoadInsts){
      /*
      auto *loadInst = dyn_cast<LoadInst>(I1);
      errs()<<"load operand pointer: "<<*loadInst->getPointerOperand()<<"\n";
      errs()<<"Working in dependencies\n"<<*I1<<"\nAND\n"<<*I<<"\n";
      
      Value* ptrStore = storeInst->getPointerOperand();
      Value* ptrLoad = loadInst->getPointerOperand();
      
      const SCEV *loadSCEV = SE.getSCEV(ptrLoad);
      const SCEV *storeSCEV = SE.getSCEV(ptrStore);
      errs()<<"load "<<*loadSCEV<<"\nstore "<<*storeSCEV<<"\n";
      
      const SCEV *Diff = SE.getMinusSCEV(storeSCEV, loadSCEV);
      errs()<<"diff is"<<*Diff<<"\n"<<SE.isKnownNegative(Diff)<<"\n";
      if(SE.isKnownNegative(Diff)){
        errs() << "Negative dependence distance detected!\n";
        return true;
      }
      */
    
    //If I1 depends to I
      auto Dep = DI.depends(I1, I, true);
      if (Dep) {
        errs()<<"find dep\n";
        auto *storeInst = dyn_cast<StoreInst>(I);
        auto *loadInst = dyn_cast<LoadInst>(I1);
        Value* ptrStore = storeInst->getPointerOperand();
        Value* ptrLoad = loadInst->getPointerOperand();
        const SCEV *loadSCEV = SE.getSCEV(ptrLoad);
        const SCEV *storeSCEV = SE.getSCEV(ptrStore);
        //The following operation return a expression in SCEV, instead a constant in SCEV, 
        //so the better way is extract the contante value separately from different SCEV object then compare them
        const SCEV *Diff = SE.getMinusSCEV(storeSCEV, loadSCEV);
        errs()<<"load "<<*loadSCEV<<"\nstore "<<*storeSCEV<<"\nin SCEVAddExpr\n"<<*Diff<<"\n";
        const SCEVAddRecExpr *AddRec = dyn_cast<SCEVAddRecExpr>(storeSCEV);
        const SCEVAddRecExpr *AddRec1 = dyn_cast<SCEVAddRecExpr>(loadSCEV);
        if( AddRec && AddRec1 ){
          errs()<<"prova\n";
          errs()<<AddRec1->getStart()<<"\n"<< AddRec->getStart()<<"\n";
          errs()<<AddRec1->getOperand(0)<<"\n"<<AddRec->getOperand(0)<<"\n";
          errs()<<AddRec1->getOperand(1)<<"\n"<<AddRec->getOperand(1)<<"\n";
          if (AddRec1->getStart() > AddRec->getStart()) {
              // The AddRecExprs are equal
              errs() << "negative, read before write\n";
              return true;
          }
        }
        /*
        if (const SCEVAddExpr *AddExpr = dyn_cast<SCEVAddExpr>(Diff)) {

          for (const SCEV *Op : AddExpr->operands()) {
            if (const SCEVAddRecExpr *AddRec = dyn_cast<SCEVAddRecExpr>(Op)) {
              errs() << "Found AddRec: " << *AddRec << "\n";
              const SCEV *Step = AddRec->getStepRecurrence(SE);
              errs() << "Step: " << *Step << "\n";
              if (const SCEVConstant *StepC = dyn_cast<SCEVConstant>(Step)) {
                  errs()<<StepC->getValue()<<"\n";
              }
            }
          }


          const SCEVAddRecExpr *AddRec = dyn_cast<SCEVAddRecExpr>(storeSCEV);
          const SCEVAddRecExpr *AddRec1 = dyn_cast<SCEVAddRecExpr>(loadSCEV);
          if( AddRec && AddRec1 ){
            errs()<<"prova\n";
            errs()<<AddRec1->getStart()<<"\n"<< AddRec->getStart()<<"\n";
            errs()<<AddRec1->getOperand(0)<<"\n"<<AddRec->getOperand(0)<<"\n";
            errs()<<AddRec1->getOperand(1)<<"\n"<<AddRec->getOperand(1)<<"\n";
            if (AddRec1->getStart() == AddRec->getStart() && AddRec1->getOperand(1) == AddRec->getOperand(1)) {
                // The AddRecExprs are equal
                errs() << "1.\n";
                return true;
            }
          }

        }
        */
      }
      else{
        errs()<<"Non dependencies found\n";
      }
    
    } 
  }
  
  return false;
}


void LoopFusion(Loop* L1,Loop* L2, SmallVector<Loop *, 8> Worklist,LoopInfo &LI,DominatorTree &DT){
  errs()<<*L1->getHeader()<<"\n"<<*L2->getHeader()<<"\n";
  errs()<<"Getting Induction variable\n";
  PHINode* inductionL1=getInductionVariable(L1);
  PHINode* inductionL2=getInductionVariable(L2);
  errs() << "Induction variable: " << *inductionL1 << "\n";
  errs() << "Induction variable: " << *inductionL2 << "\n";
  errs()<<"End to get Induction variable\n";


  

  errs()<<"if condition\n"<<*L2->getLoopPreheader()->getSinglePredecessor()<<"\n";
  BasicBlock* L1Header= L1->getHeader();
  BasicBlock* L2Header= L2->getHeader();
  BasicBlock* L2Latch= L2->getLoopLatch();
  BasicBlock* L1Latch= L1->getLoopLatch();
  BasicBlock* L2FirstBody= nullptr,*L2EndBody=nullptr,* L1EndBody=nullptr,*L2NoLoopExit=nullptr;
  BranchInst *BI1=nullptr,*BI2=nullptr;
  int L1NoLoopIndex=-1;
  if(isa<BranchInst>(L2Header->getTerminator())){
    BranchInst* BI=dyn_cast<BranchInst>(L2Header->getTerminator());

    if(BI->isConditional()){
      BI2=BI;

    }
  }
  if(isa<BranchInst>(L2Latch->getTerminator())){
    BranchInst* BI=dyn_cast<BranchInst>(L2Latch->getTerminator());

    if(BI->isConditional()){
      BI2=BI;

    }
  }
  if(isa<BranchInst>(L1Header->getTerminator())){
    BranchInst* BI=dyn_cast<BranchInst>(L1Header->getTerminator());

    if(BI->isConditional()){
      BI1=BI;

    }
  }
  if(isa<BranchInst>(L1Latch->getTerminator())){
    BranchInst* BI=dyn_cast<BranchInst>(L1Latch->getTerminator());

    if(BI->isConditional()){
      BI1=BI;

    }
  }
  if(BI1 && BI2){
    errs()<<"exit branch1\n"<<*BI1<<"\nexit branch2\n"<<*BI2<<"\n";
    for(int i=0;i<BI2->getNumSuccessors();i++){
      BasicBlock* BB=BI2->getSuccessor(i);
      if(L2->contains(BB)){
        L2FirstBody=BB;
        errs()<<"FirstBody\n"<<*L2FirstBody<<"\n";
      }
      else if(!L2->contains(BB)){
        L2NoLoopExit=BB;
        errs()<<"NoLoopExit\n"<<*BB<<"\n";
      }
    }
    for(int i=0;i<BI1->getNumSuccessors();i++){
      BasicBlock* BB=BI1->getSuccessor(i);
      if(!L1->contains(BB)){
        L1NoLoopIndex=i;
        errs()<<"NoLoopIndex: "<<L1NoLoopIndex<<"\n";
      }
    }

  }

  L1EndBody=L1Latch->getSinglePredecessor();
  L2EndBody=L2Latch->getSinglePredecessor();
  //redirection
  if(isa<BranchInst>(L2EndBody->getTerminator())){
    BranchInst *L2BI = dyn_cast<BranchInst>(L2EndBody->getTerminator());
    if(!L2BI->isConditional()){
      L2EndBody->getTerminator()->setSuccessor(0,L1Latch);
      
      if(L1NoLoopIndex!=-1){
        errs()<<"Modifying the non loop exit\nbefore\n"<<*BI1->getSuccessor(L1NoLoopIndex)<<"\n";
        BI1->setSuccessor(L1NoLoopIndex,L2NoLoopExit);
        errs()<<"Modified the non loop exit\nafter\n"<<*BI1->getSuccessor(L1NoLoopIndex)<<"\n";
      }

      if(isa<BranchInst>(L1EndBody->getTerminator())){
        BranchInst *L1BI = dyn_cast<BranchInst>(L1EndBody->getTerminator());
        if(!L1BI->isConditional()){
          L1EndBody->getTerminator()->setSuccessor(0,L2FirstBody);
        }
      }
    }
  }
  errs()<<"End of redirection\n";

  errs()<<"Before replace\n";
  inductionL2->replaceAllUsesWith(inductionL1);
  for (auto &U : inductionL2->uses()) {
    Instruction *userInst = dyn_cast<Instruction>(U.getUser());
    if (!DT.dominates(inductionL1, userInst)) {
      errs() << "Dominance error: inductionL1 does not dominate use: " << *userInst << "\n";
    }
  }
  



  errs()<<"Induction variable moving....\n";
  if (inductionL2->use_empty()){
    inductionL2->eraseFromParent();
  }
  errs()<<"Induction variable moved....\nBasicBlock in L2 after modification of induction variable\n";
  int icmpflag=0,removeheader=1;
  //move the body
  for(BasicBlock* BB : L2->getBlocks()){
    if(BB == L2Header){
      icmpflag=0;
      for(Instruction &Inst : *BB){
        if(isa<ICmpInst>(&Inst)){
          icmpflag++;
          
        }
      }
      if(icmpflag>0){
        continue;
        errs()<<"header of L2\n"<<*BB<<"\nContain ICmpInstruction\n";
      }
      else{
        removeheader=0;
        L1EndBody->getTerminator()->setSuccessor(0,L2Header);
        BB->moveBefore(L1Latch);
        //To notify the LI the Loop content is changed
        L1->addBasicBlockToLoop(BB, LI);
        errs()<<"header of L2\n"<<*BB<<"\nDoesn't contain ICmpInstruction, added to loop1\n";
      }
    }
    else if(BB == L2Latch){
      errs()<<"latch of L2\n"<<*BB<<"\n";
    }
    else if(BB != L2Header && BB != L2Latch){
      errs()<<"BasicBlock"<<*BB<<"\n";
      BB->moveBefore(L1Latch);
      //To notify the LI the Loop content is changed
      L1->addBasicBlockToLoop(BB, LI);

    }
  }

  
  
  if( L2->getLoopPreheader()){
    //in case guarded, the guarded is loopheader...So this if isn't needed.
    errs()<<"Loop preheader\n"<<*L2->getLoopPreheader()<<"\n";
    L2->getLoopPreheader()->eraseFromParent();
  }
  if( L2->getLoopLatch()){

    errs()<<"Loop preheader\n"<<*L2->getLoopLatch()<<"\n";
    L2->getLoopLatch()->eraseFromParent();
  }
  if(removeheader){
    L2->getHeader()->eraseFromParent();
  }
  

  LI.erase(L2);

  

  for(BasicBlock* BB : L1->getBlocks()){
    errs()<<". Block in L1:\n"<<*BB<<"\n";
  }

}

void LoopFusionForGuarded(Loop* L1,Loop* L2, SmallVector<Loop *, 8> Worklist,LoopInfo &LI,DominatorTree &DT,BasicBlock* guardedblock1,BasicBlock* guardedblock2){

  //We don't have to compare if the two guard block have same if condition or not, cause we already done it.
  errs()<<"GB1:\n"<<*guardedblock1<<"\nGB2:\n"<<*guardedblock2<<"\n";
  //Get the exit of second if 
  BasicBlock* condExitBB=nullptr;
  if(BranchInst* BI = dyn_cast<BranchInst>(guardedblock2->getTerminator())){
    for(unsigned i=0;i<BI->getNumSuccessors();i++){
      if(!(BI->getSuccessor(i) == L2->getLoopPreheader())){
        condExitBB=BI->getSuccessor(i);
      }
    }
  }
  if(condExitBB){
    errs()<<"The exitblock is \n"<<*condExitBB<<"\n";
    int index=-1;
    if(BranchInst* BI = dyn_cast<BranchInst>(guardedblock1->getTerminator())){
      for(unsigned i=0;i<BI->getNumSuccessors();i++){
        if(BI->getSuccessor(i) == guardedblock2){
          index=i;
          errs()<<"GB1's exit before\n"<<*BI->getSuccessor(i)<<"\n";
          BI->setSuccessor(index,condExitBB);
          errs()<<"GB1's exit after\n"<<*BI->getSuccessor(i)<<"\n";
        }
      }
    }
  }


  
  errs()<<*L1->getHeader()<<"\n"<<*L2->getHeader()<<"\n";
  errs()<<"Getting Induction variable\n";
  PHINode* inductionL1=getInductionVariable(L1);
  PHINode* inductionL2=getInductionVariable(L2);
  errs() << "Induction variable L1: " << *inductionL1 << "\n";
  errs() << "Induction variable L2: " << *inductionL2 << "\n";
  errs()<<"End to get Induction variable\n";


  BasicBlock* L1Header= L1->getHeader();
  BasicBlock* L2Header= L2->getHeader();
  BasicBlock* L2Latch= L2->getLoopLatch();
  BasicBlock* L1Latch= L1->getLoopLatch();
  errs()<<"L1 header\n"<<*L1Header<<"\nL1 latch\n"<<*L1Latch<<"\n";
  errs()<<"L2 header\n"<<*L2Header<<"\nL2 latch\n"<<*L2Latch<<"\n";
  BasicBlock* L2FirstBody= nullptr,*L2EndBody=nullptr,* L1EndBody=nullptr,*L2NoLoopExit=nullptr;
  BranchInst *BI1=nullptr,*BI2=nullptr;
  int L1NoLoopIndex=-1;
  if(isa<BranchInst>(L2Header->getTerminator())){
    BranchInst* BI=dyn_cast<BranchInst>(L2Header->getTerminator());

    if(BI->isConditional()){
      BI2=BI;

    }
  }
  if(isa<BranchInst>(L2Latch->getTerminator())){
    BranchInst* BI=dyn_cast<BranchInst>(L2Latch->getTerminator());

    if(BI->isConditional()){
      BI2=BI;

    }
  }
  if(isa<BranchInst>(L1Header->getTerminator())){
    BranchInst* BI=dyn_cast<BranchInst>(L1Header->getTerminator());

    if(BI->isConditional()){
      BI1=BI;

    }
  }
  if(isa<BranchInst>(L1Latch->getTerminator())){
    BranchInst* BI=dyn_cast<BranchInst>(L1Latch->getTerminator());

    if(BI->isConditional()){
      BI1=BI;

    }
  }
  if(BI1 && BI2){
    errs()<<"exit branch1\n"<<*BI1<<"\nexit branch2\n"<<*BI2<<"\n";
    for(int i=0;i<BI2->getNumSuccessors();i++){
      BasicBlock* BB=BI2->getSuccessor(i);
      if(L2->contains(BB)){
        L2FirstBody=BB;
        errs()<<"FirstBody\n"<<*L2FirstBody<<"\n";
      }
      else if(!L2->contains(BB)){
        L2NoLoopExit=BB;
        errs()<<"NoLoopExit\n"<<*BB<<"\n";
      }
    }
    for(int i=0;i<BI1->getNumSuccessors();i++){
      BasicBlock* BB=BI1->getSuccessor(i);
      if(!L1->contains(BB)){
        L1NoLoopIndex=i;
        errs()<<"NoLoopInde: "<<L1NoLoopIndex<<"\n";
      }
    }

  }

  

  L1EndBody=L1Latch->getSinglePredecessor();
  L2EndBody=L2Latch->getSinglePredecessor();


  errs()<<"Before replace\n";
  inductionL2->replaceAllUsesWith(inductionL1);
  /*
  for (auto &U : inductionL2->uses()) {
    Instruction *userInst = dyn_cast<Instruction>(U.getUser());
    if (!DT.dominates(inductionL1, userInst)) {
      errs() << "Dominance error: inductionL1 does not dominate use: " << *userInst << "\n";
    }
  }
  */



  errs()<<"Induction variable moving....\n";
  if (inductionL2->use_empty()){
    inductionL2->eraseFromParent();
  }
  errs()<<"Induction variable moved....\n";
  
  //redirection
  if(isa<BranchInst>(L2EndBody->getTerminator())){
    BranchInst *L2BI = dyn_cast<BranchInst>(L2EndBody->getTerminator());
    if(!L2BI->isConditional()){
      L2EndBody->getTerminator()->setSuccessor(0,L1Latch);
      
      if(L1NoLoopIndex!=-1){
        errs()<<"Modifying the non loop exit\nbefore\n"<<*BI1->getSuccessor(L1NoLoopIndex)<<"\n";
        BI1->setSuccessor(L1NoLoopIndex,L2NoLoopExit);
        errs()<<"Modified the non loop exit\nafter\n"<<*BI1->getSuccessor(L1NoLoopIndex)<<"\n";
      }

      if(isa<BranchInst>(L1EndBody->getTerminator())){
        BranchInst *L1BI = dyn_cast<BranchInst>(L1EndBody->getTerminator());
        if(!L1BI->isConditional()){
          L1EndBody->getTerminator()->setSuccessor(0,L2FirstBody);
        }
      }
    }
  }
  errs()<<"End of redirection\n";


  int icmpflag=0,removeheader=1;
  //move the body
  for(BasicBlock* BB : L2->getBlocks()){
    errs()<<"block of L2(including header and latch)\n"<<*BB<<"\n";
    if(BB == L2Header){
      icmpflag=0;
      for(Instruction &Inst : *BB){
        if(isa<ICmpInst>(&Inst)){
          icmpflag++;
        }
      }
      if(icmpflag>0){
        continue;
        errs()<<"header of L2\n"<<*BB<<"\nContain ICmpInstruction\n";
      }
      else{
        removeheader=0;
        L1EndBody->getTerminator()->setSuccessor(0,L2Header);
        BB->moveBefore(L1Latch);
        //To notify the LI the Loop content is changed
        L1->addBasicBlockToLoop(BB, LI);
        errs()<<"header of L2\n"<<*BB<<"\nDoesn't contain ICmpInstruction, added to loop1\n";
      }
      
    }
    else if(BB == L2Latch){
      errs()<<"latch of L2\n"<<*BB<<"\n";
    }
    else if(BB != L2Header && BB != L2Latch){
      errs()<<"BasicBlock"<<*BB<<"\n";
      BB->moveBefore(L1Latch);
      //To notify the LI the Loop content is changed
      L1->addBasicBlockToLoop(BB, LI);

    }
  }

  
  
  
  if(guardedblock2->getSinglePredecessor()){
    errs()<<"GB2 predecessor\n"<<*guardedblock2->getSinglePredecessor()<<"\n";
    guardedblock2->getSinglePredecessor()->eraseFromParent();
  }
  errs()<<"guard2\n"<<*guardedblock2<<"\n";
  guardedblock2->eraseFromParent();
  
  
  if( L2->getLoopPreheader()){
    //in case guarded, the guarded is loopheader...So this if isn't needed.
    errs()<<"Loop preheader\n"<<*L2->getLoopPreheader()<<"\n";
    L2->getLoopPreheader()->eraseFromParent();
  }
  if( L2->getLoopLatch()){

    errs()<<"Loop preheader\n"<<*L2->getLoopLatch()<<"\n";
    L2->getLoopLatch()->eraseFromParent();
  }

  
  if(removeheader){
    errs()<<"remove l2 header executing....\n";
    L2->getHeader()->eraseFromParent();
  }
  
  LI.erase(L2);

  errs()<<"printing the new loop1 (version after motion)\n";
  for(BasicBlock* BB : L1->getBlocks()){
    errs()<<"Block in L1:\n"<<*BB<<"\n";
  }

  
}

namespace {



// New PM implementation
struct LocalOpts: PassInfoMixin<LocalOpts> {
 
  PreservedAnalyses run(Function &F, FunctionAnalysisManager &AM) {

    SmallVector<Loop *, 8> Worklist;

    SmallVector<PairOfLoop, 8> LoopFusionList;
    LoopInfo &LI = AM.getResult<LoopAnalysis>(F);
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

    for (auto *L : LI){
      Worklist.push_back(L);
    }
    for (auto *Loop : Worklist) {
      for(auto *Loop1 : Worklist){
        if(Loop != Loop1){
          errs()<<"begin comparison\n";
          BasicBlock* guardedblock=IsGuardedBlock(Loop,Worklist);
          BasicBlock* guardedblock1=IsGuardedBlock(Loop1,Worklist);
          if(guardedblock){
            errs()<<"first is guraded\n";
          }
          else{
            errs()<<"first is not guraded\n";
          }

          if(guardedblock1){
            errs()<<"second is guarded\n";
          }
          else{
            errs()<<"second is not guraded\n";
          }

          if(guardedblock && guardedblock1){
              errs()<<"first guarded block:\n"<<*guardedblock<<"\nsecond guarded block:\n"<<*guardedblock1<<"\n";
              if(isEqual(guardedblock,guardedblock1)){
                errs()<<"find two guarded loop \n"<<*guardedblock<<"\nAND\n"<<*guardedblock1<<"\n They are equals in the condition\n";
                if(AdjacencyCheckForGuardedLoop(Loop,Loop1,guardedblock,guardedblock1)){
                  errs()<<"find adjacency on "<<*Loop->getHeader()<<" and "<<*Loop1->getHeader()<<"\n";
                  errs()<<"Verifying the dominance\n";
                  if (DT.dominates(guardedblock, guardedblock1) && PDT.dominates(guardedblock1, guardedblock)) {
                    errs() <<*guardedblock <<" dominates"<<*guardedblock1<<"\n"<<*guardedblock1<<" post-dominates "<<*guardedblock<<"\n";
                    if(HaveSameLoopCount(Loop,Loop1,SE)){
                      errs()<<"have same trip count\n";
                      if(!HasNegativeDependence(Loop,Loop1,DI,SE)){
                        errs()<<"There is no negative dependence between two loop\n";
                        LoopFusionList.push_back({Loop,Loop1});

                        /*
                        PHINode* inductionL1=getInductionVariable(Loop);
                        PHINode* inductionL2=getInductionVariable(Loop1);
                        inductionL2->replaceAllUsesWith(inductionL1);
                        */
                      }
                      else
                      {
                        errs()<<"There is negative dependences between two loop\n";
                      }

                    }
                    

                    
                  }
                }
              }
              else{
                errs()<<"The two guarded block aren't same\n";
              } 
          }
          else if( !guardedblock && !guardedblock1)
          {
            errs()<<"both not guarded\n";
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
                  errs()<<"have same trip count\n";
                  if(!HasNegativeDependence(Loop,Loop1,DI,SE)){
                    errs()<<"There is no negative dependence between two loop\n";
                    LoopFusionList.push_back({Loop,Loop1});

                    /*
                    PHINode* inductionL1=getInductionVariable(Loop);
                    PHINode* inductionL2=getInductionVariable(Loop1);
                    inductionL2->replaceAllUsesWith(inductionL1);
                    errs()<<"The uses replaced\n";
                    */
                  }
                  else
                  {
                    errs()<<"There is negative dependences between two loop\n";
                  }
                }


                
              }
            }
            errs()<<"They aren't adjacent\n";
          }

        }
      }

    }
    //Code moving
    auto Lbegin= LoopFusionList.begin();
    auto Lend=LoopFusionList.end();
    while(Lbegin != Lend){
      Loop* L1 = Lbegin->L1;
      Loop* L2 = Lbegin->L2;
      BasicBlock* guardedblock1 = IsGuardedBlock(L1,Worklist);
      BasicBlock* guardedblock2 = IsGuardedBlock(L2,Worklist);
      if(guardedblock1 && guardedblock2){
        LoopFusionForGuarded(L1,L2,Worklist,LI,DT,guardedblock1,guardedblock2);
      }
      else{
        LoopFusion(L1,L2,Worklist,LI,DT);
      }



      

      
      ++Lbegin;
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


