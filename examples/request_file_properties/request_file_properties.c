#include <api_grabber.h>
#include <stdio.h>
#define SERVER_FILEPATH "Photos/Birdie.jpg" //Replace spaces with %20

#define NEXTCLOUD_BASE_URL "https://cloud.example.com"
#define NEXTCLOUD_AUTH "USER:PASSWORD"
#define NEXTCLOUD_USER "USER"

int main(int argc, char *argv[]){
    api_grabber_prop_t properties;
    properties.authentication = NEXTCLOUD_AUTH;
    properties.nextcloud_url= NEXTCLOUD_BASE_URL;
    properties.user = NEXTCLOUD_USER;
    init_api_grabber(properties);

    int file_size = get_file_size(SERVER_FILEPATH);
    char * date_changed = get_date_changed(SERVER_FILEPATH);
    char * content_type = get_content_type(SERVER_FILEPATH);

    printf("File: %s, Size: %d, Content type: %s, Last modified: %s \n", SERVER_FILEPATH, file_size, content_type, date_changed);

    return 0;
}