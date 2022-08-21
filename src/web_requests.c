#include <web_requests.h>
#include <propfind_attr.h>
#include <time.h>
#include <request_parser.h>

#ifdef CACHING
#include <stdbool.h>

#define REQ_MEM_SIZE 800+MAX_URL_LENGTH //A empty req result(caching) will approximately be 800 bytes long
                                        //Adding the url size to it because it is included in req result
#else
#define REQ_MEM_SIZE 360+MAX_URL_LENGTH //A empty req result(caching) will approximately be 800 bytes long
                                        //Adding the url size to it because it is included in req result
#endif

instance_prop_t _nc_instance_properties;

struct req_type_info_t{
    char filename[MAX_FILE_PATH_LENGTH];
    time_t req_time;
};

struct req_memory {
    char memory[REQ_MEM_SIZE];
    size_t size;
};

#ifdef CACHING
bool file_is_cached(const char *filename, struct req_type_info_t* req_info) {
    bool same_filename = !strcmp(filename, req_info->filename);
    bool expired = (time(NULL) - req_info->req_time) > CACHING_TIME;
    return (same_filename && !expired);
}
#endif

size_t propfind_memory_callback(void *contents, size_t size, size_t nmemb, void *userp) {
    size_t realsize = size * nmemb;
    struct req_memory *mem = (struct req_memory *) userp;

    memcpy(&(mem->memory[mem->size]), contents, realsize);
    mem->size += realsize;
    mem->memory[mem->size] = 0;

    return realsize;
}

size_t download_file_memory_callback(void *contents, size_t size, size_t nmemb, void *userp) {
    size_t realsize = size * nmemb;
    FILE *fp = (FILE *) userp;
    fwrite(contents, realsize, size, fp);

    return realsize;
}

void setCurlOptions(CURL* curl_handle, const char *reqURL) {
    curl_easy_setopt(curl_handle, CURLOPT_URL, reqURL);
    curl_easy_setopt(curl_handle, CURLOPT_USERPWD, _nc_instance_properties.authentication);
    curl_easy_setopt(curl_handle, CURLOPT_FOLLOWLOCATION, 1L);
    curl_easy_setopt(curl_handle, CURLOPT_HTTPAUTH, CURLAUTH_BASIC);
}


char *generateReqUrl(const char *filename) {
    static char fileurl[MAX_URL_LENGTH];
    const unsigned long strsize = strlen(filename) + strlen(_nc_instance_properties.dav_url) + TERMINATING_CHAR_SIZE;
    memset(fileurl, TERMINATING_CHAR, strsize);
    strcat(fileurl, _nc_instance_properties.dav_url);
    strcat(fileurl, filename);
    return fileurl;
}


char *create_req_body(enum req_prop_type_t req_type) {
#ifdef CACHING
    return propfind_request_all;
#else
    static char req_body[220];
    const char* req_body_attr = req_body_attributes[req_type];
    const unsigned long sizeof_req_body_attr = strlen(req_body_attr);
    const unsigned long sizeof_empty_req_body = strlen(empty_req_body);
    const unsigned long req_body_size = sizeof_req_body_attr + sizeof_empty_req_body + TERMINATING_CHAR_SIZE;
    snprintf(req_body, req_body_size, empty_req_body, req_body_attr);
    return req_body;
#endif

}

int propfind_req(const char *filename, enum req_prop_type_t req_prop_type) {
    static struct req_memory g_chunk;
    char *post_field;
    CURLcode curl_status;
#ifdef CACHING
    static struct req_type_info_t req_info;

    if (file_is_cached(filename, &req_info))
        return CURLE_OK;

    req_info.req_time = time(NULL);
    strcpy(req_info.filename, filename);
#endif
    post_field = create_req_body(req_prop_type);
    char *url = generateReqUrl(filename);
    CURL* curl_handle = curl_easy_init();
    g_chunk.size = 0;
    if (curl_handle) {
        setCurlOptions(curl_handle, url);
        curl_easy_setopt(curl_handle, CURLOPT_WRITEFUNCTION, propfind_memory_callback);
        curl_easy_setopt(curl_handle, CURLOPT_WRITEDATA, (void *) &g_chunk);
        curl_easy_setopt(curl_handle, CURLOPT_CUSTOMREQUEST, "PROPFIND");
        curl_easy_setopt(curl_handle, CURLOPT_POST, 1L);
        curl_easy_setopt(curl_handle, CURLOPT_POSTFIELDS, post_field);
        curl_status = curl_easy_perform(curl_handle);
    }
    curl_easy_cleanup(curl_handle);
    preparse_propfind_resp(g_chunk.memory, req_prop_type);

    return curl_status;
}

int download_req(const char *filename, const char *loc) {
int curl_status;
#ifdef CACHING
    static struct req_type_info_t req_info;

    if (file_is_cached(filename, &req_info))
        return 0;

    req_info.req_time = time(NULL);
    strcpy(req_info.filename, filename);
#endif
    char *url = generateReqUrl(filename);
    CURL* curl_handle = curl_easy_init();
    if (curl_handle) {
        FILE *fp;
        fp = fopen(loc, "w");

        setCurlOptions(curl_handle, url);
        curl_easy_setopt(curl_handle, CURLOPT_WRITEFUNCTION, download_file_memory_callback);
        curl_easy_setopt(curl_handle, CURLOPT_WRITEDATA, (void *) fp);
        curl_status = curl_easy_perform(curl_handle);

        fclose(fp);
    }
    curl_easy_cleanup(curl_handle);
    return curl_status;
}