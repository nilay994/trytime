#include <iostream>
#include <stdint.h>
#include <queue>
#include <mutex>
#include <thread>
#include <semaphore.h>

typedef struct {
    uint8_t img[256][256];
} image_t;

class Img_BoundedBuffer {
    private:
        std::queue<image_t> bb;
        sem_t s_used; // number of used places
        sem_t s_empty; // number of empty places
        std::mutex mut;

    public:
        Img_BoundedBuffer()
        {
            sem_init(&s_used, 0, 0);
            sem_init(&s_empty, 0, 20);
        }

        void write(image_t * img)
        {
            // if a few empty places exist,
            sem_wait(&s_empty);
            std::cout << "inside write" << std::endl;
            std::lock_guard<std::mutex> lock(mut);
            bb.push(*img);
            // fill up an extra slot
            sem_post(&s_used);
        }

        void read(image_t * img)
        {
            sem_wait(&s_used);
            std::cout << "inside read" << std::endl;
            std::lock_guard<std::mutex> lock(mut);
            *img = bb.front();
            bb.pop();
            sem_post(&s_empty);
        }
};


void consumer(Img_BoundedBuffer * Img_Rb)
{
    std::cout << "consumer spawned" << std::endl;
    FILE * fp = fopen("consumer.csv", "w+");
    image_t img_stream;
    while(1) {
        // Img_Rb->display_status();
        Img_Rb->read(&img_stream);
        printf("%d\n", img_stream.img[0][0]);
        fprintf(fp, "%d\n", img_stream.img[0][0]);
        fflush(fp);
        std::this_thread::sleep_for(std::chrono::milliseconds(300));
    }
    fclose(fp);
}

void producer(Img_BoundedBuffer * Img_Rb)
{
    std::cout << "producer spawned" << std::endl;
    int i;
    image_t camera_img;
    while (1) {
        camera_img.img[0][0] = i;

        Img_Rb->write(&camera_img);

        i = (i + 1) % 256;
        std::this_thread::sleep_for(std::chrono::milliseconds(500));
    }
}

int main()
{
    Img_BoundedBuffer Img_Bb;

    std::thread consumer_thread(consumer, &Img_Bb);
    std::thread producer_thread(producer, &Img_Bb);

    consumer_thread.join();
    producer_thread.join();
    return 0;
}
