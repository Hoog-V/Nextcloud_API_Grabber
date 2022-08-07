#include <API_Grabber.h>
#include <WebRequests.h>
#include <ReqeustParsing.h>

#include <curl/curl.h>
#include <stdlib.h>

const char *oc_size_begin_tag = "<oc:size>";
const char *oc_size_end_tag = "</oc:size>";

const char *get_lastmodified_begin_tag = "<d:getlastmodified>";
const char* get_lastmodified_end_tag = "</d:getlastmodified>";

const char *get_content_type_begin_tag = "<d:getcontenttype>";
const char *get_content_type_end_tag = "</d:getcontenttype>";



void CleanUpResp(struct MemoryStruct *ReqData) {
    curl_easy_cleanup(ReqData->CURLHandle);
    free(ReqData->memory);
    free(ReqData);
}

int initAPIGrabber(API_GRABBER_PROP Properties){

    const char * DavURL = "/remote.php/dav/files/";
    int size = strlen(Properties.NextcloudURL) + strlen(Properties.User)+ strlen(DavURL)+2;
    _InstanceProperties.DavURL = malloc( size);
    strcat(_InstanceProperties.DavURL, Properties.NextcloudURL);
    strcat(_InstanceProperties.DavURL, DavURL);
    strcat(_InstanceProperties.DavURL, Properties.User);
    strcat(_InstanceProperties.DavURL, "/");
    _InstanceProperties.DavURL[size] = '\0';
    int AuthenticationSize = strlen(Properties.User)+strlen(Properties.Password)+2;
    _InstanceProperties.Authentication = malloc(AuthenticationSize);
    strcat(_InstanceProperties.Authentication, Properties.User);
    strcat(_InstanceProperties.Authentication, ":");
    strcat(_InstanceProperties.Authentication, Properties.Password);
    _InstanceProperties.Authentication[AuthenticationSize] = '\0';
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
        free(ResultStr);
    }
    CleanUpResp(ReqData);
    return Result;
}

char *getDateChanged(const char *filename) {
    static char *date;

    struct MemoryStruct *ReqData = PropFindReq(filename, get_lastmodified);
    if (ReqData->CURLStatus != CURLE_OK) {
        fprintf(stderr, "error: %s\n", curl_easy_strerror(ReqData->CURLStatus));
    } else {
        date = ParsePropFindResponse(ReqData, get_lastmodified_begin_tag, get_lastmodified_end_tag);
    }
    CleanUpResp(ReqData);
    return date;
}

char *getContentType(const char *filename) {
    static char *ContentType;

    struct MemoryStruct *ReqData = PropFindReq(filename, get_contenttype);
    if (ReqData->CURLStatus != CURLE_OK) {
        fprintf(stderr, "error: %s\n", curl_easy_strerror(ReqData->CURLStatus));
    } else {
        ContentType = ParsePropFindResponse(ReqData, get_content_type_begin_tag, get_content_type_end_tag);
    }
    CleanUpResp(ReqData);
    return ContentType;
}