/**==============================================================================
 **  File Name   :   gps_fence.c
 **-----------------------------------------------------------------------------
 **  File Description:
 **
 **  	You are asked to code a simple artificial gps fence based on a ARM cortex M4 MCU
 **		with a gps module. The MCU is connected to the gps module via usart,
 **		and it is continously receiving GPGLL NMEA messages.
 **
 **		The peripherals and NVIC are already configured, and the usart_rx_irq_handler
 **		is called everytime a new byte is received.
 **
 **		Complete this program so that the LED is turned on, and the usart warning
 **		message is sent, anytime the robot finds itself further than FENCE_RADIUS
 **		of the reference gps coordinates.
 **
 **		The warning should synchronised on the gps messages, but we still want to minimize
 **		the amount of time running in the interrupt handler (as is good practice)
 **
 **		some given functions which are found in led.h:
 **			led_set(0) - turns off led
 **			led_set(1) - turns on led
 **		some given functions which are found in usart.h:
 **			usart_send_string("string") - send string over usart via polling (slow!)
 **===========================================================================*/
/**=============================================================================
 **                                 Includes
 **===========================================================================*/
#include <usart.h>
#include <led.h>
#include <hw_config.h>
#include <FreeRTOS.h>
#include <task.h>

// add math for trig
#include <math.h>

/**=============================================================================
 **                              Defines / Constants
 **===========================================================================*/
// warning message
const char WARNING_STR[] = "Robot is outside fence!";

// fence radius is given in meters!
const float FENCE_RADIUS = 100.0f;

// reference point given in decimal degrees!
const float LATITUDE_REFERENCE = 46.522152f;
const float LONGITUDE_REFERENCE =  6.618629f;

// GPGLL packet position
#define START_LAT 8
#define LEN_LAT 7
#define START_LON 18
#define LEN_LON 8

#define GPGLL_UPDATE_RATE_DELAY 500  // 500ms, 2Hz
#define EARTH_R 6371000
#define D2R 0.01745f

/**=============================================================================
 **                             Private Typedefs
 **===========================================================================*/

/**=============================================================================
 **                             Global Variables
 **===========================================================================*/

// populated by ISR
char lat_s[LEN_LAT] = {0};
char lon_s[LEN_LON] = {0};

// struct to hold position data
typedef struct lla {
 float lat;
 float lon;
} lla;

xSemaphoreHandle binary_sem;
// or SemaphoreHandle_t binary_sem

/**=============================================================================
 **                         Private Function Prototypes
 **===========================================================================*/

/**=============================================================================
 **                                  Tasks
 **===========================================================================*/
void gps_fence_task(void *pvParameters)
{
    while(1) {
      // successful message, parse to floats
      if (xSemaphoreTake(binary_sem, GPGLL_UPDATE_RATE_DELAY)) {
        // store current position
        lla pos = {LATITUDE_REFERENCE, LONGITUDE_REFERENCE};
	pos.lat = strtof(lat_s, NULL);
	pos.lon = strtof(lon_s, NULL);

	// store ref position as struct
	lla ref = {LATITUDE_REFERENCE, LONGITUDE_REFERENCE};
        float dist = 0.0;
	calc_dist(&pos, &ref, &dist);
        if (dist > FENCE_RADIUS) {
             usart_send_string(WARNING_STR);
	     led_set(1);
        } else {
             led_set(0):
        }
      } else {
        // UART HeartBeat warning if nothing for 500ms

      }
    }
}
/**=============================================================================
 **                             Private Functions
 **===========================================================================*/

/**=============================================================================
 **                             Public Functions
 **===========================================================================*/

/*
	Function name: calc_dist
	Description: calculate distance between two lla points on Earth
	Source: https://en.wikipedia.org/wiki/Great-circle_distance
	Disclaimer: Depends on FPU, highly nonlinear when points are close
*/
void calc_dist(lla* p1, lla* p2, float* dist)
{
     float angle = acosf(sinf(p1->lat * D2R)*sinf(p2->lat * D2R)
                 + cosf(p1->lat * D2R)*cosf(p2->lat * D2R)*cosf(D2R * fabsf(p1->lon - p2->lon)));
     *dist = EARTH_R * angle;
}


/*******************************************************************************
 * Function Name  : usart_rx_irq_handler
 * Description    : usart rx interrupt handler.
 ******************************************************************************/
void usart_rx_irq_handler(void)
{
    // byte counter for packet position
    static int cnt = 0;
    cnt += 1;

    // extract byte
    char rx_byte = USART->DR;

    // start state machine
    // $GPGLL,4916.45,N,12311.12,W,225444,A
    // 123456789....
    if (rx_byte == '$') {
	cnt = 1;
    } if (cnt >= START_LAT && cnt <= (START_LAT + LEN_LAT)) {
	lat_s[START_LAT+LEN_LAT-1-cnt] = rx_byte;
    } if (cnt >= START_LON && cnt <= (START_LON + LEN_LON)) {
	lon_s[START_LON+LEN_LON-1-cnt] = rx_byte;
    }
    // GPGLL parsed, TODO: checksum verification
    if (1) {
    	BaseType_t hp_task_woken = FALSE;
    	xSemaphoreGiveFromISR(binary_sem, &hp_task_woken);
	// hey scheduler, avoid starving high priority tasks!
	if (hp_task_woken) {
		vPortYieldFromISR();
	}

}

/*******************************************************************************
 * Function Name  : main
 * Description    : code entry point
 ******************************************************************************/
void main(void)
{
	// Create sync semaphore between uart isr and gps_fence_task
	vSemaphoreCreateBinary(binary_sem);

	// Configure all peripherals, interrupts and clocks
	hw_config();

   	// Create task
   	xTaskCreate(gps_fence_task, "gps fence", 256, NULL, 0, NULL);

    	// Start Scheduler
    	vTaskStartScheduler();

    	while(1) {
        	// No Man's Land
    	}
}
