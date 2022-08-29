#ifndef NEXTCLOUD_API_GRABBER_LIBRARY_H
#define NEXTCLOUD_API_GRABBER_LIBRARY_H

#include <stddef.h>

typedef struct {
    char * nextcloud_url;
    char * username;
    char * password;
}api_grabber_prop_t;

int init_api_grabber(api_grabber_prop_t properties);

void download_file(const char * filename, const char * loc);

void upload_file(const char* filesystem_path, const char* nc_path);

char * get_content_type(const char * filename);

char * get_date_changed(const char * filename);

size_t get_file_size(const char * filename);

char *get_etag(const char *filename);

char *get_file_id(const char *filename);

char *get_permissions(const char *filename);

char *get_content_length(const char *filename);

char *file_has_preview(const char *filename);

char *file_is_favorite(const char *filename);

char *file_has_unread_comments(const char *filename);

char *file_owner(const char *filename);


#endif //NEXTCLOUD_API_GRABBER_LIBRARY_H
