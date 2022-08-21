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

extern instance_prop_t _nc_instance_properties;

int download_req(const char* filename, const char* loc);

int propfind_req(const char *filename, enum req_prop_type_t req_prop_type);

#endif //NEXTCLOUD_API_GRABBER_WEB_REQUESTS_H
