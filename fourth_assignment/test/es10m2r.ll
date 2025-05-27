; ModuleID = 'es10.bc'
source_filename = "es10.c"
target datalayout = "e-m:e-p270:32:32-p271:32:32-p272:64:64-i64:64-i128:128-f80:128-n8:16:32:64-S128"
target triple = "x86_64-unknown-linux-gnu"

; Function Attrs: noinline nounwind uwtable
define dso_local void @Loop6() #0 {
  br label %1

1:                                                ; preds = %11, %0
  %.03 = phi i32 [ 0, %0 ], [ %12, %11 ]
  %.0 = phi i32 [ 0, %0 ], [ %.1, %11 ]
  %2 = icmp slt i32 %.03, 5
  br i1 %2, label %3, label %13

3:                                                ; preds = %1
  br label %4

4:                                                ; preds = %8, %3
  %.04 = phi i32 [ 0, %3 ], [ %9, %8 ]
  %.1 = phi i32 [ %.0, %3 ], [ %7, %8 ]
  %5 = icmp slt i32 %.04, 5
  br i1 %5, label %6, label %10

6:                                                ; preds = %4
  %7 = add nsw i32 %.1, 1
  br label %8

8:                                                ; preds = %6
  %9 = add nsw i32 %.04, 1
  br label %4, !llvm.loop !6

10:                                               ; preds = %4
  br label %11

11:                                               ; preds = %10
  %12 = add nsw i32 %.03, 1
  br label %1, !llvm.loop !8

13:                                               ; preds = %1
  br label %14

14:                                               ; preds = %24, %13
  %.05 = phi i32 [ 0, %13 ], [ %25, %24 ]
  %.01 = phi i32 [ 0, %13 ], [ %.12, %24 ]
  %15 = icmp slt i32 %.05, 5
  br i1 %15, label %16, label %26

16:                                               ; preds = %14
  br label %17

17:                                               ; preds = %21, %16
  %.06 = phi i32 [ 0, %16 ], [ %22, %21 ]
  %.12 = phi i32 [ %.01, %16 ], [ %20, %21 ]
  %18 = icmp slt i32 %.06, 5
  br i1 %18, label %19, label %23

19:                                               ; preds = %17
  %20 = add nsw i32 %.12, 1
  br label %21

21:                                               ; preds = %19
  %22 = add nsw i32 %.06, 1
  br label %17, !llvm.loop !9

23:                                               ; preds = %17
  br label %24

24:                                               ; preds = %23
  %25 = add nsw i32 %.05, 1
  br label %14, !llvm.loop !10

26:                                               ; preds = %14
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
!9 = distinct !{!9, !7}
!10 = distinct !{!10, !7}
