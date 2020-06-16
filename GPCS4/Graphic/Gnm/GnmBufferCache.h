#pragma once

#include "GnmCommon.h"
#include "GnmResourceMemory.h"

#include <unordered_map>

namespace sce
{;
struct SceGpuQueueDevice;
}  // namespace sce

namespace vlt
{;
class VltBuffer;
class VltContext;
}  // namespace vlt

class GnmMemoryMonitor;
class GnmBuffer;
struct GnmIndexBuffer;


/**
 * \brief Buffer create information
 *
 * We need input usage type to create proper
 * vulkan buffers.
 */
struct GnmBufferCreateInfo
{
	union
	{
		const GnmBuffer*      buffer;
		const GnmIndexBuffer* index;
	};
	
	VkPipelineStageFlags  stages;
	VkBufferUsageFlags    usage;
};


struct GnmBufferInstance
{
	RcPtr<vlt::VltBuffer> buffer    = nullptr;
	uint32_t              idleCount = 0;
	GnmResourceMemory     memory;
};


class GnmBufferCache
{
public:
	GnmBufferCache(const sce::SceGpuQueueDevice* device,
				   vlt::VltContext*              context,
				   GnmMemoryMonitor*             monitor);
	~GnmBufferCache();

	GnmBufferInstance* grabBuffer(const GnmBufferCreateInfo& desc);

	void flush(const GnmMemoryRange& range);

	void invalidate(const GnmMemoryRange& range);

private:

	GnmMemoryRange extractMemoryRange(
		const GnmBufferCreateInfo& desc);

	GnmBufferInstance createBuffer(
		const GnmBufferCreateInfo& desc);


private:
	const sce::SceGpuQueueDevice* m_device;
	vlt::VltContext*              m_context;
	GnmMemoryMonitor*             m_monitor;

	std::unordered_map<
		GnmMemoryRange, GnmBufferInstance,
		GnmMemoryHash> m_bufferMap;
};
