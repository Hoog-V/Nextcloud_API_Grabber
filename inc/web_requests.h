#ifndef NEXTCLOUD_API_GRABBER_WEB_REQUESTS_H
#define NEXTCLOUD_API_GRABBER_WEB_REQUESTS_H

#include <stddef.h>
#include <curl/curl.h>
#include <string.h>
#include <stdlib.h>


// CACHING enabled by default, uncomment this macro to disable
#define CACHING
#define CACHING_TIME 15*60
#define MAX_URL_LENGTH 2048
#define TERMINATING_CHAR '\0'
#define TERMINATING_CHAR_SIZE 1

typedef enum {
    get,
    propfind
} req_type_t;

struct req_memory {
    char memory[1024];
    size_t size;
    CURLcode curl_status;
    CURL* curl_handle;
#ifdef CACHING
    time_t req_time;
    req_type_t req_type;
#endif
};

typedef enum {
    req_all,
    oc_size,
    get_lastmodified,
    get_contenttype,
}req_prop_type_t;

typedef struct {
    char dav_url[512];
    char *authentication;
}instance_prop_t;



extern instance_prop_t _instance_properties;

int download_req(const char* filename, const char* loc);

struct req_memory *propfind_req(const char *filename, req_prop_type_t req_prop_type);

#endif //NEXTCLOUD_API_GRABBER_WEB_REQUESTS_H
