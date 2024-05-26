# Notes

-   To get the framebuffer, we request one. And to do so:
    ```c
    struct limine_framebuffer *framebuffer = framebuffer_request.response->framebuffers[0];
    ```
-   I'm too dumb to remember, but I'm working with the x86-64 architecture.
-   Limine's default resolution is 1280x800.
-   I want something like [this](https://github.com/Rostamborn/MmdOS/tree/master).
-   We're in **Long Mode**.