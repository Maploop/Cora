#include "vulkan_framebuffer.h"
#include "core/cmemory.h"

void vulkan_framebuffer_create(vulkan_context* context, vulkan_renderpass* renderpass, u32 width, u32 height, u32 attachment_count,
    VkImageView* attachments, vulkan_framebuffer* out_framebuffer)
{
    out_framebuffer->attachments = callocate(sizeof(VkImageView) * attachment_count, MEMORY_TAG_RENDERER);
    for (u32 i = 0; i < attachment_count; i++) {
        out_framebuffer->attachments[i] = attachments[i];
    }

    out_framebuffer->renderpass = renderpass;
    out_framebuffer->attachment_count = attachment_count;

    // create info
    VkFramebufferCreateInfo fb_create_info = {VK_STRUCTURE_TYPE_FRAMEBUFFER_CREATE_INFO};
    fb_create_info.renderPass = out_framebuffer->renderpass->handle;
    fb_create_info.attachmentCount = attachment_count;
    fb_create_info.pAttachments = out_framebuffer->attachments;
    fb_create_info.width = width;
    fb_create_info.height = height;
    fb_create_info.layers = 1;

    VK_CHECK(vkCreateFramebuffer(context->device.logical_device,
        &fb_create_info, context->allocator, &out_framebuffer->handle));
}

void vulkan_framebuffer_destroy(vulkan_context* context, vulkan_framebuffer* framebuffer)
{
    vkDestroyFramebuffer(context->device.logical_device, framebuffer->handle, context->allocator);
    if (framebuffer->attachments) {
        cfree(framebuffer->attachments, sizeof(VkImageView) * framebuffer->attachment_count, MEMORY_TAG_RENDERER);
        framebuffer->attachments = 0;
    }
    framebuffer->handle = 0;
    framebuffer->attachment_count = 0;
    framebuffer->renderpass = 0;
}