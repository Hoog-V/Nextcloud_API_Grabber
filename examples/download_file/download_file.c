#include <api_grabber.h>

#define SERVER_FILEPATH "Photos/Birdie.jpg" //Replace spaces with %20
#define DOWNLOAD_FILEPATH "Birdie.jpg"

#define NEXTCLOUD_BASE_URL "https://cloud.example.com"
#define NEXTCLOUD_USER "USER"
#define NEXTCLOUD_PASSWORD "PASSWORD"

int main(int argc, char *argv[]){
    api_grabber_prop_t properties;
    properties.nextcloud_url= NEXTCLOUD_BASE_URL;
    properties.username = NEXTCLOUD_USER;
    properties.password = NEXTCLOUD_PASSWORD;

    init_api_grabber(properties);

    download_file(SERVER_FILEPATH, DOWNLOAD_FILEPATH);
    return 0;
}