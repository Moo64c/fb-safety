core = 7.x
api = 2

; Modules
projects[admin_menu][subdir] = "contrib"
projects[admin_menu][version] = "3.0-rc4"

projects[admin_views][subdir] = "contrib"
projects[admin_views][version] = "1.3"

projects[ctools][subdir] = "contrib"
projects[ctools][version] = "1.4"

projects[date][subdir] = "contrib"
projects[date][version] = "2.8"

projects[devel][subdir] = "contrib"
projects[devel][version] = "1.5"

projects[entity][subdir] = "contrib"
projects[entity][version] = "1.5"

projects[entityreference][subdir] = "contrib"
projects[entityreference][version] = "1.1"

projects[entityreference_prepopulate][subdir] = "contrib"
projects[entityreference_prepopulate][version] = "1.5"
projects[entityreference_prepopulate][patch][] = "https://drupal.org/files/issues/1994702-values-from-cache-20.patch"

projects[features][subdir] = "contrib"
projects[features][version] = "2.2"

projects[field_collection][subdir] = "contrib"
projects[field_collection][version] = "1.0-beta5"

projects[flag][subdir] = "contrib"
projects[flag][version] = "2.2"

projects[inline_entity_form][subdir] = "contrib"
projects[inline_entity_form][version] = "1.5"

projects[jquery_update][subdir] = "contrib"
projects[jquery_update][version] = "2.4"

projects[libraries][subdir] = "contrib"
projects[libraries][version] = "2.2"

projects[mailsystem][version] = 2.34
projects[mailsystem][subdir] = "contrib"

projects[mailchimp][subdir] = "contrib"
projects[mailchimp][version] = 2.12

projects[message][subdir] = "contrib"
projects[message][version] = "1.9"

projects[message_notify][subdir] = "contrib"
projects[message_notify][version] = "2.5"

projects[message_subscribe][subdir] = "contrib"
projects[message_subscribe][version] = "1.x-dev"

projects[mimemail][subdir] = "contrib"
projects[mimemail][version] = "1.0-alpha1"
projects[mimemail][patch][] = "http://drupal.org/files/rules-1585546-1-moving_rules_actions.patch"
projects[mimemail][patch][] = "http://drupal.org/files/compress_install_missing_value.patch"

projects[module_filter][subdir] = "contrib"
projects[module_filter][version] = "2.0-alpha2"

projects[panels][subdir] = "contrib"
projects[panels][version] = "3.4"

projects[pathauto][subdir] = "contrib"
projects[pathauto][version] = "1.2"

projects[purl][subdir] = "contrib"
projects[purl][version] = "1.x-dev"

projects[restful][subdir] = "contrib"
projects[restful][version] = "1.0-rc5"

projects[strongarm][subdir] = "contrib"
projects[strongarm][version] = "2.0"

projects[title][subdir] = "contrib"
projects[title][version] = "1.0-alpha7"

projects[token][subdir] = "contrib"
projects[token][version] = "1.5"

projects[views][subdir] = "contrib"
projects[views][version] = "3.8"

projects[views_bulk_operations][subdir] = "contrib"
projects[views_bulk_operations][version] = "3.2"

projects[wysiwyg][subdir] = "contrib"
projects[wysiwyg][version] = "2.2"

; Themes
projects[bootstrap][subdir] = "contrib"
projects[bootstrap][version] = "3.0"

; Libraries
libraries[tinymce][type] = "libraries"
libraries[tinymce][download][type] = "get"
libraries[tinymce][download][url] = "http://download.moxiecode.com/tinymce/tinymce_3.5.11.zip"

libraries[bootstrap_sass][download][type] = "file"
libraries[bootstrap_sass][type] = "libraries"
libraries[bootstrap_sass][download][url] = "https://github.com/twbs/bootstrap-sass/archive/v3.1.1.zip"

libraries[mailchimp][download][type] = "file"
libraries[mailchimp][type] = "libraries"
libraries[mailchimp][download][url] = "http://apidocs.mailchimp.com/api/downloads/mailchimp-api-class.zip"

libraries[facebookphp][download][type] = "file"
libraries[facebookphp][type] = "libraries"
libraries[facebookphp][download][url] = "https://github.com/facebook/facebook-php-sdk-v4/archive/4.0-dev.zip"
