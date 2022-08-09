#include <web_requests.h>

#define MAX_URL_LENGTH 2048
#define TERMINATING_CHAR '\0'
#define TERMINATING_CHAR_SIZE 1

struct req_memory g_chunk;

const char *empty_req_body = "<?xml version=\"1.0\"?>\n"
                                "<d:propfind  xmlns:d=\"DAV:\" xmlns:oc=\"http://owncloud.org/ns\" xmlns:nc=\"http://nextcloud.org/ns\">\n"
                                "  <d:prop>\n"
                                "       %s\n"
                                "  </d:prop>\n"
                                "</d:propfind>";


char *oc_size_attr = "<oc:size />";
char *get_lastmodified_attr = "<d:getlastmodified />";
char *get_contenttype_attr = "<d:getcontenttype />";

instance_prop_t _instance_properties;

size_t curl_write_memory_callback(void *contents, size_t size, size_t nmemb, void *userp) {
    size_t realsize = size * nmemb;
    struct req_memory *mem = (struct req_memory *) userp;

    memcpy(&(mem->memory[mem->size]), contents, realsize);
    mem->size += realsize;
    mem->memory[mem->size] = 0;

    return realsize;
}

void setCurlOptions(struct req_memory *chunk, const char* reqURL){
    curl_easy_setopt(chunk->curl_handle, CURLOPT_URL, reqURL);
    curl_easy_setopt(chunk->curl_handle, CURLOPT_USERPWD, _instance_properties.authentication);
    curl_easy_setopt(chunk->curl_handle, CURLOPT_FOLLOWLOCATION, 1L);
    curl_easy_setopt(chunk->curl_handle, CURLOPT_WRITEFUNCTION, curl_write_memory_callback);
    curl_easy_setopt(chunk->curl_handle, CURLOPT_WRITEDATA, (void *) chunk);
    curl_easy_setopt(chunk->curl_handle, CURLOPT_HTTPAUTH, CURLAUTH_BASIC);
}


char *generateReqUrl(const char *filename) {
    static char fileurl[MAX_URL_LENGTH];
    const int strsize = strlen(filename) + strlen(_instance_properties.dav_url) + TERMINATING_CHAR_SIZE;
    memset(fileurl, TERMINATING_CHAR, strsize);
    strcat(fileurl, _instance_properties.dav_url);
    strcat(fileurl, filename);
    return fileurl;
}


char *create_req_body(req_prop_type_t req_type) {
    static char req_body[220];
    char *req_body_attr;
    switch (req_type) {
        case oc_size:
            req_body_attr = oc_size_attr;
            break;
        case get_lastmodified:
            req_body_attr = get_lastmodified_attr;
            break;
        case get_contenttype:
            req_body_attr = get_contenttype_attr;
            break;
        default:
            return NULL;
    }
    const int sizeof_req_body_attr = strlen(req_body_attr);
    const int sizeof_empty_req_body = strlen(empty_req_body);
    const int req_body_size = sizeof_req_body_attr + sizeof_empty_req_body + 1;
    snprintf(req_body, req_body_size, empty_req_body, req_body_attr);
    return req_body;
}


struct req_memory *propfind_req(const char *filename, req_prop_type_t ReqType) {
    char *post_field = create_req_body(ReqType);
    char *url = generateReqUrl(filename);
    g_chunk.curl_handle = curl_easy_init();
    if (g_chunk.curl_handle) {
        setCurlOptions(&g_chunk, url);
        curl_easy_setopt(g_chunk.curl_handle, CURLOPT_CUSTOMREQUEST, "PROPFIND");
        curl_easy_setopt(g_chunk.curl_handle, CURLOPT_POST, 1L);
        curl_easy_setopt(g_chunk.curl_handle, CURLOPT_POSTFIELDS, post_field);
        g_chunk.curl_status = curl_easy_perform(g_chunk.curl_handle);
    }
    return &g_chunk;
}

struct req_memory *get_req(const char *filename) {
    char *url = generateReqUrl(filename);

    g_chunk.curl_handle = curl_easy_init();
    if (g_chunk.curl_handle) {
        setCurlOptions(&g_chunk, url);
        g_chunk.curl_status = curl_easy_perform(g_chunk.curl_handle);
    }
    return &g_chunk;
}