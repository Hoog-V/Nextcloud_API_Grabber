//
// Created by victor on 2-8-22.
//

#ifndef NEXTCLOUD_API_GRABBER_REQUESTPARSING_H
#define NEXTCLOUD_API_GRABBER_REQUESTPARSING_H
#include <time.h>
#include <string.h>
#include <stdlib.h>

#define MAX_PROPFIND_RESP_SIZE 256

char* ParsePropFindResponse(struct MemoryStruct* ReqData, const char* needle, const char* endtag);

struct tm FormattedStringToTm(const char * formattedString);

time_t FormattedStringToTimeSinceEpoch(const char * formattedString);


#endif //NEXTCLOUD_API_GRABBER_REQUESTPARSING_H
