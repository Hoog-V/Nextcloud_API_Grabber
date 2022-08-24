#include <web_requests.h>
#include <web_request_tools.h>
#include <time.h>
#include <request_parser.h>

#ifdef CACHING

#define REQ_MEM_SIZE 800+MAX_URL_LENGTH //A empty req result(caching) will approximately be 800 bytes long
                                        //Adding the url size to it because it is included in req result
struct req_type_info_t{
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
bool file_is_cached(const char *filename, struct req_type_info_t* req_info) {
    bool same_filename = !strcmp(filename, req_info->filename);
    bool expired = (time(NULL) - req_info->req_time) > CACHING_TIME;
    return (same_filename && !expired);
}
#endif

const char *get_error_msg(const int error_code){
    if(error_code == FILE_WRITE_ERROR)
        return "Error writing to file..";
    else if (error_code == PARSING_ERROR)
        return "Error parsing XML request..";
    return "";
}
int propfind_req(const char *filename, const enum req_prop_type_t req_prop_type) {
    return 0;
}

int download_req(const char *filename, const char *loc) {
    return 0;
}
