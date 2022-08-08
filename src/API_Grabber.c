#include <API_Grabber.h>
#include <WebRequests.h>
#include <RequestParsing.h>

#include <curl/curl.h>
#include <stdlib.h>

const char *oc_size_begin_tag = "<oc:size>";
const char *oc_size_end_tag = "</oc:size>";

const char *get_lastmodified_begin_tag = "<d:getlastmodified>";
const char* get_lastmodified_end_tag = "</d:getlastmodified>";

const char *get_content_type_begin_tag = "<d:getcontenttype>";
const char *get_content_type_end_tag = "</d:getcontenttype>";

#define DATE_STR_SIZE 30


void CleanUpResp(struct MemoryStruct *ReqData) {
    curl_easy_cleanup(ReqData->CURLHandle);
}

int initAPIGrabber(API_GRABBER_PROP Properties){

    const char * DavURL = "/remote.php/dav/files/";
    int size = strlen(Properties.NextcloudURL) + strlen(Properties.User)+ strlen(DavURL)+2;
    strcat(_InstanceProperties.DavURL, Properties.NextcloudURL);
    strcat(_InstanceProperties.DavURL, DavURL);
    strcat(_InstanceProperties.DavURL, Properties.User);
    strcat(_InstanceProperties.DavURL, "/");
    _InstanceProperties.DavURL[size] = '\0';
    _InstanceProperties.Authentication = Properties.Authentication;
    printf("DAVURL: %s Authentication: %s \n", _InstanceProperties.DavURL, _InstanceProperties.Authentication);
    return 0;
}

void downloadFile(const char *filename, const char *loc) {
    struct MemoryStruct *ReqData = GetReq(filename);

    if (ReqData->CURLStatus != CURLE_OK) {
        fprintf(stderr, "error: %s\n", curl_easy_strerror(ReqData->CURLStatus));
    } else {
        FILE *fp;
        fp = fopen(loc, "w");
        fwrite(ReqData->memory, ReqData->size, ReqData->size, fp);
        fclose(fp);
        printf("%d", ReqData->size);
    }
    CleanUpResp(ReqData);
}


size_t getFileSize(const char *filename) {
    size_t Result = 0;

    struct MemoryStruct *ReqData = PropFindReq(filename, oc_size);
    if (ReqData->CURLStatus != CURLE_OK) {
        fprintf(stderr, "error: %s\n", curl_easy_strerror(ReqData->CURLStatus));
    } else {
        char *ResultStr = ParsePropFindResponse(ReqData, oc_size_begin_tag, oc_size_end_tag);
        Result = strtol(ResultStr, NULL, 10);
    }
    CleanUpResp(ReqData);
    return Result;
}

char *getDateChanged(const char *filename) {
    static char date[DATE_STR_SIZE];

    struct MemoryStruct *ReqData = PropFindReq(filename, get_lastmodified);
    if (ReqData->CURLStatus != CURLE_OK) {
        fprintf(stderr, "error: %s\n", curl_easy_strerror(ReqData->CURLStatus));
    } else {
        char * ParsedResponse = ParsePropFindResponse(ReqData, get_lastmodified_begin_tag, get_lastmodified_end_tag);
        memcpy(date, ParsedResponse, DATE_STR_SIZE);
    }
    CleanUpResp(ReqData);
    return date;
}

char *getContentType(const char *filename) {
    static char ContentType[MAX_PROPFIND_RESP_SIZE];

    struct MemoryStruct *ReqData = PropFindReq(filename, get_contenttype);
    if (ReqData->CURLStatus != CURLE_OK) {
        fprintf(stderr, "error: %s\n", curl_easy_strerror(ReqData->CURLStatus));
    } else {
        char * ParsedResponse = ParsePropFindResponse(ReqData, get_content_type_begin_tag, get_content_type_end_tag);
        memcpy(ContentType, ParsedResponse, strlen(ParsedResponse));
    }
    CleanUpResp(ReqData);
    return ContentType;
}