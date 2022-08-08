#ifndef NEXTCLOUD_API_GRABBER_LIBRARY_H
#define NEXTCLOUD_API_GRABBER_LIBRARY_H

#include <stddef.h>

typedef struct {
    char * NextcloudURL;
    char * User;
    char * Password;
}API_GRABBER_PROP;

void downloadFile(const char * filename, const char * loc);

int initAPIGrabber(API_GRABBER_PROP Properties);

char * getContentType(const char * filename);

char * getDateChanged(const char * filename);

size_t getFileSize(const char * filename);


#endif //NEXTCLOUD_API_GRABBER_LIBRARY_H
