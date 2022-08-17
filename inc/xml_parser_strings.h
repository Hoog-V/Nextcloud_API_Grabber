//
// Created by victor on 17-8-22.
//

#ifndef NEXTCLOUD_API_GRABBER_XML_PARSER_STRINGS_H
#define NEXTCLOUD_API_GRABBER_XML_PARSER_STRINGS_H

const char *oc_size_begin_tag = "<oc:size>";
const char *oc_size_end_tag = "</oc:size>";

const char *get_lastmodified_begin_tag = "<d:getlastmodified>";
const char *get_lastmodified_end_tag = "</d:getlastmodified>";

const char *get_content_type_begin_tag = "<d:getcontenttype>";
const char *get_content_type_end_tag = "</d:getcontenttype>";

const char *get_etag_begin_tag = "<d:getetag>";
const char *get_etag_end_tag = "</d:getetag>";

const char *resource_type_begin_tag = "<d:resourcetype>";
const char *resource_type_end_tag = "</d:resourcetype>";

const char *file_id_begin_tag = "<oc:fileid>";
const char *file_id_end_tag = "</oc:fileid>";

const char* permissions_begin_tag = "<oc:permissions>";
const char* permissions_end_tag = "</oc:permissions>";

const char *get_content_length_begin_tag = "<d:getcontentlength>";
const char *get_content_length_end_tag = "</d:getcontentlength>";

const char *has_preview_begin_tag = "<nc:has-preview>";
const char *has_preview_end_tag = "</nc:has-preview>";

const char *favorite_begin_tag = "<oc:favorite>";
const char *favorite_end_tag = "</oc:favorite>";

const char *comments_unread_begin_tag = "<oc:comments-unread>";
const char *comments_unread_end_tag = "</oc:comments-unread>";

const char *owner_display_name_begin_tag = "<oc:owner-display-name>";
const char *owner_display_name_end_tag = "</oc:owner-display-name>";


#endif //NEXTCLOUD_API_GRABBER_XML_PARSER_STRINGS_H
