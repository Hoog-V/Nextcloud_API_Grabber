cmake_minimum_required(VERSION 3.20)

#INCLUDE_DIRECTORIES(${PROJECT_SOURCE_DIR}/inc)


#add_library(Nextcloud_API_Grabber ${SOURCES} ${INCLUDES})

#add_subdirectory(examples)

if(ESP_PLATFORM)
    file(GLOB_RECURSE SOURCES "src/base/*.c")
    idf_component_register(SRCS ${SOURCES} "src/platform_dep/ESP32/platform_dep.c"
                           INCLUDE_DIRS "inc"
                           REQUIRES esp-tls nvs_flash esp_http_client protocol_examples_common)

endif()