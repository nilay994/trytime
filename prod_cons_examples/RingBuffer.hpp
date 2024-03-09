#include <iostream>
#include <mutex>

/**
 * ******************** Note: The choice of using templates **********************
 *
 * Another way to declare the templated class would be to just say template<class T>
 * Then the length of the RingBuffer could be an argument to the constructor, but then we'd have
 * to malloc the buffer and use up the unpredictable heap.
 * Hence declaring as template<class T, int N> would be better, stays in the stack.
 *
 * Another way to stay in the stack would be to let the application allocate the buffer and the
 * RingBuffer class gets a pointer to the start of the buffer.
 *
 * However, later it would be found out that there is one more tradeoff to using templates.
 *
 * Right now, the compilation step for this RingBuffer utility happens at compile time. However,
 * the compiler only comes across its real data type while compiling the applications using the
 * RingBuffer utility, and somehow that is too late. Ideally, the compiler should know what data
 * types it needs to template for before this step. One way to do so is to forward declare
 * the templated class with the anticipated data type inside the RingBuffer.cpp source file.
 *
 * This for several reasons, is not good software architecture. However C++ might know this pain,
 * and hence allowed adding function definitions of such classes directly inside RingBuffer.hpp.
 * By doing so, while compiling main.cpp, the compiler can find the template data type declaration
 * inside its file/function scope, and untemplate for that particular data type since the
 * definitions lie in the header of main.cpp.
 *
 * Hence, inside NTNU MSP map of the drone code and also the RingBuffer example of Embedded
 * Artistry, both avoid use of a separate RingBuffer.cpp and they declare and define everything
 * inside RingBuffer.hpp.
 */

template<class T, int N> class RingBuffer {
    private:
        T rb[N];
        int head = 0;
        int tail = 0;
        std::mutex mut; // std::mutex from <mutex>

        // Adding the below flag changes everything, the users of the RingBuffer might
        // have to acquire a lock before reading or writing
        bool full = false;

    public:
        RingBuffer();
        bool read(T * item);
        void write(T item);
        void clear();
        int get_size();
        bool is_full();
        bool is_empty();
        void display_status();
};

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
// template class RingBuffer<uint8_t, 20>;