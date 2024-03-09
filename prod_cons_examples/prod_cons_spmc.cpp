#include <iostream>
#include <thread>
#include <atomic>
#include <condition_variable>
#include <mutex>
#include <queue>

// not working

class BoundedBuffer {
private:
    std::queue<int> q;
    std::mutex bufferMutex;
    std::condition_variable bufferNotEmpty;
    std::atomic<bool> producerDone{false};

public:
    bool write(int val) {
        std::unique_lock<std::mutex> lock(bufferMutex);
        q.push(val);
        bufferNotEmpty.notify_one();
        return true;
    }

    bool read(int& val) {
        std::unique_lock<std::mutex> lock(bufferMutex);
        bufferNotEmpty.wait(lock, [this] { return !q.empty() || producerDone.load(); });

        if (!q.empty()) {
            val = q.front();
            q.pop();
            return true;
        }
        return false;
    }

    void signalProducerDone() {
        producerDone.store(true);
        bufferNotEmpty.notify_all();
    }
};

void producer(BoundedBuffer& buffer) {
    for (int i = 1; i <= 255; ++i) {
        buffer.write(i);
        std::this_thread::sleep_for(std::chrono::milliseconds(50)); // Simulating some work
    }

    // Signal that the producer is done
    buffer.signalProducerDone();
}

void consumer(BoundedBuffer& buffer, int id) {
    int val;
    while (buffer.read(val)) {
        std::cout << "Consumer " << id << " consumed: " << val << std::endl;
        std::this_thread::sleep_for(std::chrono::milliseconds(100)); // Simulating some work
    }
}

int main() {
    BoundedBuffer buffer;

    std::thread producerThread(producer, std::ref(buffer));
    std::thread consumerThread1(consumer, std::ref(buffer), 1);
    std::thread consumerThread2(consumer, std::ref(buffer), 2);

    producerThread.join();
    consumerThread1.join();
    consumerThread2.join();

    return 0;
}
