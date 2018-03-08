#include "EntityRender.hpp"

#include "../Devices/Display.hpp"
#include "../Meshes/Mesh.hpp"
#include "../Materials/Material.hpp"
#include "../Physics/Rigidbody.hpp"
#include "../Scenes/Scenes.hpp"
#include "UbosEntities.hpp"

namespace Flounder
{
	EntityRender::EntityRender() :
		Component(),
		m_uniformObject(new UniformBuffer(sizeof(UbosEntities::UboObject))),
		m_descriptorSet(nullptr)
	{
	}

	EntityRender::~EntityRender()
	{
		delete m_uniformObject;
		delete m_descriptorSet;
	}

	void EntityRender::Update()
	{
	}

	void EntityRender::Load(LoadedValue *value)
	{
	}

	void EntityRender::Write(LoadedValue *value)
	{
	}

	void EntityRender::CmdRender(const VkCommandBuffer &commandBuffer, const Pipeline &pipeline, const UniformBuffer &uniformScene)
	{
		// Gets required components.
		auto mesh = GetGameObject()->GetComponent<Mesh>();
		auto material = GetGameObject()->GetComponent<Material>();
		auto rigidbody = GetGameObject()->GetComponent<Rigidbody>();

		if (mesh == nullptr || mesh->GetModel() == nullptr || material == nullptr)
		{
			return;
		}

		if (m_descriptorSet == nullptr)
		{
			m_descriptorSet = new DescriptorSet(pipeline);
			std::vector<VkWriteDescriptorSet> descriptorWrites = std::vector<VkWriteDescriptorSet>();

			descriptorWrites.push_back(uniformScene.GetWriteDescriptor(0, *m_descriptorSet));
			descriptorWrites.push_back(m_uniformObject->GetWriteDescriptor(1, *m_descriptorSet));

			if (material->GetTextureDiffuse() != nullptr)
			{
				descriptorWrites.push_back(material->GetTextureDiffuse()->GetWriteDescriptor(2, *m_descriptorSet));
			}

			if (material->GetTextureMaterial() != nullptr)
			{
				descriptorWrites.push_back(material->GetTextureMaterial()->GetWriteDescriptor(3, *m_descriptorSet));
			}

			if (material->GetTextureNormal() != nullptr)
			{
				descriptorWrites.push_back(material->GetTextureNormal()->GetWriteDescriptor(4, *m_descriptorSet));
			}
			m_descriptorSet->Update(descriptorWrites);
		}

		/*if (rigidbody != nullptr && rigidbody->GetCollider() != nullptr)
		{
			if (!rigidbody->GetCollider()->InFrustum(*Scenes::Get()->GetCamera()->GetViewFrustum()))
			{
				return;
			}
		}*/

		// Creates a UBO object and write descriptor.
		UbosEntities::UboObject uboObject = {};
		GetGameObject()->GetTransform()->GetWorldMatrix(&uboObject.transform);

		if (material->GetTextureDiffuse() != nullptr)
		{
			uboObject.samples.m_x = 1.0f;
		}

		if (material->GetTextureMaterial() != nullptr)
		{
			uboObject.samples.m_y = 1.0f;
		}

		if (material->GetTextureNormal() != nullptr)
		{
			uboObject.samples.m_z = 1.0f;
		}

		uboObject.baseColor = *material->GetBaseColor();
		uboObject.surface = Vector4(material->GetMetallic(), material->GetRoughness(),
			static_cast<float>(material->GetIgnoreFog()), static_cast<float>(material->GetIgnoreLighting()));

		m_uniformObject->Update(&uboObject);

		// Draws the object.
		m_descriptorSet->BindDescriptor(commandBuffer, pipeline);
		mesh->GetModel()->CmdRender(commandBuffer);
	}
}
