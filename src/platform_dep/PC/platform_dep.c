#include <web_requests.h>
#include <web_request_tools.h>
#include <time.h>
#include <request_parser.h>
#include <curl/curl.h>
#include <sys/stat.h>
#ifdef CACHING

#include <stdbool.h>

#define REQ_MEM_SIZE 800+MAX_URL_LENGTH //A empty req result(caching) will approximately be 800 bytes long
//Adding the url size to it because it is included in req result
struct req_type_info_t {
    char filename[MAX_FILE_PATH_LENGTH];
    time_t req_time;
};

#else
#define REQ_MEM_SIZE 360+MAX_URL_LENGTH //A empty req result(caching) will approximately be 800 bytes long
//Adding the url size to it because it is included in req result
#endif


struct req_memory {
    char memory[REQ_MEM_SIZE];
    size_t size;
};

#ifdef CACHING

bool file_is_cached(const char *filename, struct req_type_info_t *req_info) {
    bool same_filename = !strcmp(filename, req_info->filename);
    bool expired = (time(NULL) - req_info->req_time) > CACHING_TIME;
    return (same_filename && !expired);
}

#endif

const char *get_error_msg(const int error_code) {
    if (error_code == FILE_WRITE_ERROR)
        return "Error writing to file..";
    else if (error_code == FILE_READ_ERROR)
        return "Error reading file; does the file exist?";
    else if (error_code == PARSING_ERROR)
        return "Error parsing XML request..";
    return curl_easy_strerror(error_code);
}

size_t propfind_memory_callback(void *contents, size_t size, size_t nmemb, void *userp) {
    size_t real_size = size * nmemb;
    struct req_memory *mem = (struct req_memory *) userp;

    memcpy(&(mem->memory[mem->size]), contents, real_size);
    mem->size += real_size;
    mem->memory[mem->size] = 0;

    return real_size;
}

size_t download_file_memory_callback(void *contents, size_t size, size_t nmemb, void *userp) {
    size_t real_size = size * nmemb;
    FILE *fp = (FILE *) userp;
    fwrite(contents, real_size, size, fp);

    return real_size;
}

size_t read_callback(void *contents, size_t size, size_t nmemb, void *userp)
{
    size_t real_size = size * nmemb;

    FILE *fp = (FILE *) userp;
    fread(contents, real_size, size, fp);

    return real_size;
}


void setCurlOptions(CURL *curl_handle, const char *reqURL) {
    curl_easy_setopt(curl_handle, CURLOPT_URL, reqURL);
    curl_easy_setopt(curl_handle, CURLOPT_USERNAME, _nc_instance_properties.username);
    curl_easy_setopt(curl_handle, CURLOPT_PASSWORD, _nc_instance_properties.password);
    curl_easy_setopt(curl_handle, CURLOPT_FOLLOWLOCATION, 1L);
    curl_easy_setopt(curl_handle, CURLOPT_HTTPAUTH, CURLAUTH_BASIC);
}

const int propfind_req(const char *filename, const enum req_prop_type_t req_prop_type) {
    static struct req_memory g_chunk;
    char *post_field;
    CURLcode curl_status;
#ifdef CACHING
    static struct req_type_info_t req_info;

    if (file_is_cached(filename, &req_info))
        return CURLE_OK;
#endif

    post_field = create_req_body(req_prop_type);
    char *url = generateReqUrl(filename);
    CURL *curl_handle = curl_easy_init();
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

#ifdef CACHING
    if (curl_status == CURLE_OK) {
        req_info.req_time = time(NULL);
        strcpy(req_info.filename, filename);
        int xml_status = preparse_propfind_resp(g_chunk.memory, req_prop_type);
        return xml_status;
    }
#endif
    return curl_status;

}

const int download_req(const char *filename, const char *loc) {
    int curl_status;
#ifdef CACHING
    static struct req_type_info_t req_info;

    if (file_is_cached(filename, &req_info))
        return 0;
#endif
    char *url = generateReqUrl(filename);
    CURL *curl_handle = curl_easy_init();
    if (curl_handle) {
        FILE *fp;
        fp = fopen(loc, "w");
        if (fp == NULL) {
            ERROR_HANDLER("Writing file failed!");
            curl_easy_cleanup(curl_handle);
            return FILE_WRITE_ERROR;
        }
        setCurlOptions(curl_handle, url);
        curl_easy_setopt(curl_handle, CURLOPT_WRITEFUNCTION, download_file_memory_callback);
        curl_easy_setopt(curl_handle, CURLOPT_WRITEDATA, (void *) fp);
        curl_status = curl_easy_perform(curl_handle);

        fclose(fp);
    }
    curl_easy_cleanup(curl_handle);

#ifdef CACHING
    if (curl_status == CURLE_OK) {
        req_info.req_time = time(NULL);
        strcpy(req_info.filename, filename);
    }
#endif
    return curl_status;
}

int upload_req(const char *filesystem_path, const char *nc_path) {
    int curl_status;
#ifdef CACHING
    static struct req_type_info_t req_info;

    if (file_is_cached(nc_path, &req_info))
        return 0;
#endif
    struct stat file_info;
    stat(filesystem_path, &file_info);
    char *url = generateReqUrl(nc_path);
    CURL *curl_handle = curl_easy_init();
    if (curl_handle) {
        FILE *fp;
        fp = fopen(filesystem_path, "rb");
        if (fp == NULL) {
            ERROR_HANDLER("Reading file failed!");
            curl_easy_cleanup(curl_handle);
            return FILE_READ_ERROR;
        }
        setCurlOptions(curl_handle, url);
        curl_easy_setopt(curl_handle, CURLOPT_READFUNCTION, read_callback);
        curl_easy_setopt(curl_handle, CURLOPT_WRITEDATA, (void *) fp);
        /* enable uploading (implies PUT over HTTP) */
        curl_easy_setopt(curl_handle, CURLOPT_UPLOAD, 1L);
        /* now specify which file to upload */
        curl_easy_setopt(curl_handle, CURLOPT_READDATA, filesystem_path);
        /* provide the size of the upload, we specicially typecast the value
   to curl_off_t since we must be sure to use the correct data size */
        curl_easy_setopt(curl_handle, CURLOPT_INFILESIZE_LARGE,
                         (curl_off_t)file_info.st_size);

        curl_status = curl_easy_perform(curl_handle);

        fclose(fp);
    }
    curl_easy_cleanup(curl_handle);
#ifdef CACHING
    if (curl_status == CURLE_OK) {
        req_info.req_time = time(NULL);
        strcpy(req_info.filename, nc_path);
    }
#endif
}