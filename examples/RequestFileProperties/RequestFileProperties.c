#include <API_Grabber.h>
#include <stdio.h>
#define SERVER_FILEPATH "Photos/Birdie.jpg" //Replace spaces with %20

#define NEXTCLOUD_BASE_URL "https://cloud.example.com"
#define NEXTCLOUD_AUTH "USER:PASSWORD"
#define NEXTCLOUD_USER "USER"

int main(int argc, char *argv[]){
    API_GRABBER_PROP Properties;
    Properties.Authentication = NEXTCLOUD_AUTH;
    Properties.NextcloudURL= NEXTCLOUD_BASE_URL;
    Properties.User = NEXTCLOUD_USER;
    initAPIGrabber(Properties);

    int filesize = getFileSize(SERVER_FILEPATH);
    char * dateChanged = getDateChanged(SERVER_FILEPATH);
    char * contentType = getContentType(SERVER_FILEPATH);

    printf("File: %s, Size: %d, Content type: %s, Last Modified: %s \n", SERVER_FILEPATH, filesize, contentType, dateChanged);

    return 0;
}