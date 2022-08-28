#include <api_grabber.h>
#include <stdio.h>
#define SERVER_FILEPATH "Photos/Birdie.jpg" //Replace spaces with %20

#define NEXTCLOUD_BASE_URL "https://cloud.example.com"
#define NEXTCLOUD_USER "USER"
#define NEXTCLOUD_PASSWORD "PASSWORD"

int main(int argc, char *argv[]){
    api_grabber_prop_t properties;
    properties.nextcloud_url= NEXTCLOUD_BASE_URL;
    properties.username = NEXTCLOUD_USER;
    properties.password = NEXTCLOUD_PASSWORD;
    init_api_grabber(properties);

    int file_size = get_file_size(SERVER_FILEPATH);
    char *date_changed = get_date_changed(SERVER_FILEPATH);
    char *content_type = get_content_type(SERVER_FILEPATH);
    char *etag = get_etag(SERVER_FILEPATH);
    char *file_id = get_file_id(SERVER_FILEPATH);
    char *permissions = get_permissions(SERVER_FILEPATH);
    char *content_length = get_content_length(SERVER_FILEPATH);
    char *has_preview = file_has_preview(SERVER_FILEPATH);
    char *favorite = file_is_favorite(SERVER_FILEPATH);
    char *unread_comments = file_has_unread_comments(SERVER_FILEPATH);
    char *owner = file_owner(SERVER_FILEPATH);

    printf("File: %s, Size: %d, Content type: %s, Last modified: %s, etag_req: %s "
           "file_id: %s, permissions: %s, content_length: %s, "
           "has_preview: %s, favorite: %s, unread_comments: %s, owner: %s\n", SERVER_FILEPATH, file_size, content_type,
                                                                                                        date_changed, etag, file_id,
                                                                                                        permissions, content_length,
                                                                                                        has_preview, favorite, unread_comments,
                                                                                                        owner);

    return 0;
}