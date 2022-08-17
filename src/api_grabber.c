#include <api_grabber.h>
#include <web_requests.h>
#include <request_parser.h>

#include <curl/curl.h>
#include <stdlib.h>

const char *oc_size_begin_tag = "<oc:size>";
const char *oc_size_end_tag = "</oc:size>";

const char *get_lastmodified_begin_tag = "<d:getlastmodified>";
const char* get_lastmodified_end_tag = "</d:getlastmodified>";

const char *get_content_type_begin_tag = "<d:getcontenttype>";
const char *get_content_type_end_tag = "</d:getcontenttype>";

#define DATE_STR_SIZE 30


int init_api_grabber(api_grabber_prop_t properties){
    const char * dav_url_postfix = "/remote.php/dav/files/";

    //Compose the webdev url out of the base url and user info from the properties struct
    int size = strlen(properties.nextcloud_url) + strlen(properties.user) + strlen(dav_url_postfix) + 2; //+2 to compensate for the '\0' and '/' char
    strcat(_nc_instance_properties.dav_url, properties.nextcloud_url);
    strcat(_nc_instance_properties.dav_url, dav_url_postfix);
    strcat(_nc_instance_properties.dav_url, properties.user);
    strcat(_nc_instance_properties.dav_url, "/");
    _nc_instance_properties.dav_url[size] = '\0';

    _nc_instance_properties.authentication = properties.authentication;
    return 0;
}

void download_file(const char *filename, const char *loc) {
    int status = download_req(filename, loc);

    if (status != 0) {
        fprintf(stderr, "error: couldn't download file\n");
    }
}

size_t get_file_size(const char *filename) {
    size_t result = 0;

    struct req_memory *req_data = propfind_req(filename, oc_size);
    if (req_data->curl_status != CURLE_OK) {
        fprintf(stderr, "error: %s\n", curl_easy_strerror(req_data->curl_status));
    } else {
        char *result_str = parse_propfind_resp(req_data->memory, oc_size_begin_tag, oc_size_end_tag);
        result = strtol(result_str, NULL, 10);
    }
    return result;
}

char *get_date_changed(const char *filename) {
    static char date[DATE_STR_SIZE];

    struct req_memory *req_data = propfind_req(filename, get_lastmodified);
    if (req_data->curl_status != CURLE_OK) {
        fprintf(stderr, "error: %s\n", curl_easy_strerror(req_data->curl_status));
    } else {
        char * parsed_resp = parse_propfind_resp(req_data->memory, get_lastmodified_begin_tag,
                                                 get_lastmodified_end_tag);
        memcpy(date, parsed_resp, DATE_STR_SIZE);
    }
    return date;
}

char *get_content_type(const char *filename) {
    static char content_type[MAX_PROPFIND_RESP_SIZE];

    struct req_memory *req_data = propfind_req(filename, get_contenttype);
    if (req_data->curl_status != CURLE_OK) {
        fprintf(stderr, "error: %s\n", curl_easy_strerror(req_data->curl_status));
    } else {
        char * parsed_response = parse_propfind_resp(req_data->memory, get_content_type_begin_tag,
                                                     get_content_type_end_tag);
        memcpy(content_type, parsed_response, strlen(parsed_response));
    }
    return content_type;
}