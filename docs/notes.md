# Notes

-   To get the framebuffer, we request one. And to do so:
    ```c
    struct limine_framebuffer *framebuffer = framebuffer_request.response->framebuffers[0];
    ```
-   I'm too dumb to remember, but I'm working with the x86-64 architecture.