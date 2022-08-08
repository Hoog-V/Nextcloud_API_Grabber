#ifndef NEXTCLOUD_API_GRABBER_LIBRARY_H
#define NEXTCLOUD_API_GRABBER_LIBRARY_H

#include <stddef.h>

typedef struct {
    char * NextcloudURL;
    char * Authentication;
    char * User;
}API_GRABBER_PROP;

int initAPIGrabber(API_GRABBER_PROP Properties);

char * getContentType(const char * filename);

char * getDateChanged(const char * filename);

size_t getFileSize(const char * filename);




void downloadFile(const char * filename, const char * loc);


#endif //NEXTCLOUD_API_GRABBER_LIBRARY_H
