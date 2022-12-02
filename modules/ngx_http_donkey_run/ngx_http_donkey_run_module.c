#include <ngx_config.h>
#include <ngx_core.h>
#include <ngx_http.h>

#include "ngx_http_donkey_run_module.h"
#include "ngx_http_donkey_run_common.h"
#include "run.h"

static void *ngx_http_donkey_run_create_loc_conf(ngx_conf_t *cf);
static char *ngx_http_donkey_run_merge_loc_conf(ngx_conf_t *cf, void *parent, void *child);
static ngx_int_t ngx_http_donkey_run_access_handler(ngx_http_request_t *r);

static ngx_int_t ngx_http_donkey_run_init(ngx_conf_t *cf);


static ngx_command_t  ngx_http_donkey_run_commands[] = {

    { ngx_string("donkey_run"),
      NGX_HTTP_MAIN_CONF|NGX_HTTP_SRV_CONF|NGX_HTTP_LOC_CONF|NGX_CONF_FLAG,
      ngx_conf_set_flag_slot,
      NGX_HTTP_LOC_CONF_OFFSET,
      offsetof(ngx_http_donkey_run_loc_conf_t, enable),
      NULL
    },

    ngx_null_command
};


static ngx_http_module_t  ngx_http_donkey_run_module_ctx = {
    NULL,                               /* proconfiguration */
    ngx_http_donkey_run_init,        /* postconfiguration */

    NULL,                               /* create main configuration */
    NULL,                               /* init main configuration */

    NULL,                               /* create server configuration */
    NULL,                               /* merge server configuration */

    ngx_http_donkey_run_create_loc_conf,    /* create location configuration */
    ngx_http_donkey_run_merge_loc_conf      /* merge location configuration */
};


ngx_module_t  ngx_http_donkey_run_module = {
    NGX_MODULE_V1,
    &ngx_http_donkey_run_module_ctx, /* module context */
    ngx_http_donkey_run_commands,    /* module directives */
    NGX_HTTP_MODULE,                    /* module type */
    NULL,                               /* init master */
    NULL,                               /* init module */
    NULL,                               /* init process */
    NULL,                               /* init thread */
    NULL,                               /* exit thread */
    NULL,                               /* exit process */
    NULL,                               /* exit master */
    NGX_MODULE_V1_PADDING
};


static ngx_http_output_header_filter_pt ngx_http_next_header_filter;
static ngx_http_output_body_filter_pt   ngx_http_next_body_filter;


static ngx_int_t
ngx_http_donkey_run_header_filter(ngx_http_request_t *r)
{
    // ngx_http_donkey_run_ctx_t       *ctx;
    ngx_http_donkey_run_loc_conf_t  *drcf;

    drcf = ngx_http_get_module_loc_conf(r, ngx_http_donkey_run_module);

    if (!drcf->enable) {
        return ngx_http_next_header_filter(r);
    }

    return ngx_http_next_header_filter(r);
}


static ngx_int_t
ngx_http_donkey_run_body_filter(ngx_http_request_t *r, ngx_chain_t *in)
{
    ngx_http_donkey_run_ctx_t *ctx;

    ctx = ngx_http_get_module_ctx(r, ngx_http_donkey_run_module);
    if (ctx == NULL) {
        return ngx_http_next_body_filter(r, in);
    }

    return ngx_http_next_body_filter(r, in);
}

static void *
ngx_http_donkey_run_create_loc_conf(ngx_conf_t *cf)
{
    ngx_http_donkey_run_loc_conf_t  *conf;

    conf = ngx_pcalloc(cf->pool, sizeof(ngx_http_donkey_run_loc_conf_t));
    if (conf == NULL) {
        return NULL;
    }

    return conf;
}

static char *
ngx_http_donkey_run_merge_loc_conf(ngx_conf_t *cf, void *parent, void *child)
{
    ngx_http_donkey_run_loc_conf_t  *prev = parent;
    ngx_http_donkey_run_loc_conf_t  *conf = child;

    ngx_conf_merge_value(conf->enable, prev->enable, 0);

    return NGX_CONF_OK;
}


static ngx_int_t
ngx_http_donkey_run_init(ngx_conf_t *cf)
{
    ngx_http_handler_pt       *h;
    ngx_http_core_main_conf_t *cmcf;

    cmcf = ngx_http_conf_get_module_main_conf(cf, ngx_http_core_module);

    h = ngx_array_push(&cmcf->phases[NGX_HTTP_ACCESS_PHASE].handlers);
    if (h == NULL) {
        return NGX_ERROR;
    }

    *h = ngx_http_donkey_run_access_handler;

    h = ngx_array_push(&cmcf->phases[NGX_HTTP_ACCESS_PHASE].handlers);
    if (h == NULL) {
        return NGX_ERROR;
    }

    *h = ngx_http_donkey_run_access_handler;

    ngx_http_next_body_filter = ngx_http_top_body_filter;
    ngx_http_top_body_filter = ngx_http_donkey_run_body_filter;

    ngx_http_next_header_filter = ngx_http_top_header_filter;
    ngx_http_top_header_filter = ngx_http_donkey_run_header_filter;

    return NGX_OK;
}

static ngx_int_t
ngx_http_uri_equal(ngx_http_request_t *r, ngx_str_t *str)
{
    ngx_str_t                       *uri = &r->uri;
    if (uri && uri->len == str->len && 0 == ngx_strncasecmp(uri->data, str->data, uri->len)) {
        return 1;
    }
    return 0;
}

static ngx_int_t
ngx_http_donkey_run_access_handler(ngx_http_request_t *r)
{
    ngx_str_t                       imp = ngx_string("import"),
                                    revw = ngx_string("review");
    ngx_http_donkey_run_ctx_t       *ctx;
    ngx_http_donkey_run_loc_conf_t  *drcf;

    drcf = ngx_http_get_module_loc_conf(r, ngx_http_donkey_run_module);
    if (!drcf->enable) {
        return NGX_DECLINED;
    }

    ctx = ngx_pcalloc(r->pool, sizeof(ngx_http_donkey_run_ctx_t));
    if (ctx == NULL) {
       return NGX_ERROR;
    }
    if (ngx_http_uri_equal(r, &imp)) {
        ctx->type = QUERY_TYPE_IMPORT;
    } else if (ngx_http_uri_equal(r, &revw)) {
        ctx->type = QUERY_TYPE_IMPORT;
    }

    ctx->enable = 1;

    return NGX_DECLINED;
}
