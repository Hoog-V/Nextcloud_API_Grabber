#ifndef NEXTCLOUD_API_GRABBER_WEB_REQUEST_TOOLS_H
#define NEXTCLOUD_API_GRABBER_WEB_REQUEST_TOOLS_H

#include <string.h>
#include <api_grabber_settings.h>
#include <stdbool.h>
#include <request_parser.h>

#define TERMINATING_CHAR '\0'
#define TERMINATING_CHAR_SIZE 1

typedef struct {
    char dav_url[MAX_URL_LENGTH];
    char *authentication;
}instance_prop_t;


extern instance_prop_t _nc_instance_properties;

char *generateReqUrl(const char *filename);

char *create_req_body(enum req_prop_type_t req_type);

#endif //NEXTCLOUD_API_GRABBER_WEB_REQUEST_TOOLS_H
