; ModuleID = 'prova1opt.ll'
source_filename = "prova1.c"
target datalayout = "e-m:e-p270:32:32-p271:32:32-p272:64:64-i64:64-i128:128-f80:128-n8:16:32:64-S128"
target triple = "x86_64-unknown-linux-gnu"

; Function Attrs: noinline nounwind uwtable
define dso_local void @prova() #0 {
  br label %1

1:                                                ; preds = %5, %0
  %.02 = phi i32 [ 0, %0 ], [ %6, %5 ]
  %.01 = phi i32 [ undef, %0 ], [ %.02, %5 ]
  %.0 = phi i32 [ 0, %0 ], [ %4, %5 ]
  %2 = icmp slt i32 %.02, 5
  br i1 %2, label %3, label %7

3:                                                ; preds = %1
  %4 = add nsw i32 0, 10
  br label %5

5:                                                ; preds = %3
  %6 = add nsw i32 %.02, 1
  br label %1, !llvm.loop !6

7:                                                ; preds = %1
  %.01.lcssa = phi i32 [ %.01, %1 ]
  %.0.lcssa = phi i32 [ %.0, %1 ]
  %8 = add nsw i32 %.01.lcssa, %.0.lcssa
  ret void
}

attributes #0 = { noinline nounwind uwtable "frame-pointer"="all" "min-legal-vector-width"="0" "no-trapping-math"="true" "stack-protector-buffer-size"="8" "target-cpu"="x86-64" "target-features"="+cmov,+cx8,+fxsr,+mmx,+sse,+sse2,+x87" "tune-cpu"="generic" }

!llvm.module.flags = !{!0, !1, !2, !3, !4}
!llvm.ident = !{!5}

!0 = !{i32 1, !"wchar_size", i32 4}
!1 = !{i32 8, !"PIC Level", i32 2}
!2 = !{i32 7, !"PIE Level", i32 2}
!3 = !{i32 7, !"uwtable", i32 2}
!4 = !{i32 7, !"frame-pointer", i32 2}
!5 = !{!"clang version 19.1.7 (/home/runner/work/llvm-project/llvm-project/clang cd708029e0b2869e80abe31ddb175f7c35361f90)"}
!6 = distinct !{!6, !7}
!7 = !{!"llvm.loop.mustprogress"}
