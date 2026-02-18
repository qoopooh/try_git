#include <ATen/ATen.h>
#include <iostream>

int main() {
    // Set the global seed for reproducibility
    at::manual_seed(42);

// If using CUDA, also set CUDA seed
#ifdef USE_CUDA
    at::cuda::manual_seed(42);
    at::cuda::manual_seed_all(42);  // For multi-GPU
#endif

    at::Tensor a = at::ones({3, 4}, at::kFloat);
    at::Tensor b = at::randn({4, 5}, at::kFloat);

    at::Tensor c = a.matmul(b);  // matrix multiplicatino
    at::Tensor d = c.relu();     // ReLU activation
    at::Tensor e = d.mean();     // scalar mean

    std::cout << "Result: " << e << std::endl;
    return 0;
}
