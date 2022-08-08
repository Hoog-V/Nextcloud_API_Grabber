#include <WebRequests.h>

#define MAX_URL_LENGTH 2048
#define TERMINATING_CHAR '\0'
#define TERMINATING_CHAR_SIZE 1

const char *empty_urlreq_body = "<?xml version=\"1.0\"?>\n"
                                "<d:propfind  xmlns:d=\"DAV:\" xmlns:oc=\"http://owncloud.org/ns\" xmlns:nc=\"http://nextcloud.org/ns\">\n"
                                "  <d:prop>\n"
                                "       %s\n"
                                "  </d:prop>\n"
                                "</d:propfind>";


char *oc_size_attr = "<oc:size />";
char *get_lastmodified_attr = "<d:getlastmodified />";
char *get_contenttype_attr = "<d:getcontenttype />";

WebReqProperties _InstanceProperties;

void setCurlOptions(struct MemoryStruct *chunk, const char* reqURL){
    curl_easy_setopt(chunk->CURLHandle, CURLOPT_URL, reqURL);
    curl_easy_setopt(chunk->CURLHandle, CURLOPT_USERPWD, _InstanceProperties.Authentication);
    curl_easy_setopt(chunk->CURLHandle, CURLOPT_FOLLOWLOCATION, 1L);
    curl_easy_setopt(chunk->CURLHandle, CURLOPT_WRITEFUNCTION, WriteMemoryCallback);
    curl_easy_setopt(chunk->CURLHandle, CURLOPT_WRITEDATA, (void *) chunk);
    curl_easy_setopt(chunk->CURLHandle, CURLOPT_HTTPAUTH, CURLAUTH_BASIC);
}

size_t WriteMemoryCallback(void *contents, size_t size, size_t nmemb, void *userp) {
    size_t realsize = size * nmemb;
    struct MemoryStruct *mem = (struct MemoryStruct *) userp;

    char *ptr = realloc(mem->memory, mem->size + realsize + 1);
    if (ptr == NULL) {
        printf("error: not enough memory\n");
        return 0;
    }

    mem->memory = ptr;
    memcpy(&(mem->memory[mem->size]), contents, realsize);
    mem->size += realsize;
    mem->memory[mem->size] = 0;

    return realsize;
}


struct MemoryStruct *InitializeCURLMemory() {
    static struct MemoryStruct *chunk;
    chunk = malloc(sizeof(struct MemoryStruct));
    chunk->memory = malloc(1);
    chunk->size = 0;
    return chunk;
}

char *generateReqUrl(const char *filename) {
    static char fileurl[MAX_URL_LENGTH];
    const int strsize = strlen(filename) + strlen(_InstanceProperties.DavURL) + TERMINATING_CHAR_SIZE;
    memset(fileurl, TERMINATING_CHAR, strsize);
    strcat(fileurl, _InstanceProperties.DavURL);
    strcat(fileurl, filename);
    return fileurl;
}


char *createReqBody(RequestPropertyType propertyType) {
    char *request;
    char *propertyBody;
    switch (propertyType) {
        case oc_size:
            propertyBody = oc_size_attr;
            break;
        case get_lastmodified:
            propertyBody = get_lastmodified_attr;
            break;
        case get_contenttype:
            propertyBody = get_contenttype_attr;
            break;
        default:
            return NULL;
    }
    const int sizeOfPropertyBody = strlen(propertyBody);
    const int sizeOfUrlReqBody = strlen(empty_urlreq_body);
    const int totalsize = sizeOfPropertyBody + sizeOfUrlReqBody + 1;
    request = malloc(totalsize);
    snprintf(request, totalsize, empty_urlreq_body, propertyBody);
    return request;
}


struct MemoryStruct *PropFindReq(const char *filename, RequestPropertyType ReqType) {
    char *UrlPostField = createReqBody(ReqType);
    char *reqURL = generateReqUrl(filename);

    struct MemoryStruct *chunk = InitializeCURLMemory();
    chunk->CURLHandle = curl_easy_init();
    if (chunk->CURLHandle) {
        setCurlOptions(chunk, reqURL);
        curl_easy_setopt(chunk->CURLHandle, CURLOPT_CUSTOMREQUEST, "PROPFIND");
        curl_easy_setopt(chunk->CURLHandle, CURLOPT_POST, 1L);
        curl_easy_setopt(chunk->CURLHandle, CURLOPT_POSTFIELDS, UrlPostField);
        chunk->CURLStatus = curl_easy_perform(chunk->CURLHandle);
    }
    free(UrlPostField);
    return chunk;
}

struct MemoryStruct *GetReq(const char *filename) {
    char *reqURL = generateReqUrl(filename);
    struct MemoryStruct *chunk = InitializeCURLMemory();

    chunk->CURLHandle = curl_easy_init();
    if (chunk->CURLHandle) {
        setCurlOptions(chunk, reqURL);
        chunk->CURLStatus = curl_easy_perform(chunk->CURLHandle);
    }
    return chunk;
}