//
// Created by victor on 17-8-22.
//

#ifndef NEXTCLOUD_API_GRABBER_PROPFIND_ATTR_H
#define NEXTCLOUD_API_GRABBER_PROPFIND_ATTR_H

#ifdef CACHING
char *propfind_request_all = "<?xml version=\"1.0\"?>\n"
                             "<d:propfind  xmlns:d=\"DAV:\" xmlns:oc=\"http://owncloud.org/ns\" xmlns:nc=\"http://nextcloud.org/ns\">\n"
                             "  <d:prop>\n"
                             "        <d:getlastmodified />\n"
                             "        <d:getetag />\n"
                             "        <d:getcontenttype />\n"
                             "        <d:resourcetype />\n"
                             "        <oc:fileid />\n"
                             "        <oc:permissions />\n"
                             "        <oc:size />\n"
                             "        <d:getcontentlength />\n"
                             "        <nc:has-preview />\n"
                             "        <oc:favorite />\n"
                             "        <oc:comments-unread />\n"
                             "        <oc:owner-display-name />\n"
                             "        <oc:share-types />\n"
                             "        <nc:contained-folder-count />\n"
                             "        <nc:contained-file-count />\n"
                             "  </d:prop>\n"
                             "</d:propfind>";


#endif

const char *empty_req_body = "<?xml version=\"1.0\"?>\n"
                             "<d:propfind  xmlns:d=\"DAV:\" xmlns:oc=\"http://owncloud.org/ns\" xmlns:nc=\"http://nextcloud.org/ns\">\n"
                             "  <d:prop>\n"
                             "       %s\n"
                             "  </d:prop>\n"
                             "</d:propfind>";

char *oc_size_attr = "<oc:size />";
char *get_lastmodified_attr = "<d:getlastmodified />";
char *get_contenttype_attr = "<d:getcontenttype />";

#endif //NEXTCLOUD_API_GRABBER_PROPFIND_ATTR_H
