#include <math.h>
#include <stdio.h>
#include <stdlib.h>

#define M_PI 3.142f

#define MEAS_MIN -5.0f
#define MEAS_MAX +5.0f

struct bw2_filt {
	float b[3]; // bi, bi-1, bi-2
	float a[3]; // ai, ai-1, ai-2
	float y[3]; // yi, yi-1, yi-2
	float x[3]; // xi, xi-1, xi-2
};


/**
 * Initializes a second order butterworth filter based on filter co-efficients.
 *
 * Note: Filter can be of any type, high pass, low pass, band-pass or band-pass
 *
 * Ref: octave-online.net
 * [b, a] = butter(2, fc/(fs/2), 'low') with params where
 *  - 2 is the order.
 *  - fc is the cutoff freq.
 *  - fs is the sampling freq.
 *  - 'low/high' is the filter type
 *
 * @param f Pointer to the filter's data struct
 * @param coeff_b Co-efficients associated with the input signal
 * @param coeff_a Co-efficients associated with the output signal
*/
void bw2_filt_init(struct bw2_filt * f, const float coeff_b[3], const float coeff_a[3])
{
	f->b[2] = coeff_b[2];
	f->b[1] = coeff_b[1];
	f->b[0] = coeff_b[0];

	f->a[2] = coeff_a[2];
	f->a[1] = coeff_a[1];
	f->a[0] = coeff_a[0];

	f->y[2] = 0.0f;
	f->y[1] = 0.0f;
	f->y[0] = 0.0f;

	f->x[2] = 0.0f;
	f->x[1] = 0.0f;
	f->x[0] = 0.0f;
}


/**
 * Runs a 2nd order butterworth filter on the passed measurement
 *
 * A second order Butterworth filter can be expressed as:
 *
 *   (a[2] * y[2] + a[1] * y[1] + a[0] * y[0])
 * = (b[2] * x[2] + b[1] * x[1] + b[0] * x[0])
 *
 * @param f  Pointer to filter's struct
 * @param in Input signal
 * @return Output signal
 */
float bw2_filt_run(struct bw2_filt * f, float in)
{
	// shift history by one
	f->x[0] = f->x[1];
	f->x[1] = f->x[2];
	f->x[2] = in;

	f->y[0] = f->y[1];
	f->y[1] = f->y[2];

	// run the filter
	f->y[2] = (1.0f/f->a[2]) * ((f->b[2] * f->x[2] +
								 f->b[1] * f->x[1] +
								 f->b[0] * f->x[0]) -
								(f->a[1] * f->y[1] +
								 f->a[0] * f->y[0]));

	return f->y[2];
}

// housekeeping for v_rms estimation
struct v_rms_est {
	float v_prev;
	float v_max;
	float v_peak;
};

/**
 * Initialize RMS voltage estimator
 *
 * @param est Pointer to the estimator's struct
 */
void estimate_v_rms_init(struct v_rms_est * est)
{
	est->v_prev = 0.0f;
	est->v_max = 0.0f;
	est->v_peak = 0.0f;
}


/**
 * @brief Estimate RMS voltage from voltage at peak.
 *
 * Resets the estimate during negative troughs to allow 
 * for a fresh RMS measurement every cycle.
 *
 * @param est Pointer to estimator's struct
 * @param v   Filtered AC voltage (in volts)
 * @return V_rms (in volts)
 */
float estimate_v_rms_run(struct v_rms_est * est, float v)
{
	if (v > 0.1f) {
		// in the positive cycle, record the maximum voltage
		if (v > est->v_max) {
			est->v_max = v;
		}
	} else if (((v * est->v_prev) < 0.0f) && (v < 0.0f)) {
		// during zero crossing, readout earlier observed
		// maximum as the peak voltage
		est->v_peak = est->v_max;
		// clear maximum to record peak in a new cycle
		est->v_max = 0.0f;
	}
	est->v_prev = v;
	return (est->v_peak * 0.7071f);
}


