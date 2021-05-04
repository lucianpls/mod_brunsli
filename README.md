# mod_brunsli

An Apache HTTPD brunsli to jpeg filter

[Brunsli][] is a fast lossless JPEG recompressor that is included in the
[committee draft of the JPEG XL standard][CD]. This Apache HTTPD module allows serving standard JPEGs from precompressed Brunsli files, saving about 20% of the JPEG storage space.

[Brunsli]: https://github.com/google/brunsli
[CD]: https://arxiv.org/abs/1908.03565

It is a standard apache HTTPD output filter module, which can be enabled using the [AddOutputFilter directive](https://httpd.apache.org/docs/2.4/mod/mod_mime.html#addoutputfilter), or using [mod_filter](https://httpd.apache.org/docs/2.4/mod/mod_filter.html)  
When enabled, mod_brunsli activates on detecting brunsli content and converts them to standard JPEG, on the fly. It currently has a hardcoded input (brunsli) size limit of 1MB, if the input is larger it will be forwarded as is, in the brunsli format. The input (brunsli) has to be present in memory when the conversion to JPEG is done, which means that a busy server using this module may use a significant amount of memory. The JPEG output is streamed and does not require a large buffer. CPU utilization and request latency increase are very low.
