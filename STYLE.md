# coding style
so for types, use **PascalCase**
and for vars/functions use **snake_case**
and as for comments just use /* */ because // looks terrible
and for tabs, just use 4 spaces like a normie
keep around 1 line between each function (elaborate?)
and macros (preprocessor?) are SNAKE_CASE_BUT_ALL_CAPS
if you have questions, just open a new discussion

example:
```c
#include <cool/cool.h>

#ifdef COOL_THING
#define COOL_THING
#endif

static CustomTypeNumber1 function_name_here(AnotherCustomType char_array_perhaps) {
    CustomTypeNumber1 cool = do_something();
    /* so something like this looks
     * awesome!!!
     */
    return cool;
}
```
