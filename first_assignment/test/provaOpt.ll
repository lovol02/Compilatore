; ModuleID = 'provam2r.ll'
source_filename = "prova.c"
target datalayout = "e-m:e-p270:32:32-p271:32:32-p272:64:64-i64:64-i128:128-f80:128-n8:16:32:64-S128"
target triple = "x86_64-unknown-linux-gnu"

; Function Attrs: noinline nounwind uwtable
define dso_local void @prova1(i32 noundef %0) #0 {
  %2 = add nsw i32 %0, 1
  %3 = sub nsw i32 %0, 0
  %4 = add nsw i32 %3, 1
  ret void
}

; Function Attrs: noinline nounwind uwtable
define dso_local void @prova2(i32 noundef %0) #0 {
  %2 = shl i32 %0, 1
  %3 = sdiv i32 %0, 1
  %4 = mul nsw i32 %3, 2
  ret void
}

; Function Attrs: noinline nounwind uwtable
define dso_local void @prova3(i32 noundef %0) #0 {
  %2 = shl i32 %0, 4
  %3 = sub i32 %2, %0
  %4 = shl i32 %0, 4
  %5 = sub i32 %4, %0
  ret void
}

; Function Attrs: noinline nounwind uwtable
define dso_local void @prova4(i32 noundef %0) #0 {
  %2 = lshr i32 %0, 3
  ret void
}

; Function Attrs: noinline nounwind uwtable
define dso_local void @prova5(i32 noundef %0) #0 {
  %2 = add nsw i32 %0, 1
  %3 = add nsw i32 %0, 2
  ret void
}

; Function Attrs: noinline nounwind uwtable
define dso_local void @prova6(i32 noundef %0) #0 {
  %2 = add nsw i32 %0, 1
  %3 = add nsw i32 %0, 1
  ret void
}

; Function Attrs: noinline nounwind uwtable
define dso_local void @prova7(i32 noundef %0) #0 {
  %2 = shl i32 %0, 2
  %3 = sdiv i32 %0, 3
  %4 = sdiv i32 %2, 4
  %5 = add nsw i32 %4, 2
  %6 = add nsw i32 %0, 3
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
