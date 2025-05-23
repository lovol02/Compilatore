; ModuleID = 'es8finale.bc'
source_filename = "es8.c"
target datalayout = "e-m:e-p270:32:32-p271:32:32-p272:64:64-i64:64-i128:128-f80:128-n8:16:32:64-S128"
target triple = "x86_64-unknown-linux-gnu"

; Function Attrs: noinline nounwind uwtable
define dso_local void @prova() #0 {
  %1 = alloca [20 x i32], align 16
  %2 = alloca [20 x i32], align 16
  %3 = icmp sgt i32 5, 0
  br i1 %3, label %4, label %21

4:                                                ; preds = %0
  br label %5

5:                                                ; preds = %17, %4
  %.0 = phi i32 [ 0, %4 ], [ %8, %17 ]
  %6 = sext i32 %.0 to i64
  %7 = getelementptr inbounds [20 x i32], ptr %1, i64 0, i64 %6
  store i32 %.0, ptr %7, align 4
  %8 = add nsw i32 %.0, 1
  br label %9

9:                                                ; preds = %5, %19
  %10 = sext i32 %.0 to i64
  %11 = getelementptr inbounds [20 x i32], ptr %1, i64 0, i64 %10
  %12 = load i32, ptr %11, align 4
  %13 = add nsw i32 %12, 1
  %14 = sext i32 %.0 to i64
  %15 = getelementptr inbounds [20 x i32], ptr %2, i64 0, i64 %14
  store i32 %13, ptr %15, align 4
  %16 = add nsw i32 %.0, 1
  br label %17

17:                                               ; preds = %9
  %18 = icmp slt i32 %8, 5
  br i1 %18, label %5, label %20, !llvm.loop !6

19:                                               ; No predecessors!
  br label %9

20:                                               ; preds = %17
  br label %21

21:                                               ; preds = %0, %20
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
