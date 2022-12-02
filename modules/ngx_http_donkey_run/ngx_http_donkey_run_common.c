#include "ngx_http_donkey_run_common.h"

ngx_int_t
ngx_http_donkey_get_header(ngx_http_request_t *r, ngx_str_t *key, ngx_str_t *val)
{
    ngx_uint_t                    i;
    ngx_list_part_t              *part;
    ngx_table_elt_t              *h;

    if (!key) {
        return NGX_ERROR;
    }

    part = &r->headers_in.headers.part;
    h = part->elts;
    for (i = 0; /* void */; i++) { 
        if (i >= part->nelts) { 
            if (part->next == NULL) {
                break;
            }
            part = part->next;
            h = part->elts;
        }

        if (h[i].key.len == key->len && ngx_strncasecmp(h[i].key.data, key->data, h[i].key.len)) {
            val->data = h[i].value.data;
            val->len = h[i].value.len;
            return NGX_OK;
        }
    }

    return NGX_DECLINED;
}


