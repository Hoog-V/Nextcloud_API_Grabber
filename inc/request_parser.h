//
// Created by victor on 2-8-22.
//

#ifndef NEXTCLOUD_API_GRABBER_REQUEST_PARSER_H
#define NEXTCLOUD_API_GRABBER_REQUEST_PARSER_H
#include <time.h>
#include <string.h>
#include <stdlib.h>

#define PARSING_ERROR 300

enum req_prop_type_t{
    e_getlastmodified,
    e_getetag,
    e_getcontenttype,
    e_fileid,
    e_permissions,
    e_size,
    e_getcontentlength,
    e_has_preview,
    e_favorite,
    e_comments_unread,
    e_owner_display_name
};


char *get_pointer_to_parsed_resp_data(const enum req_prop_type_t dataType);

int preparse_propfind_resp(char *resp_body, const enum req_prop_type_t dataType);


struct tm formatted_string_to_tm(const char * formatted_string);

time_t formatted_string_to_time_since_epoch(const char * formatted_string);


#endif //NEXTCLOUD_API_GRABBER_REQUEST_PARSER_H
