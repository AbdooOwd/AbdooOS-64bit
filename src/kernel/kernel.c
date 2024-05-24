#include "kernel.h"
#include "../drivers/screen.h"

void kernel_main() {
    print_string_at("Hello, World!", 0, 0, 1, 0xffffff);
}