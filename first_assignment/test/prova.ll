; ModuleID = 'prova.c'
source_filename = "prova.c"
target datalayout = "e-m:e-p270:32:32-p271:32:32-p272:64:64-i64:64-i128:128-f80:128-n8:16:32:64-S128"
target triple = "x86_64-unknown-linux-gnu"

; Function Attrs: noinline nounwind uwtable
define dso_local void @prova1(i32 noundef %0) #0 {
  %2 = alloca i32, align 4
  %3 = alloca i32, align 4
  %4 = alloca i32, align 4
  %5 = alloca i32, align 4
  %6 = alloca i32, align 4
  store i32 %0, ptr %2, align 4
  %7 = load i32, ptr %2, align 4
  %8 = add nsw i32 %7, 0
  store i32 %8, ptr %3, align 4
  %9 = load i32, ptr %3, align 4
  %10 = add nsw i32 %9, 1
  store i32 %10, ptr %4, align 4
  %11 = load i32, ptr %2, align 4
  %12 = sub nsw i32 %11, 0
  store i32 %12, ptr %5, align 4
  %13 = load i32, ptr %5, align 4
  %14 = add nsw i32 %13, 1
  store i32 %14, ptr %6, align 4
  ret void
}

; Function Attrs: noinline nounwind uwtable
define dso_local void @prova2(i32 noundef %0) #0 {
  %2 = alloca i32, align 4
  %3 = alloca i32, align 4
  %4 = alloca i32, align 4
  %5 = alloca i32, align 4
  %6 = alloca i32, align 4
  %7 = alloca i32, align 4
  store i32 %0, ptr %2, align 4
  %8 = load i32, ptr %2, align 4
  %9 = mul nsw i32 %8, 1
  store i32 %9, ptr %3, align 4
  %10 = load i32, ptr %2, align 4
  %11 = mul nsw i32 1, %10
  store i32 %11, ptr %4, align 4
  %12 = load i32, ptr %3, align 4
  %13 = mul nsw i32 %12, 2
  store i32 %13, ptr %5, align 4
  %14 = load i32, ptr %2, align 4
  %15 = sdiv i32 %14, 1
  store i32 %15, ptr %6, align 4
  %16 = load i32, ptr %6, align 4
  %17 = mul nsw i32 %16, 2
  store i32 %17, ptr %7, align 4
  ret void
}

; Function Attrs: noinline nounwind uwtable
define dso_local void @prova3(i32 noundef %0) #0 {
  %2 = alloca i32, align 4
  %3 = alloca i32, align 4
  %4 = alloca i32, align 4
  store i32 %0, ptr %2, align 4
  %5 = load i32, ptr %2, align 4
  %6 = mul nsw i32 %5, 15
  store i32 %6, ptr %3, align 4
  %7 = load i32, ptr %2, align 4
  %8 = mul nsw i32 15, %7
  store i32 %8, ptr %4, align 4
  ret void
}

; Function Attrs: noinline nounwind uwtable
define dso_local void @prova4(i32 noundef %0) #0 {
  %2 = alloca i32, align 4
  %3 = alloca i32, align 4
  store i32 %0, ptr %2, align 4
  %4 = load i32, ptr %2, align 4
  %5 = sdiv i32 %4, 8
  store i32 %5, ptr %3, align 4
  ret void
}

; Function Attrs: noinline nounwind uwtable
define dso_local void @prova5(i32 noundef %0) #0 {
  %2 = alloca i32, align 4
  %3 = alloca i32, align 4
  %4 = alloca i32, align 4
  %5 = alloca i32, align 4
  store i32 %0, ptr %2, align 4
  %6 = load i32, ptr %2, align 4
  %7 = add nsw i32 %6, 1
  store i32 %7, ptr %3, align 4
  %8 = load i32, ptr %2, align 4
  %9 = add nsw i32 %8, 2
  store i32 %9, ptr %4, align 4
  %10 = load i32, ptr %3, align 4
  %11 = sub nsw i32 %10, 1
  store i32 %11, ptr %5, align 4
  ret void
}

; Function Attrs: noinline nounwind uwtable
define dso_local void @prova6(i32 noundef %0) #0 {
  %2 = alloca i32, align 4
  %3 = alloca i32, align 4
  %4 = alloca i32, align 4
  %5 = alloca i32, align 4
  store i32 %0, ptr %2, align 4
  %6 = load i32, ptr %2, align 4
  %7 = add nsw i32 %6, 1
  store i32 %7, ptr %3, align 4
  %8 = load i32, ptr %3, align 4
  %9 = sub nsw i32 %8, 1
  store i32 %9, ptr %4, align 4
  %10 = load i32, ptr %4, align 4
  %11 = add nsw i32 %10, 1
  store i32 %11, ptr %5, align 4
  ret void
}

; Function Attrs: noinline nounwind uwtable
define dso_local void @prova7(i32 noundef %0) #0 {
  %2 = alloca i32, align 4
  %3 = alloca i32, align 4
  %4 = alloca i32, align 4
  %5 = alloca i32, align 4
  %6 = alloca i32, align 4
  %7 = alloca i32, align 4
  %8 = alloca i32, align 4
  store i32 %0, ptr %2, align 4
  %9 = load i32, ptr %2, align 4
  %10 = mul nsw i32 %9, 4
  store i32 %10, ptr %3, align 4
  %11 = load i32, ptr %2, align 4
  %12 = sdiv i32 %11, 3
  store i32 %12, ptr %4, align 4
  %13 = load i32, ptr %3, align 4
  %14 = sdiv i32 %13, 4
  store i32 %14, ptr %5, align 4
  %15 = load i32, ptr %4, align 4
  %16 = mul nsw i32 %15, 3
  store i32 %16, ptr %6, align 4
  %17 = load i32, ptr %5, align 4
  %18 = add nsw i32 %17, 2
  store i32 %18, ptr %7, align 4
  %19 = load i32, ptr %6, align 4
  %20 = add nsw i32 %19, 3
  store i32 %20, ptr %8, align 4
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
