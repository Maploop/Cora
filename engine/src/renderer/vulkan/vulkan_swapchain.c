#include "vulkan_swapchain.h"

#include "core/logger.h"
#include "core/cmemory.h"
#include "vulkan_device.h"

void create(vulkan_context* context, u32 width, u32 height, vulkan_swapchain* swapchain);
void destroy(vulkan_context* context, vulkan_swapchain* swapchain);

void vulkan_swapchain_create(vulkan_context* context, u32 width, u32 height, vulkan_swapchain* out_swapchain) {
    create(context, width, height, out_swapchain);
}

void vulkan_swapchain_recreate(vulkan_context* context, u32 width, u32 height, vulkan_swapchain* out_swapchain) {
    destroy(context, out_swapchain);
    create(context, width, height, out_swapchain);
}

void vulkan_swapchain_destroy(vulkan_context* context, vulkan_swapchain* swapchain) {
    destroy(context, swapchain);
}