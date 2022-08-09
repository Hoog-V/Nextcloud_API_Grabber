//
// Created by victor on 2-8-22.
//

#ifndef NEXTCLOUD_API_GRABBER_REQUEST_PARSER_H
#define NEXTCLOUD_API_GRABBER_REQUEST_PARSER_H
#include <time.h>
#include <string.h>
#include <stdlib.h>

#define MAX_PROPFIND_RESP_SIZE 256

char* parse_propfind_resp(char * resp_body, const char* begin_tag, const char* end_tag);

struct tm formatted_string_to_tm(const char * formatted_string);

time_t formatted_string_to_time_since_epoch(const char * formatted_string);


#endif //NEXTCLOUD_API_GRABBER_REQUEST_PARSER_H
