#pragma once

#define CONFIG_UART 1
#define CONFIG_PIT_TARGET_SPEED 1000 // 1000 hz target
#define CONFIG_MAX_PAGES (16384) // equation: target memory / 4KB per page / 8. in this case, 64MB