/**
 * Emulates a sinewave for test purposes
 *
 * Characteristics:
 *  - Amplitude: 1V
 *  - Freq: 50 Hz
 *  - Noise augmented: 10%
 *  - DC bias augmented: 20%
 *
 * @param t Elapsed time
 * @return Sample of the sinewave at time t
 */
float gen_50hz_sinewave(float t)
{
	float wave = 0.2f + sin(2.0f * M_PI * 50.0f * t);
	wave += 0.1f * (((float)(rand()) / (float)(RAND_MAX)) - 0.5f);
	return wave;
}

struct chk_meas {
	float prev_meas;
};


/**
 * Initializes the measurement checking struct
 *
 * @param f Pointer to the check measure struct
 */
void check_measure_init(struct chk_meas * f)
{
	f->prev_meas = 0.0f;
}

/**
 * Rejects measurements when they seem incorrect
 *
 * @param f Pointer to the check measure struct
 * @param meas Current measurement
 * @return Measurements after rejections
 */
float check_measure_run(struct chk_meas * f, float meas)
{
	float out;
	out = ((meas < MEAS_MIN) || (meas > MEAS_MAX)) ? f->prev_meas : meas;
	f->prev_meas = out;
	return out;
}


void main()
{
	// load params
	float fs = 20e3f; // sampling freq: 20 kHz
	float t = 0.0f;   // elapsed time (s)

	// open a few files for logging
	FILE * filt_fp, * est_fp;
	filt_fp = fopen("filt.csv", "w+");
	est_fp = fopen("est.csv", "w+");

	// initialize a low pass filter via [b, a] = butter(2, 1000/(20000/2), 'low');
	// Note: accuracy matters, use precise co-efficients
	struct bw2_filt lpf_1kHz;
	const float lpf_1kHz_coeff_b[3] = {0.020083365564211f,  0.040166731128422f, 0.020083365564211f};
	const float lpf_1kHz_coeff_a[3] = {0.641351538057563f, -1.561018075800718f, 1.000000000000000f};
	bw2_filt_init(&lpf_1kHz, lpf_1kHz_coeff_b, lpf_1kHz_coeff_a);

	// initialize a high pass filter via [b, a] = butter(2, 30/(20000/2), 'high');
	// Note: accuracy matters, use precise co-efficients
	struct bw2_filt hpf_30Hz;
	const float hpf_30Hz_coeff_b[3] = {0.993357832710430f, -1.986715665420860f, 0.993357832710430f};
	const float hpf_30Hz_coeff_a[3] = {0.986759784293781f, -1.986671546547938f, 1.000000000000000f};
	bw2_filt_init(&hpf_30Hz, hpf_30Hz_coeff_b, hpf_30Hz_coeff_a);

	// initialize the RMS voltage estimator
	struct v_rms_est est_filt;
	estimate_v_rms_init(&est_filt);

	// initialize the measurement checker
	struct chk_meas check_measure_filt;
	check_measure_init(&check_measure_filt);

	while (1) {

		// step ahead in time
		t += 1.0f/fs;

		if (t > 0.1f) {
			// flush all filestream buffers
			fflush(filt_fp);
			fflush(est_fp);
			// simulate only for 1 second
			break;
		}

		// emulate a sinewave measurement
		float volt_meas = gen_50hz_sinewave(t);

		// reject incorrect measurements
		volt_meas = check_measure_run(&check_measure_filt, volt_meas);

		// first LPF
		float filt_volt_meas = bw2_filt_run(&lpf_1kHz, volt_meas);
		// then HPF
		filt_volt_meas = bw2_filt_run(&hpf_30Hz, filt_volt_meas);

		// estimate the RMS voltage with filtered measurements
		float v_rms = estimate_v_rms_run(&est_filt, filt_volt_meas);

		// save results to files
		fprintf(filt_fp, "%f, %f, %f\n", t, volt_meas, filt_volt_meas);
		fprintf(est_fp, "%f, %f\n", t, v_rms);

		// sleep for 50 microseconds (loop rate)
		// usleep(50);
	}
}
