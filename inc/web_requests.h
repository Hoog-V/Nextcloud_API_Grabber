#ifndef NEXTCLOUD_API_GRABBER_WEB_REQUESTS_H
#define NEXTCLOUD_API_GRABBER_WEB_REQUESTS_H

#include <stddef.h>
#include <string.h>
#include <stdlib.h>
#include <api_grabber_settings.h>
#include <request_parser.h>

#define WEB_REQ_OK 0
#define FILE_WRITE_ERROR 200
#define FILE_READ_ERROR 400


const char *get_error_msg(const int error_code);

int download_req(const char* filename, const char* loc);

int upload_req(const char* filesystem_path, const char* nc_path);

int propfind_req(const char *filename, const enum req_prop_type_t req_prop_type);

int init_platform();

#endif //NEXTCLOUD_API_GRABBER_WEB_REQUESTS_H