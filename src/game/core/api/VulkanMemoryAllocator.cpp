#pragma msvc diagnostic push
#pragma clang diagnostic warning "-Weverything"
#pragma clang diagnostic warning "-Wall"
#pragma clang diagnostic warning "-Wextra"

#define VMA_STATIC_VULKAN_FUNCTIONS 0
#define VMA_IMPLEMENTATION
#include <vk_mem_alloc.h>

#pragma clang diagnostic pop