#include <web_requests.h>

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

#ifndef CACHING
char attributes[NUM_OF_ATTR][60];
#endif

char *req_memory_ptrs[11];

/*
 * In the API the month is represented in text.
 * This means it has to be converted to a number to be used in the tm struct.
 */
int month_str_to_int(const char *month) {
    const char months[12][3] = {"Jan", "Feb", "Mar", "Apr", "May", "Jun",
                                "Jul", "Aug", "Sep", "Oct", "Nov", "Dec"};
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

char *find_char_in_string(char *string, const char character_to_find){
    do{
        string++;
    }while(*string!= character_to_find);
    return string+1;
}

void preparse_propfind_resp(char *resp_body, const enum req_prop_type_t dataType){
    resp_body = strstr(resp_body, "<d:prop>");
#ifdef CACHING
    for(int i =0; i< NUM_OF_ATTR; i++) {
        resp_body = find_char_in_string(resp_body, '>');
        resp_body = find_char_in_string(resp_body, '>');
        req_memory_ptrs[i] = resp_body;
        resp_body = find_char_in_string(resp_body, '<');
        *(resp_body-1) = '\0';
    }
#else
    resp_body = find_char_in_string(resp_body, '>');
    resp_body = find_char_in_string(resp_body, '>');
    req_memory_ptrs[dataType] = resp_body;
    resp_body = find_char_in_string(resp_body, '<');
    *(resp_body-1) = '\0';
    strcpy(attributes[dataType], req_memory_ptrs[dataType]);
#endif
}


char *get_pointer_to_parsed_resp_data(enum req_prop_type_t dataType){
#ifdef CACHING
    return req_memory_ptrs[dataType];
#else
    return attributes[dataType];
#endif
}
