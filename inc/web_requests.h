//
// Created by victor on 2-8-22.
//

#ifndef NEXTCLOUD_API_GRABBER_WEB_REQUESTS_H
#define NEXTCLOUD_API_GRABBER_WEB_REQUESTS_H

#include <stddef.h>
#include <curl/curl.h>
#include <string.h>
#include <stdlib.h>

struct req_memory {
    char memory[2048];
    size_t size;
    CURLcode curl_status;
    CURL* curl_handle;
};

typedef enum {
    oc_size,
    get_lastmodified,
    get_contenttype,
}req_prop_type_t;

typedef struct {
    char dav_url[512];
    char *authentication;
}instance_prop_t;


extern instance_prop_t _instance_properties;

struct req_memory* get_req(const char* filename);

struct req_memory *propfind_req(const char *filename, req_prop_type_t ReqType);

#endif //NEXTCLOUD_API_GRABBER_WEB_REQUESTS_H
