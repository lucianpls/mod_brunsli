/*
 * mod_brunsli
 * An apache httpd brunsli-jpeg convertor filter
 * 
 */

#define APR_WANT_STRFUNC
#define APR_WANT_MEMFUNC
#include <apr_want.h>

#include <httpd.h>
#include <http_core.h>

#include <brunsli/decode.h>

extern module AP_MODULE_DECLARE_DATA brunsli_module;

#if defined(APLOG_USE_MODULE)
APLOG_USE_MODULE(brunsli);
#endif

static apr_status_t compress_filter(ap_filter_t* f, apr_bucket_brigade* bb)
{
    return APR_SUCCESS;
}

static apr_status_t debrun_filter(ap_filter_t* f, apr_bucket_brigade* bb)
{
    return APR_SUCCESS;
}

static const command_rec cmds[] = {
    {NULL}
};

static const char BDName[] = "BRUNSLI_DECOMPRESS";

#define DEC_PROTO_FLAGS  AP_FILTER_PROTO_CHANGE | AP_FILTER_PROTO_CHANGE_LENGTH | AP_FILTER_PROTO_NO_BYTERANGE

static void register_hooks(apr_pool_t *p) {
//	ap_register_output_filter("BRUNSLI_COMPRESS", compress_filter, NULL, AP_FTYPE_CONTENT_SET);

    ap_register_output_filter_protocol(BDName, debrun_filter, NULL, AP_FTYPE_CONTENT_SET, DEC_PROTO_FLAGS);
}

module AP_MODULE_DECLARE_DATA brunsli_module = {
    STANDARD20_MODULE_STUFF,
    0,
    0,
    0,
    0,
    cmds,
    register_hooks
};