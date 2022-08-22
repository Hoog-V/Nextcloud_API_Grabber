#ifndef NEXTCLOUD_API_GRABBER_WEB_REQUESTS_H
#define NEXTCLOUD_API_GRABBER_WEB_REQUESTS_H

#include <stddef.h>
#include <curl/curl.h>
#include <string.h>
#include <stdlib.h>
#include <request_parser.h>
#include <api_grabber_settings.h>

#define TERMINATING_CHAR '\0'
#define TERMINATING_CHAR_SIZE 1

typedef struct {
    char dav_url[MAX_URL_LENGTH];
    char *authentication;
}instance_prop_t;

#define WEB_REQ_OK 0
#define FILE_WRITE_ERROR 200

extern instance_prop_t _nc_instance_properties;

const int download_req(const char* filename, const char* loc);

const int propfind_req(const char *filename, const enum req_prop_type_t req_prop_type);

const char *get_error_msg(const int error_code);

#endif //NEXTCLOUD_API_GRABBER_WEB_REQUESTS_H
