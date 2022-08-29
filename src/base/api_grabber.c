#include <api_grabber.h>
#include <web_requests.h>
#include <web_request_tools.h>
#include <request_parser.h>

#include <stdlib.h>
#include <stdio.h>

int init_api_grabber(api_grabber_prop_t properties){
    const char * dav_url_postfix = "/remote.php/dav/files/";

    //Compose the webdev url out of the base url and user info from the properties struct
    int size = strlen(properties.nextcloud_url) + strlen(properties.username) + strlen(dav_url_postfix) + 2; //+2 to compensate for the '\0' and '/' char
    strcat(_nc_instance_properties.dav_url, properties.nextcloud_url);
    strcat(_nc_instance_properties.dav_url, dav_url_postfix);
    strcat(_nc_instance_properties.dav_url, properties.username);
    strcat(_nc_instance_properties.dav_url, "/");
    _nc_instance_properties.dav_url[size] = '\0';

    _nc_instance_properties.username = properties.username;
    _nc_instance_properties.password = properties.password;
    return 0;
}

void download_file(const char *filename, const char *loc) {
    int status = download_req(filename, loc);

    if (status != WEB_REQ_OK) {
        ERROR_HANDLER(get_error_msg(status));
    }
}

void upload_file(const char* filesystem_path, const char* nc_path) {
    int status = upload_req(filesystem_path, nc_path);

    if (status != WEB_REQ_OK) {
        ERROR_HANDLER(get_error_msg(status));
    }
}

size_t get_file_size(const char *filename) {
    size_t result = 0;

    int status = propfind_req(filename, e_size);
    if (status != WEB_REQ_OK) {
        ERROR_HANDLER(get_error_msg(status));
    } else {
        char *result_str = get_pointer_to_parsed_resp_data(e_size);
        result = strtol(result_str, NULL, 10);
    }
    return result;
}

char *get_date_changed(const char *filename) {
    char *date = "NULL";
    int status = propfind_req(filename, e_getlastmodified);
    if (status != WEB_REQ_OK) {
        ERROR_HANDLER(get_error_msg(status));
    } else {
        date = get_pointer_to_parsed_resp_data(e_getlastmodified);
    }
    return date;
}

char *get_content_type(const char *filename) {
    char *content_type= "NULL";
    int status = propfind_req(filename, e_getcontenttype);
    if (status != WEB_REQ_OK) {
        ERROR_HANDLER(get_error_msg(status));
    } else {
        content_type = get_pointer_to_parsed_resp_data(e_getcontenttype);

    }
    return content_type;
}

char *get_etag(const char *filename) {
    char *etag= "NULL";

    int status = propfind_req(filename, e_getetag);
    if (status != WEB_REQ_OK) {
        ERROR_HANDLER(get_error_msg(status));
    } else {
        etag = get_pointer_to_parsed_resp_data(e_getetag);
    }
    return etag;
}

char *get_file_id(const char *filename) {
    char *file_id= "NULL";

    int status = propfind_req(filename, e_fileid);
    if (status != WEB_REQ_OK) {
        ERROR_HANDLER(get_error_msg(status));
    } else {
        file_id = get_pointer_to_parsed_resp_data(e_fileid);
    }
    return file_id;
}

char *get_permissions(const char *filename) {
    char *permissions= "NULL";

    int status = propfind_req(filename, e_permissions);
    if (status != WEB_REQ_OK) {
        ERROR_HANDLER(get_error_msg(status));
    } else {
        permissions = get_pointer_to_parsed_resp_data(e_permissions);
    }
    return permissions;
}

char *get_content_length(const char *filename) {
    char *content_length= "NULL";

    int status = propfind_req(filename, e_getcontentlength);
    if (status != WEB_REQ_OK) {
        ERROR_HANDLER(get_error_msg(status));
    } else {
        content_length = get_pointer_to_parsed_resp_data(e_getcontentlength);
    }
    return content_length;
}

char *file_has_preview(const char *filename) {
    char *preview= "NULL";
    int status = propfind_req(filename, e_has_preview);
    if (status != WEB_REQ_OK) {
        ERROR_HANDLER(get_error_msg(status));
    } else {
        preview = get_pointer_to_parsed_resp_data(e_has_preview);
    }
    return preview;
}

char *file_is_favorite(const char *filename) {
    char *favorite= "NULL";

    int status = propfind_req(filename, e_favorite);
    if (status != WEB_REQ_OK) {
        ERROR_HANDLER(get_error_msg(status));
    } else {
        favorite = get_pointer_to_parsed_resp_data(e_favorite);
    }
    return favorite;
}

char *file_has_unread_comments(const char *filename) {
    char *unread_comments= "NULL";

    int status = propfind_req(filename, e_comments_unread);
    if (status != WEB_REQ_OK) {
        ERROR_HANDLER(get_error_msg(status));
    } else {
        unread_comments = get_pointer_to_parsed_resp_data(e_comments_unread);
    }
    return unread_comments;
}

char *file_owner(const char *filename) {
    char *owner= "NULL";

    int status  = propfind_req(filename, e_owner_display_name);
    if (status != WEB_REQ_OK) {
        ERROR_HANDLER(get_error_msg(status));
    } else {
        owner = get_pointer_to_parsed_resp_data(e_owner_display_name);
    }
    return owner;
}

