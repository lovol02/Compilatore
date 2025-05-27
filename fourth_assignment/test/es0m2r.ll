; ModuleID = 'es0m2r.bc'
source_filename = "loop.c"
target datalayout = "e-m:e-p270:32:32-p271:32:32-p272:64:64-i64:64-i128:128-f80:128-n8:16:32:64-S128"
target triple = "x86_64-unknown-linux-gnu"

; Function Attrs: noinline nounwind uwtable
define dso_local void @Loop7() #0 {
  %1 = icmp eq i32 0, 0
  br i1 %1, label %2, label %10

2:                                                ; preds = %0
  br label %3

3:                                                ; preds = %7, %2
  %.03 = phi i32 [ 0, %2 ], [ %8, %7 ]
  %.0 = phi i32 [ 0, %2 ], [ %6, %7 ]
  %4 = icmp slt i32 %.03, 5
  br i1 %4, label %5, label %9

5:                                                ; preds = %3
  %6 = add nsw i32 %.0, 1
  br label %7

7:                                                ; preds = %5
  %8 = add nsw i32 %.03, 1
  br label %3, !llvm.loop !6

9:                                                ; preds = %3
  br label %10

10:                                               ; preds = %9, %0
  %11 = icmp eq i32 0, 0
  br i1 %11, label %12, label %20

12:                                               ; preds = %10
  br label %13

13:                                               ; preds = %17, %12
  %.02 = phi i32 [ 0, %12 ], [ %18, %17 ]
  %.01 = phi i32 [ 0, %12 ], [ %16, %17 ]
  %14 = icmp slt i32 %.02, 5
  br i1 %14, label %15, label %19

15:                                               ; preds = %13
  %16 = add nsw i32 %.01, 1
  br label %17

17:                                               ; preds = %15
  %18 = add nsw i32 %.02, 1
  br label %13, !llvm.loop !8

19:                                               ; preds = %13
  br label %20

20:                                               ; preds = %19, %10
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
!8 = distinct !{!8, !7}
