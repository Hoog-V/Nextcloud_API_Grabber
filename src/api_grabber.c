#include <api_grabber.h>
#include <web_requests.h>
#include <request_parser.h>
#include <xml_parser_strings.h>

#include <curl/curl.h>
#include <stdlib.h>

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

    struct req_memory *req_data = propfind_req(filename, oc_size_req);
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

    struct req_memory *req_data = propfind_req(filename, lastmodified_req);
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

    struct req_memory *req_data = propfind_req(filename, contenttype_req);
    if (req_data->curl_status != CURLE_OK) {
        fprintf(stderr, "error: %s\n", curl_easy_strerror(req_data->curl_status));
    } else {
        char * parsed_response = parse_propfind_resp(req_data->memory, get_content_type_begin_tag,
                                                     get_content_type_end_tag);
        memcpy(content_type, parsed_response, strlen(parsed_response));
    }
    return content_type;
}

char *get_etag(const char *filename) {
    static char etag[41];

    struct req_memory *req_data = propfind_req(filename, etag_req);
    if (req_data->curl_status != CURLE_OK) {
        fprintf(stderr, "error: %s\n", curl_easy_strerror(req_data->curl_status));
    } else {
        char * parsed_response = parse_propfind_resp(req_data->memory, get_etag_begin_tag,
                                                     get_etag_end_tag);
        memcpy(etag, parsed_response, strlen(parsed_response));
    }
    return etag;
}

char *get_resource_type(const char *filename) {
    static char resource_type[41];

    struct req_memory *req_data = propfind_req(filename, resource_type_req);
    if (req_data->curl_status != CURLE_OK) {
        fprintf(stderr, "error: %s\n", curl_easy_strerror(req_data->curl_status));
    } else {
        char * parsed_response = parse_propfind_resp(req_data->memory, resource_type_begin_tag,
                                                                        resource_type_end_tag);
        memcpy(resource_type, parsed_response, strlen(parsed_response));
    }
    return resource_type;
}

char *get_file_id(const char *filename) {
    static char file_id[41];

    struct req_memory *req_data = propfind_req(filename, file_id_req);
    if (req_data->curl_status != CURLE_OK) {
        fprintf(stderr, "error: %s\n", curl_easy_strerror(req_data->curl_status));
    } else {
        char * parsed_response = parse_propfind_resp(req_data->memory, file_id_begin_tag,
                                                     file_id_end_tag);
        memcpy(file_id, parsed_response, strlen(parsed_response));
    }
    return file_id;
}

char *get_permissions(const char *filename) {
    static char permissions[41];

    struct req_memory *req_data = propfind_req(filename, permissions_req);
    if (req_data->curl_status != CURLE_OK) {
        fprintf(stderr, "error: %s\n", curl_easy_strerror(req_data->curl_status));
    } else {
        char * parsed_response = parse_propfind_resp(req_data->memory, permissions_begin_tag,
                                                     permissions_end_tag);
        memcpy(permissions, parsed_response, strlen(parsed_response));
    }
    return permissions;
}

char *get_content_length(const char *filename) {
    static char content_length[41];

    struct req_memory *req_data = propfind_req(filename, content_length_req);
    if (req_data->curl_status != CURLE_OK) {
        fprintf(stderr, "error: %s\n", curl_easy_strerror(req_data->curl_status));
    } else {
        char * parsed_response = parse_propfind_resp(req_data->memory, get_content_length_begin_tag,
                                                     get_content_length_end_tag);
        memcpy(content_length, parsed_response, strlen(parsed_response));
    }
    return content_length;
}

char *file_has_preview(const char *filename) {
    static char preview[41];

    struct req_memory *req_data = propfind_req(filename, has_preview_req);
    if (req_data->curl_status != CURLE_OK) {
        fprintf(stderr, "error: %s\n", curl_easy_strerror(req_data->curl_status));
    } else {
        char * parsed_response = parse_propfind_resp(req_data->memory, has_preview_begin_tag,
                                                     has_preview_end_tag);
        memcpy(preview, parsed_response, strlen(parsed_response));
    }
    return preview;
}

char *file_is_favorite(const char *filename) {
    static char favorite[41];

    struct req_memory *req_data = propfind_req(filename, favorite_req);
    if (req_data->curl_status != CURLE_OK) {
        fprintf(stderr, "error: %s\n", curl_easy_strerror(req_data->curl_status));
    } else {
        char * parsed_response = parse_propfind_resp(req_data->memory, favorite_begin_tag,
                                                     favorite_end_tag);
        memcpy(favorite, parsed_response, strlen(parsed_response));
    }
    return favorite;
}

char *file_has_unread_comments(const char *filename) {
    static char unread_comments[41];

    struct req_memory *req_data = propfind_req(filename, comments_unread_req);
    if (req_data->curl_status != CURLE_OK) {
        fprintf(stderr, "error: %s\n", curl_easy_strerror(req_data->curl_status));
    } else {
        char * parsed_response = parse_propfind_resp(req_data->memory, comments_unread_begin_tag,
                                                     comments_unread_end_tag);
        memcpy(unread_comments, parsed_response, strlen(parsed_response));
    }
    return unread_comments;
}

char *file_owner(const char *filename) {
    static char owner[41];

    struct req_memory *req_data = propfind_req(filename, owner_display_name_req);
    if (req_data->curl_status != CURLE_OK) {
        fprintf(stderr, "error: %s\n", curl_easy_strerror(req_data->curl_status));
    } else {
        char * parsed_response = parse_propfind_resp(req_data->memory, owner_display_name_begin_tag,
                                                     owner_display_name_end_tag);
        memcpy(owner, parsed_response, strlen(parsed_response));
    }
    return owner;
}

