#include <TeleportList.hpp>
#include <UnrolledTeleportList.hpp>

#include <algorithm>
#include <cassert>
#include <chrono>
#include <cstdlib>
#include <iostream>
#include <set>
#include <vector>

template <class SetType, typename FindFunc>
void benchmark(SetType& set, const std::vector<int>& nums, FindFunc func)
{
    set.clear();
    auto now = std::chrono::high_resolution_clock::now();
    for (auto&& v : nums) {
        set.insert(v);
    }
    auto end      = std::chrono::high_resolution_clock::now();
    auto duration = std::chrono::duration_cast<std::chrono::milliseconds>(end - now);
    std::cout << "Time for " << nums.size() << " inserts: " << duration.count() << " ms\n";

    now = std::chrono::high_resolution_clock::now();
    for (auto&& v : nums) {
        if (!func(set, v)) {
            assert(false);
        }
    }
    end      = std::chrono::high_resolution_clock::now();
    duration = std::chrono::duration_cast<std::chrono::milliseconds>(end - now);
    std::cout << "Time for " << nums.size() << " finds: " << duration.count() << " ms\n";
}
template <class SetType, typename FindFunc>
void benchmark2(SetType& set, const std::vector<int>& nums, FindFunc func)
{
    set.clear();
    auto now = std::chrono::high_resolution_clock::now();
    for (auto&& v : nums) {
        set.insert(v);
        volatile bool b = func(set, v);
    }
    auto end      = std::chrono::high_resolution_clock::now();
    auto duration = std::chrono::duration_cast<std::chrono::milliseconds>(end - now);
    std::cout << "Time for " << nums.size() << " inserts and finds: " << duration.count()
              << " ms\n";
}
template <class SetType, typename FindFunc, typename RemFunc>
void benchmark3(SetType& set, const std::vector<int>& nums, FindFunc findFunc, RemFunc remFunc)
{
    set.clear();
    auto now = std::chrono::high_resolution_clock::now();
    for (auto&& v : nums) {
        set.insert(v);
    }
    auto end      = std::chrono::high_resolution_clock::now();
    auto duration = std::chrono::duration_cast<std::chrono::milliseconds>(end - now);
    std::cout << "Time for " << nums.size() << " inserts: " << duration.count() << " ms\n";

    now = std::chrono::high_resolution_clock::now();
    for (auto&& v : nums) {
        if (!findFunc(set, v)) {
            assert(false);
        }
    }
    end      = std::chrono::high_resolution_clock::now();
    duration = std::chrono::duration_cast<std::chrono::milliseconds>(end - now);
    std::cout << "Time for " << nums.size() << " finds: " << duration.count() << " ms\n";
    now = std::chrono::high_resolution_clock::now();
    for (auto&& v : nums) {
        if (!remFunc(set, v)) {
            assert(false);
        }
    }
    end      = std::chrono::high_resolution_clock::now();
    duration = std::chrono::duration_cast<std::chrono::milliseconds>(end - now);
    std::cout << "Time for " << nums.size() << " removes: " << duration.count() << " ms\n";
    assert(set.size() == 0);
}

bool stdSetFind(const std::set<int>& set, const int& v) { return set.find(v) != set.end(); }
bool telepFind(TeleportList<int>& set, const int& v) { return set.find(v); }
bool unrolledTelepFind(UnrolledTeleportList<int>& set, const int& v) { return set.find(v); }

bool stdSetRemove(std::set<int>& set, const int& v) { return set.erase(v) == 1; }
bool unrolledTelepRemove(UnrolledTeleportList<int>& set, const int& v) { return set.remove(v); }

void testTelep()
{
    TeleportList<int> set;
    size_t numInserts = 100;
    for (int i = 0; i < numInserts; ++i) {
        set.insert(rand() % numInserts);
        set.find(rand() % numInserts);
    }
    set.printAll();
}

void testUnrolledTelep()
{
    UnrolledTeleportList<int> set;
    size_t numInserts = 100;
    size_t removed    = 0;
    for (int i = 0; i < numInserts; ++i) {
        set.insert(rand() % numInserts);
        set.find(rand() % numInserts);
    }
    set.printAll();
    for (int i = 0; i < numInserts; ++i) {
        removed += set.remove(rand() % numInserts);
    }
    set.printAll();
    std::cout << set.size() << std::endl;
    set.clear();
    // set.printAll();
}

int main()
{
    // testUnrolledTelep();
    TeleportList<int> telep;
    UnrolledTeleportList<int> utelep;
    std::set<int> stdSet;
    std::vector<int> nums;
    size_t numInserts = 1000000;
    for (int i = 0; i < numInserts; ++i) {
        nums.push_back(i);
    }
    std::random_shuffle(nums.begin(), nums.end());
    std::cout << "Random inserts (std::set):\n";
    benchmark(stdSet, nums, &stdSetFind);
    std::cout << "Random inserts (TeleportList):\n";
    benchmark(telep, nums, &telepFind);
    std::cout << "Random inserts (UnrolledTeleportList):\n";
    benchmark(utelep, nums, &unrolledTelepFind);

    std::cout << "\nRandom inserts/finds (std::set):\n";
    benchmark2(stdSet, nums, &stdSetFind);
    std::cout << "Random inserts/finds (TeleportList):\n";
    benchmark2(telep, nums, &telepFind);
    std::cout << "Random inserts/finds (UnrolledTeleportList):\n";
    benchmark2(utelep, nums, &unrolledTelepFind);

    nums.clear();
    for (int i = 0; i < numInserts; ++i) {
        nums.push_back(i);
    }
    std::cout << "\nSequential inserts (std::set):\n";
    benchmark(stdSet, nums, &stdSetFind);
    std::cout << "Sequential inserts (TeleportList):\n";
    benchmark(telep, nums, &telepFind);
    std::cout << "Sequential inserts (UnrolledTeleportList):\n";
    benchmark(utelep, nums, &unrolledTelepFind);

    std::reverse(nums.begin(), nums.end());
    std::cout << "\nSequential inserts (reverse) (std::set):\n";
    benchmark(stdSet, nums, &stdSetFind);
    std::cout << "Sequential inserts (reverse) (TeleportList):\n";
    benchmark(telep, nums, &telepFind);
    std::cout << "Sequential inserts (reverse) (UnrolledTeleportList):\n";
    benchmark(utelep, nums, &unrolledTelepFind);

    std::cout << "\nSequential inserts/finds (reverse) (std::set):\n";
    benchmark2(stdSet, nums, &stdSetFind);
    std::cout << "Sequential inserts/finds (reverse) (TeleportList):\n";
    benchmark2(telep, nums, &telepFind);
    std::cout << "Sequential inserts/finds (reverse) (UnrolledTeleportList):\n";
    benchmark2(utelep, nums, &unrolledTelepFind);

    std::random_shuffle(nums.begin(), nums.end());
    std::cout << "\nRandom inserts (std::set):\n";
    benchmark3(stdSet, nums, &stdSetFind, &stdSetRemove);
    std::cout << "Random inserts (UnrolledTeleportList):\n";
    benchmark3(utelep, nums, &unrolledTelepFind, &unrolledTelepRemove);
}