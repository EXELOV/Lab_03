#include <iostream>
#include <vector>
#include <chrono>
#include <random>
#include <algorithm>
#include <utility>

// Сортировка слиянием
void merge(std::vector<int>& arr, int left, int mid, int right) {
    int n1 = mid - left + 1;
    int n2 = right - mid;
    std::vector<int> L(n1), R(n2);
    for (int i = 0; i < n1; i++) L[i] = arr[left + i];
    for (int j = 0; j < n2; j++) R[j] = arr[mid + 1 + j];
    int i = 0, j = 0, k = left;
    while (i < n1 && j < n2) {
        if (L[i] <= R[j]) arr[k++] = L[i++];
        else arr[k++] = R[j++];
    }
    while (i < n1) arr[k++] = L[i++];
    while (j < n2) arr[k++] = R[j++];
}

void mergeSort(std::vector<int>& arr, int left, int right) {
    if (left < right) {
        int mid = left + (right - left) / 2;
        mergeSort(arr, left, mid);
        mergeSort(arr, mid + 1, right);
        merge(arr, left, mid, right);
    }
}

// Поразрядная сортировка
void countingSortForRadix(std::vector<int>& arr, int exp) {
    int n = arr.size();
    if (n == 0) return;
    std::vector<int> output(n);
    int count[10] = { 0 };
    for (int i = 0; i < n; i++) count[(arr[i] / exp) % 10]++;
    for (int i = 1; i < 10; i++) count[i] += count[i - 1];
    for (int i = n - 1; i >= 0; i--) {
        output[count[(arr[i] / exp) % 10] - 1] = arr[i];
        count[(arr[i] / exp) % 10]--;
    }
    for (int i = 0; i < n; i++) arr[i] = output[i];
}

void radixSort(std::vector<int>& arr) {
    if (arr.empty()) return;
    int max_val = *std::max_element(arr.begin(), arr.end());
    for (int exp = 1; max_val / exp > 0; exp *= 10) {
        countingSortForRadix(arr, exp);
    }
}

//  Быстрая сортировка
int partition(std::vector<int>& arr, int low, int high) {
    int pivot = arr[high];
    int i = (low - 1);
    for (int j = low; j < high; j++) {
        if (arr[j] <= pivot) {
            i++;
            std::swap(arr[i], arr[j]);
        }
    }
    std::swap(arr[i + 1], arr[high]);
    return (i + 1);
}

void quickSort(std::vector<int>& arr, int low, int high) {
    if (low < high) {
        int pi = partition(arr, low, high);
        quickSort(arr, low, pi - 1);
        quickSort(arr, pi + 1, high);
    }
}

std::vector<int> generate_random_vector(int size) {
    std::vector<int> vec(size);
    std::random_device rd;
    std::mt19937 gen(rd());
    std::uniform_int_distribution<> distrib(0, 1000000);
    for (int i = 0; i < size; ++i) {
        vec[i] = distrib(gen);
    }
    return vec;
}

int main() {
    setlocale(LC_ALL, "Russian");
    std::vector<int> sizes = { 1000, 10000, 50000, 100000, 500000 };

    printf("%-20s | %-28s | %-31s | %-28s\n", "Размер массива (n)", "Сортировка слиянием (in)", "Поразрядная сортировка (in)", "Быстрая сортировка (in)");
    printf("-------------------------------------------------------------------------------------------------------------------\n");

    for (int size : sizes) {
        std::vector<int> original_data = generate_random_vector(size);
        std::vector<int> data_to_sort;
        double merge_time, radix_time, quick_time;

        // Замер для слияния
        data_to_sort = original_data;
        auto start = std::chrono::high_resolution_clock::now();
        if (!data_to_sort.empty()) mergeSort(data_to_sort, 0, data_to_sort.size() - 1);
        auto end = std::chrono::high_resolution_clock::now();
        std::chrono::duration<double, std::milli> duration = end - start;
        merge_time = duration.count();

        // Замер для поразрядной
        data_to_sort = original_data;
        start = std::chrono::high_resolution_clock::now();
        radixSort(data_to_sort);
        end = std::chrono::high_resolution_clock::now();
        duration = end - start;
        radix_time = duration.count();

        // Замер для Быстрой
        data_to_sort = original_data;
        start = std::chrono::high_resolution_clock::now();
        if (!data_to_sort.empty()) quickSort(data_to_sort, 0, data_to_sort.size() - 1);
        end = std::chrono::high_resolution_clock::now();
        duration = end - start;
        quick_time = duration.count();

        printf("%-20d | %-28.4f | %-31.4f | %-28.4f\n", size, merge_time, radix_time, quick_time);
    }

    return 0;
}