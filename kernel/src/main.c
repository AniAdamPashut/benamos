#include <stdint.h>
#include <stddef.h>
#include <stdbool.h>
#include <limine.h>

#include "init.h"
#include "printf.h"
#include "utilities.h"

// Set the base revision to 3, this is recommended as this is the latest
// base revision described by the Limine boot protocol specification.
// See specification for further info.

__attribute__((used, section(".limine_requests")))
static volatile LIMINE_BASE_REVISION(3);

// The Limine requests can be placed anywhere, but it is important that
// the compiler does not optimise them away, so, usually, they should
// be made volatile or equivalent, _and_ they should be accessed at least
// once or marked as used with the "used" attribute as done here.

__attribute__((used, section(".limine_requests")))
static volatile struct limine_framebuffer_request framebuffer_request = {
    .id = LIMINE_FRAMEBUFFER_REQUEST,
    .revision = 0
};

// Finally, define the start and end markers for the Limine requests.
// These can also be moved anywhere, to any .c file, as seen fit.

__attribute__((used, section(".limine_requests_start")))
static volatile LIMINE_REQUESTS_START_MARKER;

__attribute__((used, section(".limine_requests_end")))
static volatile LIMINE_REQUESTS_END_MARKER;


// ========== Draw Pixel ==========
void put_pixel(uint32_t *fb, uint64_t pitch, int x, int y, uint32_t color, int width, int height) {
    if (x >= 0 && x < width && y >= 0 && y < height)
        fb[y * pitch + x] = color;
}

// ========== Draw Circle Outline ==========
void draw_circle_outline(uint32_t *fb, int pitch, int cx, int cy, int r, uint32_t color, int width, int height) {
    int x = 0;
    int y = r;
    int d = 3 - 2 * r;

    while (y >= x) {
        // 8 symmetry points
        put_pixel(fb, pitch, cx + x, cy + y, color, width, height);
        put_pixel(fb, pitch, cx - x, cy + y, color, width, height);
        put_pixel(fb, pitch, cx + x, cy - y, color, width, height);
        put_pixel(fb, pitch, cx - x, cy - y, color, width, height);
        put_pixel(fb, pitch, cx + y, cy + x, color, width, height);
        put_pixel(fb, pitch, cx - y, cy + x, color, width, height);
        put_pixel(fb, pitch, cx + y, cy - x, color, width, height);
        put_pixel(fb, pitch, cx - y, cy - x, color, width, height);

        x++;
        if (d < 0) {
            d = d + 4 * x + 6;
        } else {
            y--;
            d = d + 4 * (x - y) + 10;
        }
    }
}

// Draw vertical cylinder (outlined) with rounded top
void draw_cylinder_outline(uint32_t *fb, int pitch, int cx, int base_y, int height, int radius, uint32_t color, int screen_w, int screen_h) {
    int x_left = cx - radius;
    int x_right = cx + radius;
    int top_y = base_y - height;

    // Draw vertical sides
    for (int y = top_y + radius; y < base_y; y++) {
        put_pixel(fb, pitch, x_left, y, color, screen_w, screen_h);
        put_pixel(fb, pitch, x_right, y, color, screen_w, screen_h);
    }

    // Draw top cap (half-circle, top-facing)
    int x = 0;
    int y = radius;
    int d = 3 - 2 * radius;
    while (y >= x) {
        // Only draw the top half
        put_pixel(fb, pitch, cx + x, top_y + radius - y, color, screen_w, screen_h);
        put_pixel(fb, pitch, cx - x, top_y + radius - y, color, screen_w, screen_h);
        put_pixel(fb, pitch, cx + y, top_y + radius - x, color, screen_w, screen_h);
        put_pixel(fb, pitch, cx - y, top_y + radius - x, color, screen_w, screen_h);
        x++;
        if (d < 0)
            d += 4 * x + 6;
        else {
            y--;
            d += 4 * (x - y) + 10;
        }
    }
}


// The following will be our kernel's entry point.
// If renaming kmain() to something else, make sure to change the
// linker script accordingly.
void kmain(void) {
    // Ensure the bootloader actually understands our base revision (see spec).
    if (LIMINE_BASE_REVISION_SUPPORTED == false) {
        hcf();
    }

    init();

    // Ensure we got a framebuffer.
    if (framebuffer_request.response == NULL
     || framebuffer_request.response->framebuffer_count < 1) {
        hcf();
    }

    // Fetch the first framebuffer.
    struct limine_framebuffer *fb = framebuffer_request.response->framebuffers[0];

    uint32_t *pixels = (uint32_t *)fb->address;
    int width = fb->width;
    int height = fb->height;
    int pitch = fb->pitch / 4; // pixels per line

    int r = 100;
    int cx = width / 2;
    int cy = height / 2;

    // Circle 1: center at (cx + r, cy)
    draw_circle_outline(pixels, pitch, cx + r, cy, r, 0xFFFFFF, width, height); // red
    // Circle 2: center at (cx - r, cy)
    draw_circle_outline(pixels, pitch, cx - r, cy, r, 0xFFFFFF, width, height); // blue

    int cylinder_height = cy - r;
    int cylinder_radius = r;
    draw_cylinder_outline(pixels, pitch, cx, cy - r, cylinder_height, cylinder_radius, 0xFFFFFF, width, height); // Green

    const char *msg = "Hello, World!\n";
    printf("%s", msg);

    // We're done, just hang...
    hcf();
}
