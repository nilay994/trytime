#ifndef INCLUDE_RANSAC_H_
#define INCLUDE_RANSAC_H_


#define RANSAC_BUF_SIZE 150
#define RANSAC_ITERATIONS 100
#define RANSAC_A_WIDTH 6
#define RANSAC_MIN_SAMPLES_FOR_FIT 10

// job
void* ransac_job(void* data);

// control class
class Ransac {
    public:

	int buffer_cnt = 0;
	int ransac_cnt;
	volatile bool new_measurement = false;
	double prev_flushing_time = 0.0;	

	Ransac();
	~Ransac();

	void reset();
	void get_indices_without_replacement(int *indices_subset, int n_samples, int count);
	void ransac_correct();
};

#endif /* INCLUDE_RANSAC_H_ */
