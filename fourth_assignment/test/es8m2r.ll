; ModuleID = 'es8m2r.bc'
source_filename = "es8.c"
target datalayout = "e-m:e-p270:32:32-p271:32:32-p272:64:64-i64:64-i128:128-f80:128-n8:16:32:64-S128"
target triple = "x86_64-unknown-linux-gnu"

; Function Attrs: noinline nounwind uwtable
define dso_local void @prova() #0 {
  %1 = alloca [20 x i32], align 16
  %2 = alloca [20 x i32], align 16
  %3 = icmp sgt i32 5, 0
  br i1 %3, label %4, label %12

4:                                                ; preds = %0
  br label %5

5:                                                ; preds = %9, %4
  %.0 = phi i32 [ 0, %4 ], [ %8, %9 ]
  %6 = sext i32 %.0 to i64
  %7 = getelementptr inbounds [20 x i32], ptr %1, i64 0, i64 %6
  store i32 %.0, ptr %7, align 4
  %8 = add nsw i32 %.0, 1
  br label %9

9:                                                ; preds = %5
  %10 = icmp slt i32 %8, 5
  br i1 %10, label %5, label %11, !llvm.loop !6

11:                                               ; preds = %9
  br label %12

12:                                               ; preds = %11, %0
  %13 = icmp sgt i32 5, 0
  br i1 %13, label %14, label %26

14:                                               ; preds = %12
  br label %15

15:                                               ; preds = %23, %14
  %.1 = phi i32 [ 0, %14 ], [ %22, %23 ]
  %16 = sext i32 %.1 to i64
  %17 = getelementptr inbounds [20 x i32], ptr %1, i64 0, i64 %16
  %18 = load i32, ptr %17, align 4
  %19 = add nsw i32 %18, 1
  %20 = sext i32 %.1 to i64
  %21 = getelementptr inbounds [20 x i32], ptr %2, i64 0, i64 %20
  store i32 %19, ptr %21, align 4
  %22 = add nsw i32 %.1, 1
  br label %23

23:                                               ; preds = %15
  %24 = icmp slt i32 %22, 5
  br i1 %24, label %15, label %25, !llvm.loop !8

25:                                               ; preds = %23
  br label %26

26:                                               ; preds = %25, %12
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
