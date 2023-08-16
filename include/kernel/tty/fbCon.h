typedef struct {
	uint32_t width;
	uint32_t height;

	enum {
		FB_MODE_ARGB,
		FB_MODE_RGBA,
		FB_MODE_BGRA
	} mode;

	uint32_t padding1;
	uint32_t *ptr;
} framebuffer_t;

extern uint8_t *font[];


extern framebuffer_t fbCon;
extern framebuffer_t FBCON_Init();