#include "Model.hpp"

#include "Scenes/Scenes.hpp"
#include "Resources/Resources.hpp"

namespace acid
{
std::shared_ptr<Model> Model::Create(const Metadata &metadata)
{
	return Scenes::Get()->GetModelRegister().Create(metadata);
}

std::shared_ptr<Model> Model::Create(const std::string &filename)
{
	return Scenes::Get()->GetModelRegister().Create(filename);
}

Model::Model() :
	m_vertexBuffer(nullptr),
	m_indexBuffer(nullptr),
	m_vertexCount(0),
	m_indexCount(0),
	m_radius(0.0f)
{
}

bool Model::CmdRender(const CommandBuffer &commandBuffer, const uint32_t &instances) const
{
	if (m_vertexBuffer != nullptr && m_indexBuffer != nullptr)
	{
		VkBuffer vertexBuffers[] = { m_vertexBuffer->GetBuffer() };
		VkDeviceSize offsets[] = { 0 };
		vkCmdBindVertexBuffers(commandBuffer, 0, 1, vertexBuffers, offsets);
		vkCmdBindIndexBuffer(commandBuffer, m_indexBuffer->GetBuffer(), 0, GetIndexType());
		vkCmdDrawIndexed(commandBuffer, m_indexCount, instances, 0, 0, 0);
	}
	else if (m_vertexBuffer != nullptr && m_indexBuffer == nullptr)
	{
		VkBuffer vertexBuffers[] = { m_vertexBuffer->GetBuffer() };
		VkDeviceSize offsets[] = { 0 };
		vkCmdBindVertexBuffers(commandBuffer, 0, 1, vertexBuffers, offsets);
		vkCmdDraw(commandBuffer, m_vertexCount, instances, 0, 0);
	}
	else
	{
		//throw std::runtime_error("Model with no buffers cannot be rendered");
		return false;
	}

	return true;
}

void Model::Load()
{
}

void Model::Decode(const Metadata &metadata)
{
}

void Model::Encode(Metadata &metadata) const
{
}

std::vector<float> Model::GetPointCloud() const
{
	if (m_vertexBuffer == nullptr)
	{
		return std::vector<float>();
	}

	// TODO: Fix Vulkan memory mapping error.
	std::vector<float> pointCloud(m_vertexBuffer->GetSize() / sizeof(float));
	m_vertexBuffer->MapMemory(reinterpret_cast<void **>(pointCloud.data()));
	m_vertexBuffer->UnmapMemory();
	return pointCloud;
}
}
