; ModuleID = 'es1m2r.ll'
source_filename = "es1.c"
target datalayout = "e-m:e-p270:32:32-p271:32:32-p272:64:64-i64:64-i128:128-f80:128-n8:16:32:64-S128"
target triple = "x86_64-unknown-linux-gnu"

; Function Attrs: noinline nounwind uwtable
define dso_local void @prova() #0 {
  %1 = add nsw i32 20, 30
  %2 = srem i32 %1, 2
  %3 = icmp eq i32 %2, 0
  %4 = add nsw i32 %1, 1
  br label %5

5:                                                ; preds = %12, %0
  %.0 = phi i32 [ 0, %0 ], [ %13, %12 ]
  %6 = icmp slt i32 %.0, 5
  br i1 %6, label %7, label %14

7:                                                ; preds = %5
  br i1 %3, label %8, label %9

8:                                                ; preds = %7
  br label %10

9:                                                ; preds = %7
  br label %10

10:                                               ; preds = %9, %8
  %.01 = phi i32 [ 2, %8 ], [ 3, %9 ]
  %11 = add nsw i32 %.01, 2
  br label %12

12:                                               ; preds = %10
  %13 = add nsw i32 %.0, 1
  br label %5, !llvm.loop !6

14:                                               ; preds = %5
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
