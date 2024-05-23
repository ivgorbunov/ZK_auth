#pragma once

#include <chrono>
#include <gtest/gtest.h>
#include <iostream>
#include <new>
#include <set>
#include <random>

#include "bigint.hpp"


using std::chrono::steady_clock;
using std::chrono::duration_cast;
using std::chrono::milliseconds;
using std::chrono::microseconds;


std::mt19937 test_random(1791791791);
const int RANDOM_TRIES_COUNT = 20;

long long random_value() {
    return (test_random() % static_cast<long long>(1e9 + 7));
}

BigInteger random_bigint(size_t size) {
    string value = "";
    for (size_t i = 0; i < size; ++i) {
        value += '0' + (abs(random_value()) % 10);
    }
    return BigInteger(value);
}

#define CHECK_OPERATOR_ALLOCATIONS(op, max_allocations_count) \
    BigInteger a = 179; \
    BigInteger b = 228; \
    OperatorNewCounter cntr; \
    a op b; \
    ASSERT_LE(cntr.get_counter(), max_allocations_count);

#define TEST_SAME_OPERATOR(testingOp, correctOp) \
    for (int i = 0; i < RANDOM_TRIES_COUNT; ++i) { \
        BigInteger first = random_bigint(100); \
        BigInteger second = random_bigint(100); \
        BigInteger result = first testingOp second; \
        first correctOp second; \
        ASSERT_EQ(first, result); \
    }

#define CHECK_AGREED_WITH_INT(op) \
    for (int i = 0; i < RANDOM_TRIES_COUNT; ++i) { \
        long long a = random_value(); \
        long long b = random_value(); \
        BigInteger abi = a; \
        BigInteger bbi = b; \
        ASSERT_EQ(abi op bbi, a op b); \
    }

class OperatorNewCounter {
  private:
    int counter = 0;
    size_t total_size = 0;
    static std::set<OperatorNewCounter*> instances;
    
    void notify(size_t size) {
        ++counter;
        total_size += size;
    }

  public:
    OperatorNewCounter() {
        instances.insert(this);
    }

    static void notify_all(size_t size) {
        for (auto item : instances) {
            item->notify(size);
        }
    }

    int get_counter() {
        return counter;
    }

    size_t get_total_allocated() {
        return total_size;
    }
    

    ~OperatorNewCounter() {
        instances.erase(this);
    }
};

std::set<OperatorNewCounter*> OperatorNewCounter::instances = std::set<OperatorNewCounter*>();

void* operator new[] (size_t size) {
    OperatorNewCounter::notify_all(size);
    void* p = malloc(size);
    if (p == nullptr) throw std::bad_alloc();
    return p;
}

class Timer {
  private:
    steady_clock::time_point begin;
    steady_clock::time_point end;
  public:
    void start() {
        begin = steady_clock::now();
    }

    void finish() {
        end = steady_clock::now();
    }

    int get_time_milliseconds() {
        return duration_cast<milliseconds>(end - begin).count();
    }

    int get_time_microseconds() {
        return duration_cast<microseconds>(end - begin).count();
    }
};

