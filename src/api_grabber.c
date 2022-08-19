#include <api_grabber.h>
#include <web_requests.h>
#include <request_parser.h>

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
        char *result_str = get_pointer_to_parsed_resp_data(e_size);
        result = strtol(result_str, NULL, 10);
    }
    return result;
}

char *get_date_changed(const char *filename) {
    char *date;
    struct req_memory *req_data = propfind_req(filename, lastmodified_req);
    if (req_data->curl_status != CURLE_OK) {
        fprintf(stderr, "error: %s\n", curl_easy_strerror(req_data->curl_status));
    } else {
        date = get_pointer_to_parsed_resp_data(e_getlastmodified);
    }
    return date;
}

char *get_content_type(const char *filename) {
    char *content_type;
    struct req_memory *req_data = propfind_req(filename, contenttype_req);
    if (req_data->curl_status != CURLE_OK) {
        fprintf(stderr, "error: %s\n", curl_easy_strerror(req_data->curl_status));
    } else {
        content_type = get_pointer_to_parsed_resp_data(e_getcontenttype);

    }
    return content_type;
}

char *get_etag(const char *filename) {
    char *etag;

    struct req_memory *req_data = propfind_req(filename, etag_req);
    if (req_data->curl_status != CURLE_OK) {
        fprintf(stderr, "error: %s\n", curl_easy_strerror(req_data->curl_status));
    } else {
        etag = get_pointer_to_parsed_resp_data(e_getetag);
    }
    return etag;
}

char *get_file_id(const char *filename) {
    char *file_id;

    struct req_memory *req_data = propfind_req(filename, file_id_req);
    if (req_data->curl_status != CURLE_OK) {
        fprintf(stderr, "error: %s\n", curl_easy_strerror(req_data->curl_status));
    } else {
        file_id = get_pointer_to_parsed_resp_data(e_fileid);
    }
    return file_id;
}

char *get_permissions(const char *filename) {
    char *permissions;

    struct req_memory *req_data = propfind_req(filename, permissions_req);
    if (req_data->curl_status != CURLE_OK) {
        fprintf(stderr, "error: %s\n", curl_easy_strerror(req_data->curl_status));
    } else {
        permissions = get_pointer_to_parsed_resp_data(e_permissions);
    }
    return permissions;
}

char *get_content_length(const char *filename) {
    char *content_length;

    struct req_memory *req_data = propfind_req(filename, content_length_req);
    if (req_data->curl_status != CURLE_OK) {
        fprintf(stderr, "error: %s\n", curl_easy_strerror(req_data->curl_status));
    } else {
        content_length = get_pointer_to_parsed_resp_data(e_getcontentlength);
    }
    return content_length;
}

char *file_has_preview(const char *filename) {
    char *preview;

    struct req_memory *req_data = propfind_req(filename, has_preview_req);
    if (req_data->curl_status != CURLE_OK) {
        fprintf(stderr, "error: %s\n", curl_easy_strerror(req_data->curl_status));
    } else {
        preview = get_pointer_to_parsed_resp_data(e_has_preview);
    }
    return preview;
}

char *file_is_favorite(const char *filename) {
    char *favorite;

    struct req_memory *req_data = propfind_req(filename, favorite_req);
    if (req_data->curl_status != CURLE_OK) {
        fprintf(stderr, "error: %s\n", curl_easy_strerror(req_data->curl_status));
    } else {
        favorite = get_pointer_to_parsed_resp_data(e_favorite);
    }
    return favorite;
}

char *file_has_unread_comments(const char *filename) {
    char *unread_comments;

    struct req_memory *req_data = propfind_req(filename, comments_unread_req);
    if (req_data->curl_status != CURLE_OK) {
        fprintf(stderr, "error: %s\n", curl_easy_strerror(req_data->curl_status));
    } else {
        unread_comments = get_pointer_to_parsed_resp_data(e_comments_unread);
    }
    return unread_comments;
}

char *file_owner(const char *filename) {
    char *owner;

    struct req_memory *req_data = propfind_req(filename, owner_display_name_req);
    if (req_data->curl_status != CURLE_OK) {
        fprintf(stderr, "error: %s\n", curl_easy_strerror(req_data->curl_status));
    } else {
        owner = get_pointer_to_parsed_resp_data(e_owner_display_name);
    }
    return owner;
}

