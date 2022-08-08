#include <API_Grabber.h>

#define SERVER_FILEPATH "Photos/Birdie.jpg" //Replace spaces with %20
#define DOWNLOAD_FILEPATH "Birdie.jpg"

#define NEXTCLOUD_BASE_URL "https://cloud.example.com"
#define NEXTCLOUD_USER "username"
#define NEXTCLOUD_USER_Password "password"

int main(int argc, char *argv[]){
    API_GRABBER_PROP Properties;
    Properties.User = NEXTCLOUD_USER;
    Properties.Password = NEXTCLOUD_USER_Password;
    Properties.NextcloudURL= NEXTCLOUD_BASE_URL;
    initAPIGrabber(Properties);

    downloadFile(SERVER_FILEPATH,DOWNLOAD_FILEPATH);

    return 0;
}