# mod_brunsli

A brunsli to JPEG convertor output filter

[Brunsli][] is a fast lossless JPEG recompressor that is included in the
[committee draft of the JPEG XL standard][CD]. This Apache HTTPD module allows serving legacy JPEGs from recompressed Brunsli files, saving about 20% of the JPEG storage space.

[Brunsli]: https://github.com/google/brunsli
[CD]: https://arxiv.org/abs/1908.03565
As an Apache HTTPD output filter module, it can be enabled using the [AddOutputFilter directive](https://httpd.apache.org/docs/2.4/mod/mod_mime.html#addoutputfilter), or using [mod_filter](https://httpd.apache.org/docs/2.4/mod/mod_filter.html)  
When enabled, mod_brunsli activates when it detects brunsli content and converts it back to the original JPEG and sends that JPEG to the user. Currently it has a hardcoded input (brunsli) size limit of 1MB, if the input is larger it will be forwarded as is, in the brunsli format. The input (brunsli) has to be present in memory when the conversion to JPEG is done, which means that a busy server using this module may use an increased amount of memory. The JPEG output is streamed and does not require a large buffer. CPU utilization and request latency increase are very low.
