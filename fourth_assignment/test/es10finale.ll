; ModuleID = 'es10finale.bc'
source_filename = "es10.c"
target datalayout = "e-m:e-p270:32:32-p271:32:32-p272:64:64-i64:64-i128:128-f80:128-n8:16:32:64-S128"
target triple = "x86_64-unknown-linux-gnu"

; Function Attrs: noinline nounwind uwtable
define dso_local void @Loop6() #0 {
  br label %1

1:                                                ; preds = %15, %0
  %.03 = phi i32 [ 0, %0 ], [ %16, %15 ]
  %2 = phi i32 [ 0, %0 ], [ %6, %15 ]
  %.0 = phi i32 [ 0, %0 ], [ %.1, %15 ]
  %3 = icmp slt i32 %.03, 5
  br i1 %3, label %4, label %17

4:                                                ; preds = %1
  br label %5

5:                                                ; preds = %12, %4
  %.04 = phi i32 [ 0, %4 ], [ %13, %12 ]
  %6 = phi i32 [ %2, %4 ], [ %11, %12 ]
  %.1 = phi i32 [ %.0, %4 ], [ %9, %12 ]
  %7 = icmp slt i32 %.04, 5
  br i1 %7, label %8, label %14

8:                                                ; preds = %5
  %9 = add nsw i32 %.1, 1
  br label %10

10:                                               ; preds = %8
  %11 = add nsw i32 %6, 1
  br label %12

12:                                               ; preds = %10
  %13 = add nsw i32 %.04, 1
  br label %5, !llvm.loop !6

14:                                               ; preds = %5
  br label %15

15:                                               ; preds = %14
  %16 = add nsw i32 %.03, 1
  br label %1, !llvm.loop !8

17:                                               ; preds = %1
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
