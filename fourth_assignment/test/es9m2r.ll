; ModuleID = 'es9m2r.bc'
source_filename = "es9.c"
target datalayout = "e-m:e-p270:32:32-p271:32:32-p272:64:64-i64:64-i128:128-f80:128-n8:16:32:64-S128"
target triple = "x86_64-unknown-linux-gnu"

; Function Attrs: noinline nounwind uwtable
define dso_local void @prova() #0 {
  %1 = alloca [20 x i32], align 16
  %2 = alloca [20 x i32], align 16
  br label %3

3:                                                ; preds = %7, %0
  %.0 = phi i32 [ 0, %0 ], [ %6, %7 ]
  %4 = sext i32 %.0 to i64
  %5 = getelementptr inbounds [20 x i32], ptr %1, i64 0, i64 %4
  store i32 %.0, ptr %5, align 4
  %6 = add nsw i32 %.0, 1
  br label %7

7:                                                ; preds = %3
  %8 = icmp slt i32 %6, 5
  br i1 %8, label %3, label %9, !llvm.loop !6

9:                                                ; preds = %7
  br label %10

10:                                               ; preds = %18, %9
  %.1 = phi i32 [ 0, %9 ], [ %17, %18 ]
  %11 = sext i32 %.1 to i64
  %12 = getelementptr inbounds [20 x i32], ptr %1, i64 0, i64 %11
  %13 = load i32, ptr %12, align 4
  %14 = add nsw i32 %13, 1
  %15 = sext i32 %.1 to i64
  %16 = getelementptr inbounds [20 x i32], ptr %2, i64 0, i64 %15
  store i32 %14, ptr %16, align 4
  %17 = add nsw i32 %.1, 1
  br label %18

18:                                               ; preds = %10
  %19 = icmp slt i32 %17, 5
  br i1 %19, label %10, label %20, !llvm.loop !8

20:                                               ; preds = %18
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
