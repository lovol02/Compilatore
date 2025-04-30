void example(int* A, int B, int N) {
    int x = B + 5; // loop-invariant

    for (int i = 0; i < N; i++) {
        A[i] = x * 2; // x is constant during loop
    }
}