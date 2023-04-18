#include <iostream>
#include <thread>
#include <vector>

template <typename T, typename R>
std::vector<R> SplitWork(const std::vector<T>& input, R (*func)(T),
                         int threshold) {
  if (input.size() <= threshold) {
    std::vector<R> result(input.size());
    for (size_t i = 0; i < input.size(); ++i) result[i] = func(input[i]);
    return result;
  } else {
    std::vector<R> result(input.size());
    size_t midpoint = input.size() / 2;
    std::vector<T> first_half(input.begin(), input.begin() + midpoint);
    std::vector<T> second_half(input.begin() + midpoint, input.end());

    std::thread t1([&result, &first_half, func]() {
      for (size_t i = 0; i < first_half.size(); i++)
        result[i] = func(first_half[i]);
    });

    std::thread t2([&result, &second_half, midpoint, func]() {
      for (size_t i = 0; i < second_half.size(); i++)
        result[i + midpoint] = func(second_half[i]);
    });

    t1.join();
    t2.join();

    return result;
  }
}

int main() {
  // Example usage
  int threshold = 10;
  std::vector<int> input{1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12, 13, 14, 15};
  std::vector<double> result = SplitWork<int, double>(
      input, [](int x) { return x * 1.5; }, threshold);

  for (int i = 0; i < result.size(); i++) {
    std::cout << result[i] << '\n';
  }

  return 0;
}
