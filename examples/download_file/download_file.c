#include <api_grabber.h>

#define SERVER_FILEPATH "Photos/Birdie.jpg" //Replace spaces with %20
#define DOWNLOAD_FILEPATH "Birdie.jpg"

#define NEXTCLOUD_BASE_URL "https://cloud.example.com"
#define NEXTCLOUD_AUTH "USER:PASSWORD"
#define NEXTCLOUD_USER "USER"

int main(int argc, char *argv[]){
    api_grabber_prop_t properties;
    properties.authentication = NEXTCLOUD_AUTH;
    properties.nextcloud_url= NEXTCLOUD_BASE_URL;
    properties.user = NEXTCLOUD_USER;

    init_api_grabber(properties);

    download_file(SERVER_FILEPATH, DOWNLOAD_FILEPATH);
    return 0;
}