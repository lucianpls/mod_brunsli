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
#include <http_log.h>
#include <http_protocol.h>

#include <brunsli/decode.h>

// Maximum size of brunsli input, output jpeg will be larger
#define MAX_SZ 1024*1024

extern module AP_MODULE_DECLARE_DATA brunsli_module;

#if defined(APLOG_USE_MODULE)
APLOG_USE_MODULE(brunsli);
#endif

static apr_status_t compress_filter(ap_filter_t* f, apr_bucket_brigade* bb)
{
    return APR_SUCCESS;
}

typedef struct {
    apr_bucket_brigade* outbb;
    ap_filter_t* f;
} debrun_ctx;


// Get a chunk of data from DEBRUN, place a copy in the output brigade
static size_t out_fun(debrun_ctx* ctx, const uint8_t* data, size_t size) {
    ap_fwrite(ctx->f->next, ctx->outbb, data, size);
    return size;
}

static apr_status_t debrun_filter(ap_filter_t* f, apr_bucket_brigade* bb)
{
    // Read some data, look for brunsli signature
    char* buff;
    apr_size_t bytes;
    apr_bucket* first = APR_BRIGADE_FIRST(bb);
    if (!first) return APR_SUCCESS; // empty brigade
    int state = apr_bucket_read(first, &buff, &bytes, APR_BLOCK_READ);
    static const char SIG[] = { 0x0a, 0x04, 0x42, 0xd2, 0xd5, 0x4e };
    if (APR_SUCCESS != state || bytes < 6 || memcmp(buff, SIG, sizeof(SIG))) {
        ap_remove_output_filter(f);
        return ap_pass_brigade(f->next, bb);
    }

    // Looks like brunsli content, is is small enough
    apr_off_t len;
    //  Returns -1 if it fails
    state = apr_brigade_length(bb, 1, &len);
    if (APR_SUCCESS != state || len > MAX_SZ || len < 0) {
        ap_log_rerror(APLOG_MARK, APLOG_WARNING, 0, f->r, "Brunsli input too large");
        ap_remove_output_filter(f);
        return ap_pass_brigade(f->next, bb);
    }

    apr_brigade_pflatten(bb, &buff, &bytes, f->r->pool);
    apr_brigade_cleanup(bb);
    debrun_ctx* ctx = apr_pcalloc(f->r->pool, sizeof(debrun_ctx));
    f->ctx = ctx;
    ctx->outbb = apr_brigade_create(f->r->pool, f->c->bucket_alloc);
    ctx->f = f;
    apr_table_unset(f->r->headers_out, "Content-Length");
    DecodeBrunsli(len, buff, ctx, out_fun);
    APR_BRIGADE_INSERT_TAIL(ctx->outbb, apr_bucket_eos_create(f->c->bucket_alloc));
    return ap_pass_brigade(f->next, ctx->outbb);
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