#pragma GCC diagnostic push
#pragma GCC diagnostic ignored "-Wpadded"
#include <external/limine.h>
#pragma GCC diagnostic pop

#include <kernel/tty/fbCon.h>
volatile struct limine_framebuffer_request fbRequest = {
	.id = LIMINE_FRAMEBUFFER_REQUEST,
	.revision = 0
};

static framebuffer_t fbInfoLocal;

framebuffer_t LM_GetFramebuffer() {
	struct limine_framebuffer *res = fbRequest.response->framebuffers[0];
	fbInfoLocal.ptr    = (uint32_t *)res->address;
	fbInfoLocal.width  = (uint32_t)res->width;
	fbInfoLocal.height = (uint32_t)res->height;

	fbInfoLocal.mode = FB_MODE_RGBA;

	return fbInfoLocal;
}