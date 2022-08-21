#ifndef NEXTCLOUD_API_GRABBER_LIBRARY_H
#define NEXTCLOUD_API_GRABBER_LIBRARY_H

#include <stddef.h>

typedef struct {
    char * nextcloud_url;
    char * authentication;
    char * user;
}api_grabber_prop_t;

int init_api_grabber(api_grabber_prop_t properties);

char * get_content_type(const char * filename);

char * get_date_changed(const char * filename);

size_t get_file_size(const char * filename);

void download_file(const char * filename, const char * loc);

char *get_etag(const char *filename);

char *get_file_id(const char *filename);

char *get_permissions(const char *filename);

char *get_content_length(const char *filename);

char *file_has_preview(const char *filename);

char *file_is_favorite(const char *filename);

char *file_has_unread_comments(const char *filename);

char *file_owner(const char *filename);


#endif //NEXTCLOUD_API_GRABBER_LIBRARY_H
