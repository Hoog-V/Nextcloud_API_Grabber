#ifndef NEXTCLOUD_API_GRABBER_PROPFIND_ATTR_H
#define NEXTCLOUD_API_GRABBER_PROPFIND_ATTR_H

#ifdef CACHING
char *propfind_request_all = "<?xml version=\"1.0\"?>\n"
                             "<d:propfind  xmlns:d=\"DAV:\" xmlns:oc=\"http://owncloud.org/ns\" xmlns:nc=\"http://nextcloud.org/ns\">\n"
                             "  <d:prop>\n"
                             "        <d:getlastmodified />\n"
                             "        <d:getetag />\n"
                             "        <d:getcontenttype />\n"
                             "        <oc:fileid />\n"
                             "        <oc:permissions />\n"
                             "        <oc:size />\n"
                             "        <d:getcontentlength />\n"
                             "        <nc:has-preview />\n"
                             "        <oc:favorite />\n"
                             "        <oc:comments-unread />\n"
                             "        <oc:owner-display-name />\n"
                             "  </d:prop>\n"
                             "</d:propfind>";

#else
    const char *empty_req_body = "<?xml version=\"1.0\"?>\n"
                             "<d:propfind  xmlns:d=\"DAV:\" xmlns:oc=\"http://owncloud.org/ns\" xmlns:nc=\"http://nextcloud.org/ns\">"
                             "  <d:prop>\n"
                             "       <%s />\n"
                             "  </d:prop>\n"
                             "</d:propfind>";
    const char req_body_attributes[][25] = {"d:getlastmodified","d:getetag", "d:getcontenttype",
                                        "oc:fileid", "oc:permissions", "oc:size", "d:getcontentlength","nc:has-preview",
                                        "oc:favorite", "oc:comments-unread", "oc:owner-display-name"};
#endif



#endif //NEXTCLOUD_API_GRABBER_PROPFIND_ATTR_H
