; ModuleID = 'es7m2r.bc'
source_filename = "es7.c"
target datalayout = "e-m:e-p270:32:32-p271:32:32-p272:64:64-i64:64-i128:128-f80:128-n8:16:32:64-S128"
target triple = "x86_64-unknown-linux-gnu"

; Function Attrs: noinline nounwind uwtable
define dso_local void @prova() #0 {
  %1 = alloca [20 x i32], align 16
  %2 = alloca [20 x i32], align 16
  %3 = icmp sgt i32 100, 0
  br i1 %3, label %4, label %13

4:                                                ; preds = %0
  br label %5

5:                                                ; preds = %10, %4
  %.01 = phi i32 [ 0, %4 ], [ %11, %10 ]
  %6 = icmp slt i32 %.01, 5
  br i1 %6, label %7, label %12

7:                                                ; preds = %5
  %8 = sext i32 %.01 to i64
  %9 = getelementptr inbounds [20 x i32], ptr %1, i64 0, i64 %8
  store i32 %.01, ptr %9, align 4
  br label %10

10:                                               ; preds = %7
  %11 = add nsw i32 %.01, 1
  br label %5, !llvm.loop !6

12:                                               ; preds = %5
  br label %13

13:                                               ; preds = %12, %0
  %14 = icmp sgt i32 100, 0
  br i1 %14, label %15, label %29

15:                                               ; preds = %13
  br label %16

16:                                               ; preds = %26, %15
  %.0 = phi i32 [ 0, %15 ], [ %27, %26 ]
  %17 = icmp slt i32 %.0, 5
  br i1 %17, label %18, label %28

18:                                               ; preds = %16
  %19 = add nsw i32 %.0, 2
  %20 = sext i32 %19 to i64
  %21 = getelementptr inbounds [20 x i32], ptr %1, i64 0, i64 %20
  %22 = load i32, ptr %21, align 4
  %23 = add nsw i32 %22, 1
  %24 = sext i32 %.0 to i64
  %25 = getelementptr inbounds [20 x i32], ptr %2, i64 0, i64 %24
  store i32 %23, ptr %25, align 4
  br label %26

26:                                               ; preds = %18
  %27 = add nsw i32 %.0, 1
  br label %16, !llvm.loop !8

28:                                               ; preds = %16
  br label %29

29:                                               ; preds = %28, %13
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
