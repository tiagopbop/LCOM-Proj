#include "rtc.h"
#include <time.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <minix/syslib.h>
#include <minix/drivers.h>

#define RTC_ADDR_REG 0x70
#define RTC_DATA_REG 0x71

#define SECONDS 0x00
#define MINUTES 0x02
#define HOURS 0x04
#define DAY 0x07
#define MONTH 0x08
#define YEAR 0x09

uint8_t convertBcd(uint8_t bcdNum) {
    return ((bcdNum >> 4) * 10) + (bcdNum & 0x0F);
}

int RTC_getTime(RTC_Time *time) {
    if (time == NULL) return 1;

    uint32_t sec, min, hour, day, month, year;

    sys_outb(RTC_ADDR_REG, SECONDS);
    sys_inb(RTC_DATA_REG, &sec);
    time->seconds = convertBcd(sec);

    sys_outb(RTC_ADDR_REG, MINUTES);
    sys_inb(RTC_DATA_REG, &min);
    time->minutes = convertBcd(min);

    sys_outb(RTC_ADDR_REG, HOURS);
    sys_inb(RTC_DATA_REG, &hour);
    time->hours = convertBcd(hour);

    sys_outb(RTC_ADDR_REG, DAY);
    sys_inb(RTC_DATA_REG, &day);
    time->day = convertBcd(day);

    sys_outb(RTC_ADDR_REG, MONTH);
    sys_inb(RTC_DATA_REG, &month);
    time->month = convertBcd(month);

    sys_outb(RTC_ADDR_REG, YEAR);
    sys_inb(RTC_DATA_REG, &year);
    time->year = convertBcd(year) + 2000;

    return 0;
}
