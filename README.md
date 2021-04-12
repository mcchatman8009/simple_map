# Simple Map

## Overview 
A simple header only Hash Map library to enable for fast development and abstraction.


### C Example

```C
#include "simple_map.h"
#include <stdio.h>

int main() {
    smap* m = sm_new();

    sm_put_str(m, "Key 1", "Value");
    sm_put_str(m, "Key 2", "Value");
    sm_put_str(m, "Key 3", "Value");
    sm_put_str(m, "Key 4", "Value");
    sm_put_str(m, "Key 5", "Value");
    sm_put_str(m, "Key 6", "Value");
    sm_put_str(m, "Key 7", "Value");
    sm_put_str(m, "Key 8", "Value");
    sm_put_str(m, "Key 9", "Value");
    sm_put_str(m, "Key 10", "Value");
    sm_put_str(m, "Key 11", "Value");
    sm_put_str(m, "Key 12", "Value");

    const char* val = sm_get(m, "Key 1");
    printf("Key 1 -> %s\n", val);

    printf("map_size: %d", sm_size(m));

    return 0;
}
```
