#include <web_requests.h>
#include <web_request_tools.h>
#include <time.h>
#include <request_parser.h>

#include "esp_log.h"
#include "esp_system.h"

#include "esp_tls.h"
#include "esp_crt_bundle.h"
#include "esp_http_client.h"
#include "esp_err.h"
#include "esp_spiffs.h"

static const char *TAG = "WEB_REQUESTS";

#ifdef CACHING

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

esp_err_t _http_event_handler(esp_http_client_event_t *evt) {
    static int output_len;       // Stores number of bytes read
    switch (evt->event_id) {
        case HTTP_EVENT_ERROR:
            break;
        case HTTP_EVENT_ON_CONNECTED:
            break;
        case HTTP_EVENT_HEADER_SENT:
            break;
        case HTTP_EVENT_ON_HEADER:
            break;
        case HTTP_EVENT_ON_DATA:
            if (esp_http_client_is_chunked_response(evt->client)) {
                struct req_memory *g_chunk = (struct req_memory *) evt->user_data;
                memcpy(g_chunk->memory + output_len, evt->data, evt->data_len);
                output_len += evt->data_len;
            }
                break;
                case HTTP_EVENT_ON_FINISH: {
                    struct req_memory *g_chunk = (struct req_memory *) evt->user_data;
                    g_chunk->memory[output_len] = '\0';
                    g_chunk->size = output_len;
                    output_len = 0;
                    break;
                }
                case HTTP_EVENT_DISCONNECTED: {
                    int mbedtls_err = 0;
                    esp_err_t err = esp_tls_get_and_clear_last_error(evt->data, &mbedtls_err, NULL);
                    if (err != 0) {
                        output_len = 0;
                        ESP_LOGI(TAG, "Last esp error code: 0x%x", err);
                        ESP_LOGI(TAG, "Last mbedtls failure: 0x%x", mbedtls_err);
                    }
                    break;
                }
            }
            return ESP_OK;
    }


esp_err_t _download_event_handler(esp_http_client_event_t *evt) {
     static int output_len;       // Stores number of bytes read
    switch (evt->event_id) {
        case HTTP_EVENT_ERROR:
            break;
        case HTTP_EVENT_ON_CONNECTED:
            break;
        case HTTP_EVENT_HEADER_SENT:
            break;
        case HTTP_EVENT_ON_HEADER:
            break;
        case HTTP_EVENT_ON_DATA:
        {
            if (!esp_http_client_is_chunked_response(evt->client)) {
        FILE *fp = (FILE *) evt->user_data;
        fwrite(evt->data,1,evt->data_len,fp);
        output_len += evt->data_len;
            }
        }
        break;
                case HTTP_EVENT_ON_FINISH: {
                    output_len = 0;
                    break;
                }
                case HTTP_EVENT_DISCONNECTED: {
                    int mbedtls_err = 0;
                    esp_err_t err = esp_tls_get_and_clear_last_error(evt->data, &mbedtls_err, NULL);
                    if (err != 0) {
                        output_len = 0;
                        ESP_LOGI(TAG, "Last esp error code: 0x%x", err);
                        ESP_LOGI(TAG, "Last mbedtls failure: 0x%x", mbedtls_err);
                    }
                    break;
                }
            }
            return ESP_OK;
    }


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
        else if (error_code == PARSING_ERROR)
            return "Error parsing XML request..";
        return esp_err_to_name(error_code);
    }

    int propfind_req(const char *filename, const enum req_prop_type_t req_prop_type) {
        struct req_memory g_chunk;
        char *post_field;
#ifdef CACHING
        static struct req_type_info_t req_info;

        if (file_is_cached(filename, &req_info))
            return 0;
#endif
        post_field = create_req_body(req_prop_type);
        char *url = generateReqUrl(filename);
        g_chunk.size = 0;
        esp_http_client_config_t config = {
                .url = url,
                .event_handler = _http_event_handler,
                .auth_type = HTTP_AUTH_TYPE_BASIC,
                .username = _nc_instance_properties.username,
                .password = _nc_instance_properties.password,
                .method = HTTP_METHOD_PROPFIND,
                .transport_type = HTTP_TRANSPORT_OVER_SSL,
                .crt_bundle_attach = esp_crt_bundle_attach,
                .user_data = &g_chunk,       // Pass address of local buffer to get response
        };
        esp_http_client_handle_t client = esp_http_client_init(&config);
        esp_http_client_set_post_field(client, post_field, strlen(post_field));
        esp_err_t err = esp_http_client_perform(client);
        esp_http_client_cleanup(client);

#ifdef CACHING
        if (err == ESP_OK) {
            req_info.req_time = time(NULL);
            strcpy(req_info.filename, filename);
            int xml_status = preparse_propfind_resp(g_chunk.memory, req_prop_type);
            return xml_status;
        }
#endif

        return err;
    }

    int download_req(const char *filename, const char *loc) {
        // Use settings defined above to initialize and mount SPIFFS filesystem.
        // Note: esp_vfs_spiffs_register is an all-in-one convenience function.
#ifdef CACHING
        static struct req_type_info_t req_info;

        if (file_is_cached(filename, &req_info))
            return 0;
#endif
        char *url = generateReqUrl(filename);
        remove(loc);
        FILE *fp;
        fp = fopen(loc, "w");
        if (fp == NULL) {
            ERROR_HANDLER("Writing file failed!");
            return FILE_WRITE_ERROR;
        }
        esp_http_client_config_t config = {
                .url = url,
                .event_handler = _download_event_handler,
                .auth_type = HTTP_AUTH_TYPE_BASIC,
                .username = _nc_instance_properties.username,
                .password = _nc_instance_properties.password,
                .method = HTTP_METHOD_GET,
                .transport_type = HTTP_TRANSPORT_OVER_SSL,
                .crt_bundle_attach = esp_crt_bundle_attach,
                .user_data = fp,
        };
        esp_http_client_handle_t client = esp_http_client_init(&config);
        esp_err_t err = esp_http_client_perform(client);
        esp_http_client_cleanup(client);
        fclose(fp);
#ifdef CACHING
        if (err == ESP_OK) {
            req_info.req_time = time(NULL);
            strcpy(req_info.filename, filename);
        }
#endif
        return 0;
    }
