#include <web_requests.h>
#include <request_parser.h>

/*
 *  Macros to retrieve the index/ptr of the needed time/date info
 *  This is not best practice, though very fast because of the strict
 *  formatting used in the API. As long as the string to be formatted keeps using
 *  the same format it will work.
 */
#define HOUR_PTR(str_ptr) str_ptr+17
#define SECONDS_PTR(str_ptr) str_ptr+23
#define MINUTES_PTR(str_ptr) str_ptr+21
#define DOM_PTR(str_ptr) str_ptr+5
#define YEAR_PTR(str_ptr) str_ptr+12

#define DECIMAL_BASE 10
#define CLOCK_START_YEAR 1900


/*
 * In the API the month is represented in text.
 * This means it has to be converted to a number to be used in the tm struct.
 */
int month_str_to_int(const char *month) {
    const char months[12][4] = {"Jan\0", "Feb\0", "Mar\0", "Apr\0", "May\0", "Jun\0",
                                "Jul\0", "Aug\0", "Sep\0", "Oct\0", "Nov\0", "Dec\0"};
    for (int i = 0; i < 12; i++) {
        if (!strcmp(month, months[i])) {
            return i;
        }
    }
    return -1;
}



struct tm formatted_string_to_tm(const char *formatted_string) {
    struct tm result;
    if (formatted_string != NULL || strlen(formatted_string) < 28) {
        const char month[4] = {formatted_string[8], formatted_string[9], formatted_string[10], '\0'};
        const int year = strtol(YEAR_PTR(formatted_string), NULL, DECIMAL_BASE);
        result.tm_hour = strtol(HOUR_PTR(formatted_string), NULL, DECIMAL_BASE);
        result.tm_sec = strtol(SECONDS_PTR(formatted_string), NULL, DECIMAL_BASE);
        result.tm_min = strtol(MINUTES_PTR(formatted_string), NULL, DECIMAL_BASE);
        result.tm_mday = strtol(DOM_PTR(formatted_string), NULL, DECIMAL_BASE);
        result.tm_year = year - CLOCK_START_YEAR;
        result.tm_mon = month_str_to_int(month);
    }
    return result;
}

time_t formatted_string_to_time_since_epoch(const char *formatted_string) {
    struct tm parsed_tm_struct = formatted_string_to_tm(formatted_string);
    return mktime(&parsed_tm_struct);
}

char *parse_propfind_resp(char *resp_body, const char *begin_tag, const char *end_tag) {
    static char result[MAX_PROPFIND_RESP_SIZE];

    const char *ptr_needle = strstr(resp_body, begin_tag);
    const char *ptr_needle_end = strstr(resp_body, end_tag);

    if (ptr_needle != NULL && ptr_needle_end != NULL) {
        ptr_needle += strlen(begin_tag);

        size_t needle_size = abs(ptr_needle - ptr_needle_end);
        memcpy(result, ptr_needle, needle_size);

        result[needle_size] = '\0';
    }
    return result;
}