; ModuleID = 'es6m2r.bc'
source_filename = "es6.c"
target datalayout = "e-m:e-p270:32:32-p271:32:32-p272:64:64-i64:64-i128:128-f80:128-n8:16:32:64-S128"
target triple = "x86_64-unknown-linux-gnu"

; Function Attrs: noinline nounwind uwtable
define dso_local void @prova() #0 {
  %1 = alloca [20 x i32], align 16
  %2 = alloca [20 x i32], align 16
  %3 = getelementptr inbounds [20 x i32], ptr %1, i64 0, i64 0
  store i32 0, ptr %3, align 16
  %4 = getelementptr inbounds [20 x i32], ptr %2, i64 0, i64 0
  store i32 0, ptr %4, align 16
  br label %5

5:                                                ; preds = %10, %0
  %.01 = phi i32 [ 1, %0 ], [ %11, %10 ]
  %6 = icmp slt i32 %.01, 20
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

13:                                               ; preds = %23, %12
  %.0 = phi i32 [ 1, %12 ], [ %24, %23 ]
  %14 = icmp slt i32 %.0, 20
  br i1 %14, label %15, label %25

15:                                               ; preds = %13
  %16 = sub nsw i32 %.0, 1
  %17 = sext i32 %16 to i64
  %18 = getelementptr inbounds [20 x i32], ptr %2, i64 0, i64 %17
  %19 = load i32, ptr %18, align 4
  %20 = add nsw i32 %19, 1
  %21 = sext i32 %.0 to i64
  %22 = getelementptr inbounds [20 x i32], ptr %2, i64 0, i64 %21
  store i32 %20, ptr %22, align 4
  br label %23

23:                                               ; preds = %15
  %24 = add nsw i32 %.0, 1
  br label %13, !llvm.loop !8

25:                                               ; preds = %13
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
