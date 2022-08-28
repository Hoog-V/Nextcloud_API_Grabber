cmake_minimum_required(VERSION 3.20)

file(GLOB_RECURSE SOURCES "src/base/*.c")
if(ESP_PLATFORM)
    idf_component_register(SRCS ${SOURCES} "src/platform_dep/ESP32/platform_dep.c"
                           INCLUDE_DIRS "inc"
                           REQUIRES esp-tls nvs_flash esp_http_client protocol_examples_common spiffs)
else()
    project(NEXTCLOUD_API_GRABBER C)
    file(GLOB_RECURSE INCLUDES "inc/*.h")
    INCLUDE_DIRECTORIES(${PROJECT_SOURCE_DIR}/inc)
    add_library(Nextcloud_API_Grabber ${SOURCES} "src/platform_dep/PC/platform_dep.c" ${INCLUDES})
    add_subdirectory(examples)
endif()