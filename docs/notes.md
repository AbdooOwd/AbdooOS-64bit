# Notes

-   To get the framebuffer, we request one. And to do so:
    ```c
    struct limine_framebuffer *framebuffer = framebuffer_request.response->framebuffers[0];
    ```
-   I'm too dumb to remember, but I'm working with the x86-64 architecture.
-   Limine's default resolution is 1280x800.
-   I want something like [this](https://github.com/Rostamborn/MmdOS/tree/master).
-   We're in **Long Mode**.
-   Run `addr2line -e mykernel.elf 0xADDRESSOFFAULT` to get assembly of address.

-   Interrupts between 0 and 32 are exceptions, after it are ISQs.

-   Requests for Limine: 
    ```c
    __attribute__((used, section(".requests")))
    static struct limine_xxxxx_request xxxxx_request = {
        .id = LIMINE_XXXXXX_REQUEST,
        .revision = 0
    };
    ```

-   When trying to get the size of a data type by doing `sizeof(datatype)`,
    even if your variable is a pointer *(e.g `datatype* variable = ...;`)*,
    you **MUST NOT** include the star at the end. So to get the size of the datatype
    when we're calculating somehting for `datatype* variable;`, we take the **type's size**.
    Not its **pointer size**.
    