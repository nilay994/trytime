#pragma once
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
