//
// Created by victor on 2-8-22.
//

#ifndef NEXTCLOUD_API_GRABBER_WEBREQUESTS_H
#define NEXTCLOUD_API_GRABBER_WEBREQUESTS_H

#include <stddef.h>
#include <curl/curl.h>
#include <string.h>
#include <stdlib.h>

struct MemoryStruct {
    char memory[2048];
    size_t size;
    CURLcode CURLStatus;
    CURL* CURLHandle;
};

typedef enum {
    oc_size,
    get_lastmodified,
    get_contenttype,
}RequestPropertyType;

typedef struct {
    char DavURL[512];
    char *Authentication;
} WebReqProperties;


extern WebReqProperties _InstanceProperties;

size_t WriteMemoryCallback(void *contents, size_t size, size_t nmemb, void *userp);

struct MemoryStruct* GetReq(const char* filename);

struct MemoryStruct *PropFindReq(const char *filename, RequestPropertyType ReqType);

#endif //NEXTCLOUD_API_GRABBER_WEBREQUESTS_H
