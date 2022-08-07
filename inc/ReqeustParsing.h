//
// Created by victor on 2-8-22.
//

#ifndef NEXTCLOUD_API_GRABBER_REQEUSTPARSING_H
#define NEXTCLOUD_API_GRABBER_REQEUSTPARSING_H
#include <time.h>
#include <string.h>
#include <stdlib.h>

char* ParsePropFindResponse(struct MemoryStruct* ReqData, const char* needle, const char* endtag);

struct tm FormattedStringToTm(const char * formattedString);

time_t FormattedStringToTimeSinceEpoch(const char * formattedString);


#endif //NEXTCLOUD_API_GRABBER_REQEUSTPARSING_H
