; ModuleID = 'es1.bc'
source_filename = "es1.c"
target datalayout = "e-m:e-p270:32:32-p271:32:32-p272:64:64-i64:64-i128:128-f80:128-n8:16:32:64-S128"
target triple = "x86_64-unknown-linux-gnu"

; Function Attrs: noinline nounwind uwtable
define dso_local void @prova() #0 {
  %1 = alloca [20 x i32], align 16
  %2 = alloca [20 x i32], align 16
  br label %3

3:                                                ; preds = %8, %0
  %.01 = phi i32 [ 0, %0 ], [ %9, %8 ]
  %4 = icmp slt i32 %.01, 5
  br i1 %4, label %5, label %10

5:                                                ; preds = %3
  %6 = sext i32 %.01 to i64
  %7 = getelementptr inbounds [20 x i32], ptr %1, i64 0, i64 %6
  store i32 %.01, ptr %7, align 4
  br label %8

8:                                                ; preds = %5
  %9 = add nsw i32 %.01, 1
  br label %3, !llvm.loop !6

10:                                               ; preds = %3
  br label %11

11:                                               ; preds = %20, %10
  %.0 = phi i32 [ 0, %10 ], [ %21, %20 ]
  %12 = icmp slt i32 %.0, 5
  br i1 %12, label %13, label %22

13:                                               ; preds = %11
  %14 = sext i32 %.0 to i64
  %15 = getelementptr inbounds [20 x i32], ptr %1, i64 0, i64 %14
  %16 = load i32, ptr %15, align 4
  %17 = add nsw i32 %16, 1
  %18 = sext i32 %.0 to i64
  %19 = getelementptr inbounds [20 x i32], ptr %2, i64 0, i64 %18
  store i32 %17, ptr %19, align 4
  br label %20

20:                                               ; preds = %13
  %21 = add nsw i32 %.0, 1
  br label %11, !llvm.loop !8

22:                                               ; preds = %11
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
