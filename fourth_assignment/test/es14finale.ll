; ModuleID = 'es14finale.bc'
source_filename = "es14.c"
target datalayout = "e-m:e-p270:32:32-p271:32:32-p272:64:64-i64:64-i128:128-f80:128-n8:16:32:64-S128"
target triple = "x86_64-unknown-linux-gnu"

; Function Attrs: noinline nounwind uwtable
define dso_local void @prova() #0 {
  %1 = alloca [20 x i32], align 16
  %2 = alloca [20 x i32], align 16
  %3 = getelementptr inbounds [20 x i32], ptr %1, i64 0, i64 0
  store i32 0, ptr %3, align 16
  br label %4

4:                                                ; preds = %16, %0
  %.01 = phi i32 [ 0, %0 ], [ %17, %16 ]
  %5 = icmp slt i32 %.01, 20
  br i1 %5, label %6, label %18

6:                                                ; preds = %4
  %7 = sext i32 %.01 to i64
  %8 = getelementptr inbounds [20 x i32], ptr %1, i64 0, i64 %7
  %9 = load i32, ptr %8, align 4
  %10 = sext i32 %.01 to i64
  %11 = getelementptr inbounds [20 x i32], ptr %2, i64 0, i64 %10
  store i32 %9, ptr %11, align 4
  br label %12

12:                                               ; preds = %6
  %13 = add nsw i32 %.01, 1
  %14 = sext i32 %13 to i64
  %15 = getelementptr inbounds [20 x i32], ptr %1, i64 0, i64 %14
  store i32 %.01, ptr %15, align 4
  br label %16

16:                                               ; preds = %12
  %17 = add nsw i32 %.01, 1
  br label %4, !llvm.loop !6

18:                                               ; preds = %4
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
