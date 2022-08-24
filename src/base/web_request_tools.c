#include <web_request_tools.h>
#include <propfind_attr.h>


instance_prop_t _nc_instance_properties;


char *generateReqUrl(const char *filename) {
    static char file_url[MAX_URL_LENGTH];
    const unsigned long strsize = strlen(filename) + strlen(_nc_instance_properties.dav_url) + TERMINATING_CHAR_SIZE;
    memset(file_url, TERMINATING_CHAR, strsize);
    strcat(file_url, _nc_instance_properties.dav_url);
    strcat(file_url, filename);
    return file_url;
}


char *create_req_body(enum req_prop_type_t req_type) {
#ifdef CACHING
    return propfind_request_all;
#else
    static char req_body[220];
    const char* req_body_attr = req_body_attributes[req_type];
    const unsigned long sizeof_req_body_attr = strlen(req_body_attr);
    const unsigned long sizeof_empty_req_body = strlen(empty_req_body);
    const unsigned long req_body_size = sizeof_req_body_attr + sizeof_empty_req_body + TERMINATING_CHAR_SIZE;
    snprintf(req_body, req_body_size, empty_req_body, req_body_attr);
    return req_body;
#endif

}