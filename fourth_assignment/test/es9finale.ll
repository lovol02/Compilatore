; ModuleID = 'es9finale.bc'
source_filename = "es9.c"
target datalayout = "e-m:e-p270:32:32-p271:32:32-p272:64:64-i64:64-i128:128-f80:128-n8:16:32:64-S128"
target triple = "x86_64-unknown-linux-gnu"

; Function Attrs: noinline nounwind uwtable
define dso_local void @prova() #0 {
  %1 = alloca [20 x i32], align 16
  %2 = alloca [20 x i32], align 16
  br label %3

3:                                                ; preds = %15, %0
  %.0 = phi i32 [ 0, %0 ], [ %6, %15 ]
  %4 = sext i32 %.0 to i64
  %5 = getelementptr inbounds [20 x i32], ptr %1, i64 0, i64 %4
  store i32 %.0, ptr %5, align 4
  %6 = add nsw i32 %.0, 1
  br label %7

7:                                                ; preds = %3, %17
  %8 = sext i32 %.0 to i64
  %9 = getelementptr inbounds [20 x i32], ptr %1, i64 0, i64 %8
  %10 = load i32, ptr %9, align 4
  %11 = add nsw i32 %10, 1
  %12 = sext i32 %.0 to i64
  %13 = getelementptr inbounds [20 x i32], ptr %2, i64 0, i64 %12
  store i32 %11, ptr %13, align 4
  %14 = add nsw i32 %.0, 1
  br label %15

15:                                               ; preds = %7
  %16 = icmp slt i32 %6, 5
  br i1 %16, label %3, label %18, !llvm.loop !6

17:                                               ; No predecessors!
  br label %7

18:                                               ; preds = %15
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
