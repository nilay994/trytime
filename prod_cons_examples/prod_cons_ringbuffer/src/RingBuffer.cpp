#include "RingBuffer.hpp"
#include <iostream>

// constructor
template<class T, int N>
RingBuffer<T, N>::RingBuffer() {
    clear();
};

// consumer
template<class T, int N>
bool RingBuffer<T, N>::read(T * item) {
    // Note: scoped mutex, std::mutex::lock() is rarely used without lock_guard
    std::lock_guard<std::mutex> lock(mut);
    if (is_empty()) return false;
    *item = rb[tail];
    tail = (tail + 1) % N;
    full = false;
    return true;
};

// producer
template<class T, int N>
void RingBuffer<T, N>::write(T item) {
    // Note: scoped mutex, std::mutex::lock() is rarely used without lock_guard
    std::lock_guard<std::mutex> lock(mut);
    if (is_full()) {
        tail = (tail + 1) % N;
    }
    rb[head] = item;
    head = (head + 1) % N;
    if (head == tail) {
        full = true;
    }
}

template<class T, int N>
void RingBuffer<T, N>::clear() {
    // Note: scoped mutex, std::mutex::lock() is rarely used without lock_guard
    std::lock_guard<std::mutex> lock(mut);
    // head = 0;
    // tail = 0;
    head = tail;
    full = false;
}

// size of already populated entries
template<class T, int N>
int RingBuffer<T, N>::get_size() {
    if (head > tail) {
        return (head - tail);
    } else {
        return (head - tail + N);
    }
};

// true if buffer is full
template<class T, int N>
bool RingBuffer<T, N>::is_full() {
    return full;
};

// true if buffer is empty
template<class T, int N>
bool RingBuffer<T, N>::is_empty() {
    return ((head == tail) && !full);
};

template<class T, int N>
void RingBuffer<T, N>::display_status() {
    for (int i = 0; i < N; i++) {
        if (i == head) {
            printf("\033[32m");
        } else if (i== tail) {
            printf("\033[31m");
        } else {
            printf("\033[0m");
        }
        std::cout << (int) rb[i] << "\t";
    }
    std::cout << std::endl;
    std::cout << "h: " << head << "\t" << "t: " << tail << std::endl;
}

/**
 * Below is a nasty forward declaration. Without the line below, you'd get a undefined reference to
 * any methods or objects of the class. Not sure what this phenomenon is called. Why the problem is
 * nasty is because undefined references usually direct you to CMakeLists.txt, where you'd wonder
 * why the linking step cannot bind the references inside RingBuffer.cpp. You'd include it as
 * source, you'd include it as a precompiled static library - and yet, it wouldn't work out.
 *
 * The problem is caused by the fact that main.cpp and RingBuffer.cpp will be compiled separately
 * first. In main.cpp, the compiler will implicitly instantiate the template
 * class RingBuffer<uint8_t, 20> because those particular instantiations are used in main.cpp.
 * The bad news is that the implementations of those member functions are not in main.cpp, nor in
 * any header file included in main.cpp, and therefore the compiler can't include complete versions
 * of those functions in main.o. When compiling RingBuffer.cpp, the compiler won't compile those
 * instantiations either, because there are no implicit or explicit instantiations of
 * RingBuffer<uint8_t, 20> in RingBuffer.cpp etiher. Remember, when compiling RingBuffer.cpp, the
 * compiler has no clue which instantiations will be needed; and we can't expect it to compile for
 * every type in order to ensure this problem never happens!
 * (RingBuffer<int, 4>, RingBuffer<cv::Image, 20>, and so on ...)
 *
 * Reference:
 * https://stackoverflow.com/questions/8752837/undefined-reference-to-template-class-constructor
 *
 * Solution:
 * Forward declaration like how its down below is a possible solution, but the other way is to
 * entirely move these method definitions into the headere file.
 *
 * Refer RingBuffer.hpp for more info
 */
template class RingBuffer<uint8_t, 20>;

/**
 * Usage
    int main()
    {
        // RingBuffer<cv::Mat, 5> opencv image
        RingBuffer<int, 8> rb;
        for (int i = 0; i < 20; i++) {
            cout << "-----Iteration [" << i << "] ------" << endl;
            cout << "is_empty: " << rb.is_empty() << endl;
            cout << "is_full: "  << rb.is_full()  << endl;
            cout << "get_size: " << rb.get_size() << endl;
            cout << "write: "    << rb.write(i, true)   << endl;
        }

        while (!rb.is_empty()) {
            int val;
            cout << "read: " << rb.read(&val) <<  " val: " << val << endl;
        }

        getchar();
        return 0;
    }
*/