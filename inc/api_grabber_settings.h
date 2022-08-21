#ifndef NEXTCLOUD_API_GRABBER_API_GRABBER_SETTINGS_H
#define NEXTCLOUD_API_GRABBER_API_GRABBER_SETTINGS_H

/* Library settings Macros
 * Changing these settings will affect the memory footprint and performance of this library
 */

/* Enable request caching.
 * This means when requesting file-properties like size, last-modified e.g. all properties will be requested at once...
 * Using this will give a huge boost in performance and will be more power efficient (less requests means less energy :))
 * It will also save space because the cache where the request data will be saved in, will be reused till it's time to refresh
 * Uncomment the macro to disable
 */
#define CACHING
//The time a particular result is kept cached
#define CACHING_TIME 15*60
//The maximum length in characters of any URL in the library (with the webdav section of url included).
#define MAX_URL_LENGTH 512
//The maximum length in characters of any filepath used in the library.
#define MAX_FILE_PATH_LENGTH 200


#endif //NEXTCLOUD_API_GRABBER_API_GRABBER_SETTINGS_H
