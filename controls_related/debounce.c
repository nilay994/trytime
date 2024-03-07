#include <stdio.h>
#include <stdint.h>

typedef struct {
    uint8_t rise_count;
    uint8_t fall_count;

    uint8_t prev_state;

    uint8_t n_rise;
    uint8_t n_fall;
} db_t;


/**
 * Initialize a debounce functionality
 * 
 * @param db     Pointer to the internal debounce struct
 * @param n_rise Number of samples that need to be 1 to make the output 1
 * @param n_fall Number of samples that need to be 0 to make the output 0
 * @param init_state Initial state of the debounce output
 */
void db_init(db_t * db, uint8_t n_rise, uint8_t n_fall, uint8_t init_state)
{
    db->prev_state = (init_state > 0u) ? 1u : 0u;
    db->rise_count = 0u;
    db->fall_count = 0u;
    db->n_rise = (n_rise < 1u) ? 1u : n_rise;
    db->n_fall = (n_fall < 1u) ? 1u : n_fall;
}

/**
 * Run a debouncer
 * 
 * @param db Pointer to the internal debounce struct
 * @param in Input signal to be debounced
 * @return Debounced input
 */
uint8_t db_update(db_t * db, uint8_t in)
{
    uint8_t out;

    if (in > 0u) {
        if (db->rise_count < 255u) {
            db->rise_count++;
        }
        db->fall_count = 0u;
    } else {
        if (db->fall_count < 255u) {
            db->fall_count++;
        }
        db->rise_count = 0u;
    }

    if (db->rise_count >= db->n_rise) {
        out = 1u;
    } else if (db->fall_count >= db->n_fall) {
        out = 0u;
    } else {
        out = db->prev_state;
    }

    db->prev_state = out;

    return out;
}

static db_t button_db;
static xSemaphore button_pressed_semaphore;

void button_gpio_intr_handler() {
    uint8_t button_state = db_update(&button_db, 0u);
    // active low 
    if (!button_state) {
        xSemaphoreGiveFromISR(button_pressed_semaphore);
    }
}


void main()
{
    // initialize GPIO
    gpio_set_pin_mode(PIN_A6, PULLUP_10K);
    // trigger interrupt on falling edge and register callback
    gpio_set_intr(FALLING_EDGE, button_gpio_intr_handler);
    
    // physical debounce of around 2 ms in rise and fall is already present
    // at 20 kHz, mark 200 samples (i.e. 10 ms) for debouncing on either sides
    db_init(&button_db, 200u, 200u, 1u);

    while (1) {
        // blocking call to wait until button is pressed
        xSemaphoreTake(button_pressed_semaphore);
        usleep(0.05 ms);
    }
}


void test1()
{
    db_t switch_db;
    uint8_t in[15] = {0u, 0u, 0u, 1u, 0u,
                      1u, 0u, 1u, 1u, 0u,
                      0u, 1u, 0u, 0u, 0u};

    uint8_t exp[15] = {0u, 0u, 0u, 0u, 0u,
                       0u, 0u, 0u, 1u, 1u,
                       1u, 1u, 1u, 1u, 0u};

    db_init(&switch_db, 2u, 3u, 0u);

    for (uint8_t i = 0u; i < 15u; i++) {
        assert(exp[i] == db_update(&switch_db, in[i]));
    }
}

void test2()
{
    db_t switch_db;
    uint8_t in[15] = {0u, 0u, 0u, 1u, 0u,
                      0u, 0u, 0u, 0u, 1u,
                      0u, 0u, 0u, 0u, 0u};

    uint8_t exp[15] = {1u, 1u, 1u, 1u, 1u,
                       1u, 1u, 0u, 0u, 1u,
                       1u, 1u, 1u, 0u, 0u};

    db_init(&switch_db, 1u, 4u, 1u);

    for (uint8_t i = 0u; i < 15u; i++) {
        assert(exp[i] == db_update(&switch_db, in[i]));
    }
}

void test3()
{
    db_t switch_db;
    uint8_t in[15] = {0u, 1u, 0u, 1u, 0u,
                      0u, 1u, 0u, 1u, 0u,
                      1u, 1u, 0u, 0u, 1u};

    uint8_t exp[15] = {0u, 1u, 0u, 1u, 0u,
                       0u, 1u, 0u, 1u, 0u,
                       1u, 1u, 0u, 0u, 1u};

    db_init(&switch_db, 1u, 1u, 0u);

    for (uint8_t i = 0u; i < 15u; i++) {
        assert(exp[i] == db_update(&switch_db, in[i]));
    }
}

void test4()
{
    db_t switch_db;
    uint8_t in[15] = {0u, 0u, 0u, 0u, 0u,
                      1u, 1u, 1u, 1u, 1u,
                      0u, 0u, 0u, 0u, 0u};

    uint8_t exp[15] = {1u, 1u, 1u, 1u, 0u,
                       0u, 0u, 0u, 0u, 1u,
                       1u, 1u, 1u, 1u, 0u};

    db_init(&switch_db, 5u, 5u, 1u);

    for (uint8_t i = 0u; i < 15u; i++) {
        assert(exp[i] == db_update(&switch_db, in[i]));
    }
}