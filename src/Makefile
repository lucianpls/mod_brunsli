APXS = apxs
LIBS += -lbrunslidec-c

default: mod_brunsli.so

mod_brunsli.so: mod_brunsli.c
    $(APXS) -c $(INCLUDES) $(LIBS) $^

clean:
    $(RM) -r mod_brunsli.lo mod_brunsli.la mod_brunsli.slo .libs
