#ifndef _NGX_HTTP_DONKEY_RUN_COMMON_INCLUDE_H_
#define _NGX_HTTP_DONKEY_RUN_COMMON_INCLUDE_H_

#include <ngx_core.h>
#include <ngx_http.h>

ngx_int_t ngx_http_donkey_get_header(ngx_http_request_t *r, ngx_str_t *key, ngx_str_t *val);

#endif
