#include <math.h>
#include <eigen3/Eigen/Dense>
#include "ransac.h"

#ifdef LOG
    FILE *ransac_in_f;
    FILE *ransac_out_f;
#endif

int main() {
    // Ransac *d = (Ransac *) data;
    // todo: event trigger
    // if (d->new_measurement) {        
    //     d->ransac_correct();
        // d->ransac_counter++;
    // }
    return 0;
}

// class constructor
Ransac::Ransac() {
    // ransac_buffer_t zero_struct = {0};
    for (int i = 0; i < RANSAC_BUF_SIZE; i++) {
        // this->buffer.push_back(zero_struct);
    }

#ifdef LOG
    ransac_in_f = fopen("/mnt/sdcard/sensor_data/ransac_in_f.csv", "w+");
  	ransac_out_f = fopen("/mnt/sdcard/sensor_data/ransac_out_f.csv", "w+");
#endif
}


// class destructor
Ransac::~Ransac() {
#ifdef LOG
    fclose(ransac_in_f);
  	fclose(ransac_out_f);
#endif
}


// reset buffer after passing a gate
void Ransac::reset() {
	this->buffer_cnt = 0;

    for (int i = 0; i < RANSAC_BUF_SIZE; i++) {
        // this->buffer[i].valid = false;
    }
  
}

// todo: (2) ransac_push
// // insert the new sample in the buffer
// void Ransac::ransac_push(double mt, double mx, double my, double mz, double myaw) {

//     // std::rotate(this->buffer.begin(),
//     // 			this->buffer.begin() + 1,
// 	// 			this->buffer.end());

// 	// // Compare Vision measurement with the state at the time the image was taken
//     // this->buffer.back().a1 = detectedPoints->p1;
//     // this->buffer.back().a2 = detectedPoints->p2;
//     // this->buffer.back().a3 = detectedPoints->p3;
//     // this->buffer.back().a4 = detectedPoints->p4;
//     // this->buffer.back().a5 = detectedPoints->p5;
//     // this->buffer.back().a6 = detectedPoints->p6;
//     // this->buffer.back().valid = true;
//     // this->new_measurement = true;

// #ifdef LOG
//     fprintf(ransac_in_f, "%f,%f,%f,%f,%f\n", mt, mx, my, mz, myaw);
// #endif
// }



// modifies global variables of drone state and ransac buffer
void Ransac::ransac_correct() {

    this->new_measurement = false;
    
    // std::copy(this->buffer.begin(), this->buffer.end(), buffer_local.begin());
    
    // todo: (1) take buf_size_local from input stream
    int buf_size_local = RANSAC_BUF_SIZE; //getbufsize();

    // buffer correction if enough measurements
    if (this->buffer_cnt >= RANSAC_MIN_SAMPLES_FOR_FIT) {
        int n_samples = ((int)((float)buf_size_local * 0.75));

        // need at least 8 samples for ensuring no rank definiency 
        // while estimating RANSAC_A_WIDTH number of parameters
        if (n_samples < (RANSAC_A_WIDTH + 2)) {
        	return;
		}

        Eigen::Matrix<double, RANSAC_MIN_SAMPLES_FOR_FIT, RANSAC_A_WIDTH> a_matrix;
        Eigen::Matrix<double, RANSAC_MIN_SAMPLES_FOR_FIT, 1> b_matrix;
        // todo: (2) Fill LINEAR MODEL here!
        a_matrix.resize(5,5);
        b_matrix.resize(5,1);
        a_matrix.Zero();
        b_matrix.Zero();       
        
        // MatrixXd subject to change in size
        Eigen::MatrixXd subset_b_matrix(RANSAC_BUF_SIZE, 1);
        Eigen::MatrixXd subset_a_matrix(RANSAC_BUF_SIZE, RANSAC_A_WIDTH);
        Eigen::MatrixXd residual(RANSAC_BUF_SIZE, 1);

        // Matrix fixed size
        Eigen::Matrix<double, RANSAC_A_WIDTH, 1> subset_params;
        Eigen::Matrix<double, RANSAC_A_WIDTH, 1> best_params;

        // todo: CAUTION: need to know more about buf_size_local, indices subset, n_samples
        // maximum subset could be all the samples in get_indices_without_replacement = count
        int indices_subset[RANSAC_BUF_SIZE];

        // do the RANSAC iterations:
        for (int i = 0; i < RANSAC_ITERATIONS; i++) {

            // get a subset of indices - randomizer
            this->get_indices_without_replacement(indices_subset, n_samples, buf_size_local);

            subset_b_matrix.resize(n_samples, 1);
            subset_a_matrix.resize(n_samples, RANSAC_A_WIDTH);
            residual.resize(n_samples, 1);

            for (int j = 0; j < n_samples; j++) {
                subset_b_matrix.row(j) = b_matrix.row(indices_subset[j]);
                subset_a_matrix.row(j) = a_matrix.row(indices_subset[j]);
            }

            // calculate the params using least squares
            //subset_params = (subset_a_matrix.transpose()*subset_a_matrix).ldlt().solve(subset_a_matrix.transpose()*subset_b_matrix);

            // instead use svd. in case of Ax = 0; x is orthogonal to the space of A. Find the column of the right singular vector, 
            // whose corresponding singular value is the smallest - the last column of the decomposed matrix v. use jacobi for small and bcd for larger
            Eigen::JacobiSVD<Eigen::MatrixXd> svd(subset_a_matrix, Eigen::ComputeThinU | Eigen::ComputeThinV);
            // pick the last column of right singular matrix as orthogonal vector
            subset_params = svd.matrixV().col(RANSAC_A_WIDTH);

            // calculate the residual after least squares
            residual = subset_a_matrix*subset_params - subset_b_matrix; // where, b = 0;

            // 1-norm or normalized abs sum
            double fit_error = (residual.cwiseAbs().sum())/n_samples;

            static double least_error = fit_error;
            
            if (least_error > fit_error) {
                least_error = fit_error;
                best_params = subset_params;
            }
        }
        // todo: send best params ONLY IF least_error is under threshold?!


        // ask controller to reset est_state
        // controller->ransac_correct = true;
        

#ifdef LOG
		fprintf(ransac_out_f, "%f,%f,%f,%f,%f,%f,%f\n", curr_time, controller->corr_state.x, controller->corr_state.y, controller->corr_state.z,
														controller->corr_state.vx, controller->corr_state.vy, controller->corr_state.vz);
#endif
        this->ransac_cnt++;
    }
}


/** Get indices without replacement.
 *
 * @param[out] indices_subset This will be filled with the sampled indices
 * @param[in] n_samples The number of samples / indices.
 * @param[in] count The function will sample n_sample numbers from the range 1, 2, 3,..., count
 */
void Ransac::get_indices_without_replacement(int *indices_subset, int n_samples, int count) {
    int index;
    for (int j = 0; j < n_samples; j++) {
        bool picked_number = false;
        while (!picked_number) {
            index = rand() % count;
            bool new_val = true;
            for (int k = 0; k < j; k++) {
                if (indices_subset[k] == index) {
                    new_val = false;
                    break;
                }
            }
            if (new_val) {
                indices_subset[j] = index;
                picked_number = true;
            }
        }
    }
}