#include "vulkan_image.h"

#include "vulkan_device.h"

#include "core/cmemory.h"
#include "core/logger.h"

void vulkan_image_create(
    vulkan_context* context,
    VkImageType image_type,
    u32 width, u32 height,
    VkFormat format, VkImageTiling tiling,
    VkImageUsageFlags usage,
    VkMemoryPropertyFlags memory_flags,
    b32 create_view, VkImageAspectFlags aspect_flags,
    vulkan_image* out_image) {
        out_image->width = width;
        out_image->height = height;

        // creation info
        VkImageCreateInfo imageCreateInfo = {VK_STRUCTURE_TYPE_IMAGE_CREATE_INFO};
        imageCreateInfo.imageType = VK_IMAGE_TYPE_2D; // NOTE: This is hardcoded for now...
        imageCreateInfo.extent.width = width;
        imageCreateInfo.extent.height = height;
        imageCreateInfo.extent.depth = 1;   // TODO: make this configurable
        imageCreateInfo.mipLevels = 4;      // TODO: make this configurable
        imageCreateInfo.arrayLayers = 1;    // TODO: make this configurable
        imageCreateInfo.format = format;
        imageCreateInfo.tiling = tiling;
        imageCreateInfo.initialLayout = VK_IMAGE_LAYOUT_UNDEFINED;
        imageCreateInfo.usage = usage;
        imageCreateInfo.samples = VK_SAMPLE_COUNT_1_BIT;            // TODO: make this configurable
        imageCreateInfo.sharingMode = VK_SHARING_MODE_EXCLUSIVE;    // TODO: make this configurable

        VK_CHECK(vkCreateImage(context->device.logical_device, &imageCreateInfo, context->allocator, &out_image->handle));

        VkMemoryRequirements memory_requirements;
        vkGetImageMemoryRequirements(context->device.logical_device, out_image->handle, &memory_requirements);
        i32 memory_type = context->find_memory_index(memory_requirements.memoryTypeBits, memory_flags);
        if (memory_type == -1) {
            CERROR("Requiured memory type not found! Image not valid.");
        }

        VkMemoryAllocateInfo memory_allocate_info = {VK_STRUCTURE_TYPE_MEMORY_ALLOCATE_INFO};
        memory_allocate_info.allocationSize = memory_requirements.size;
        memory_allocate_info.memoryTypeIndex = memory_type;

        VK_CHECK(vkAllocateMemory(context->device.logical_device, &memory_allocate_info, context->allocator, &out_image->memory));
        VK_CHECK(vkBindImageMemory(context->device.logical_device, out_image->handle, out_image->memory, 0)); // TODO: make this configurable

        if (create_view) {
            out_image->view = 0;
            vulkan_image_view_create(context, format, out_image, aspect_flags);
        }
}

void vulkan_image_view_create(vulkan_context* context, VkFormat format, vulkan_image* image, VkImageAspectFlags aspect_flags) {
    VkImageViewCreateInfo view_create_info = {VK_STRUCTURE_TYPE_IMAGE_VIEW_CREATE_INFO};
    view_create_info.image = image->handle;
    view_create_info.viewType = VK_IMAGE_VIEW_TYPE_2D; // TODO: make configurable
    view_create_info.format = format;
    view_create_info.subresourceRange.aspectMask = aspect_flags;

    // TODO: make all of this configurable
    view_create_info.subresourceRange.baseMipLevel = 0;
    view_create_info.subresourceRange.levelCount = 1;
    view_create_info.subresourceRange.baseArrayLayer = 0;
    view_create_info.subresourceRange.layerCount = 1;

    VK_CHECK(vkCreateImageView(context->device.logical_device, &view_create_info, context->allocator, &image->view));
}

void vulkan_image_destroy(vulkan_context* context, vulkan_image* image) {
    if (image->view) {
        vkDestroyImageView(context->device.logical_device, image->view, context->allocator);
        image->view = 0;
    }
    
    if (image->memory) {
        vkFreeMemory(context->device.logical_device, image->memory, context->allocator);
        image->memory = 0;
    }
    
    if (image->handle) {
        vkDestroyImage(context->device.logical_device, image->handle, context->allocator);
        image->handle = 0;
    }
}