#ifndef _NGX_HTTP_DONKEY_RUN_MODULE_H_INCLUDE_
#define _NGX_HTTP_DONKEY_RUN_MODULE_H_INCLUDE_

#include <ngx_config.h>
#include <ngx_core.h>
#include <ngx_http.h>


typedef struct {
    ngx_flag_t                         enable;
} ngx_http_donkey_run_loc_conf_t;

typedef enum {
    QUERY_TYPE_IMPORT = 0

} ngx_dr_query_type;

typedef struct {
    ngx_flag_t          enable;
    ngx_dr_query_type   type;

} ngx_http_donkey_run_ctx_t;

#endif
