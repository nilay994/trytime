
struct kf_filt {
    // covariances
    float p;
    float q;
    float r;
    
    // state
    float x;
};

void kf_filt_init(struct kf_filt * f, float p, float q, float r, float x0)
{
    f->p = p;
    f->q = q;
    f->r = r;

    f->x = x0;
}

void kf_filt_propagate(struct kf_filt * f)
{
    // (no state propagation since F = 1)
    f->p = f->p + f->q; // P = (F * P * F') + Q
}

float kf_filt_update(struct kf_filt * f, float z)
{
    // y = z - H * x (measurement innovation)
    float y = z - (2.0f * 1.414f * f->x); 

    // s = H * P * H' + R (preresidual covariance)
    float s = 8.0f * f->p + f->r; 

    // k = P * H' * s^(-1) (kalman gain)
    float k = f->p * 2.0f * 1.414f * 1.0f / s; 

    // x = x + k * y (update state)
    f->x = f->x + k * y;

    // P = (I - k * H) * P (update posterior)
    f->p = (1.0f - k * 2.0f * 1.414f) * f->p; 

    return f->x;
}


    // initialize a kalman estimator for RMS voltage (p, q, r)
    struct kf_filt kf_rms_est;
    kf_filt_init(&kf_rms_est, 1e-3f, 1e-3f, 1e-3f, 0.5f);