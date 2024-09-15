#ifndef _LCOM_RTC_H_
#define _LCOM_RTC_H_

/**
 * @file rtc.h
 * @brief Functions for handling Real-Time Clock (RTC) operations.
 */

#include <stdint.h>
#include <stdbool.h>

/**
 * @brief Structure to hold the RTC time information.
 */
typedef struct {
    int hours;   /**< Hours */
    int minutes; /**< Minutes */
    int seconds; /**< Seconds */
    int day;     /**< Day */
    int month;   /**< Month */
    int year;    /**< Year */
} RTC_Time;

/**
 * @brief Retrieves the current time from the RTC.
 *
 * @param time Pointer to an RTC_Time structure to store the current time.
 * @return 0 on success, non-zero otherwise.
 */
int RTC_getTime(RTC_Time *time);

#endif // _LCOM_RTC_H_
