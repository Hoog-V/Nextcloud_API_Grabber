#include <WebRequests.h>

/*
 *  Macros to retrieve the index/ptr of the needed time/date info
 *  This is not best practice, though very fast because of the strict
 *  formatting used in the API. As long as the string to be formatted keeps using
 *  the same format it will work.
 */
#define Hour_ptr(str_ptr) str_ptr+17
#define Seconds_ptr(str_ptr) str_ptr+23
#define Minutes_ptr(str_ptr) str_ptr+21
#define DayOfMonth_ptr(str_ptr) str_ptr+5
#define Year_ptr(str_ptr) str_ptr+12

#define Decimal_base 10
#define CLOCK_START_YEAR 1900

#define MAX_PROPFIND_RESP_SIZE 256

/*
 * In the API the month is represented in text.
 * This means it has to be converted to a number to use it later in the tm struct.
 */
int MonthStrToInt(const char *month) {
    const char months[12][4] = {"Jan\0", "Feb\0", "Mar\0", "Apr\0", "May\0", "Jun\0",
                                "Jul\0", "Aug\0", "Sep\0", "Oct\0", "Nov\0", "Dec\0"};
    for (int i = 0; i < 12; i++) {
        if (!strcmp(month, months[i])) {
            return i;
        }
    }
    return -1;
}



struct tm FormattedStringToTm(const char *formattedString) {
    struct tm result;
    if (formattedString != NULL || strlen(formattedString) < 28) {
        const char Month[4] = {formattedString[8], formattedString[9], formattedString[10], '\0'};
        const int Year = strtol(Year_ptr(formattedString), NULL, Decimal_base);
        result.tm_hour = strtol(Hour_ptr(formattedString), NULL, Decimal_base);
        result.tm_sec = strtol(Seconds_ptr(formattedString), NULL, Decimal_base);
        result.tm_min = strtol(Minutes_ptr(formattedString), NULL, Decimal_base);
        result.tm_mday = strtol(DayOfMonth_ptr(formattedString), NULL, Decimal_base);
        result.tm_year = Year - CLOCK_START_YEAR;
        result.tm_mon = MonthStrToInt(Month);
    }
    return result;
}

time_t FormattedStringToTimeSinceEpoch(const char *formattedString) {
    struct tm parse = FormattedStringToTm(formattedString);
    return mktime(&parse);
}

char *ParsePropFindResponse(struct MemoryStruct *ReqData, const char *needle, const char *endtag) {
    static char result[MAX_PROPFIND_RESP_SIZE];
    const char *haystack = ReqData->memory;

    char *PtrToNeedle = strstr(haystack, needle);
    char *PtrToEndOfNeedle = strstr(haystack, endtag);

    if (PtrToNeedle != NULL && PtrToEndOfNeedle != NULL) {
        PtrToNeedle += strlen(needle);
        size_t SizeOfNeedle = abs(PtrToNeedle - PtrToEndOfNeedle);
        memcpy(result, PtrToNeedle, SizeOfNeedle);
        result[SizeOfNeedle] = '\0';
    }
    return result;
}