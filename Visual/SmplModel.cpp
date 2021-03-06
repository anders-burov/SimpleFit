#include "SmplModel.h"
#include "Utils.h"

void SmplModel::Initialize(ID3D11Device* device, ID3D11DeviceContext* device_context)
{
	device_context_ = device_context;

	const D3D11_INPUT_ELEMENT_DESC vertex_layout_desc[] =
	{
		{ "POSITION", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0,  0, D3D11_INPUT_PER_VERTEX_DATA, 0 }
	};

	{
		std::vector<byte> vertex_shader = readShaderFromCSO("CubeVS.cso");
		VALIDATE(device->CreateVertexShader(vertex_shader.data(), vertex_shader.size(),
			nullptr, &vertex_shader_), L"Could not create VertexShader");

		VALIDATE(device->CreateInputLayout(vertex_layout_desc, ARRAYSIZE(vertex_layout_desc),
			vertex_shader.data(), vertex_shader.size(), &input_layout_), L"Could not create InputLayout");

		std::vector<byte> geometry_shader = readShaderFromCSO("CubeGS.cso");
		VALIDATE(device->CreateGeometryShader(geometry_shader.data(), geometry_shader.size(),
			nullptr, &geometry_shader_), L"Could not create GeometryShader");

		std::vector<byte> pixel_shader = readShaderFromCSO("CubePS.cso");
		VALIDATE(device->CreatePixelShader(pixel_shader.data(), pixel_shader.size(),
			nullptr, &pixel_shader_), L"Could not create PixelShader");
	}

	// Generate Template SMPL Body
	Generate();
	{
		D3D11_BUFFER_DESC vertex_buffer_desc = { 0 };
		vertex_buffer_desc.ByteWidth = sizeof(smpl::float3) * (unsigned int)smpl::VERTEX_COUNT;
		vertex_buffer_desc.Usage = D3D11_USAGE_DYNAMIC;
		vertex_buffer_desc.BindFlags = D3D11_BIND_VERTEX_BUFFER;
		vertex_buffer_desc.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;
		vertex_buffer_desc.MiscFlags = 0;
		vertex_buffer_desc.StructureByteStride = 0;

		D3D11_SUBRESOURCE_DATA vertex_buffer_data;
		vertex_buffer_data.pSysMem = body_.vertices.data();
		vertex_buffer_data.SysMemPitch = 0;
		vertex_buffer_data.SysMemSlicePitch = 0;

		VALIDATE(device->CreateBuffer(&vertex_buffer_desc, &vertex_buffer_data,
			&vertex_buffer_), L"Could not create VertexBuffer");
	}

	indices_count_ = (unsigned int)body_.indices.size();

	{
		D3D11_BUFFER_DESC index_buffer_desc;
		index_buffer_desc.ByteWidth = sizeof(smpl::uint) * (unsigned int)indices_count_;
		index_buffer_desc.Usage = D3D11_USAGE_DEFAULT;
		index_buffer_desc.BindFlags = D3D11_BIND_INDEX_BUFFER;
		index_buffer_desc.CPUAccessFlags = 0;
		index_buffer_desc.MiscFlags = 0;
		index_buffer_desc.StructureByteStride = 0;

		D3D11_SUBRESOURCE_DATA index_buffer_data;
		index_buffer_data.pSysMem = body_.indices.data();
		index_buffer_data.SysMemPitch = 0;
		index_buffer_data.SysMemSlicePitch = 0;

		VALIDATE(device->CreateBuffer(&index_buffer_desc, &index_buffer_data, &index_buffer_),
			L"Could not create IndexBuffer");
	}

	// Create ConstantBuffer for camera matrices
	{
		D3D11_BUFFER_DESC constantBufferDesc = { 0 };
		constantBufferDesc.ByteWidth = camera_.GetDataSize();
		constantBufferDesc.Usage = D3D11_USAGE_DEFAULT;
		constantBufferDesc.BindFlags = D3D11_BIND_CONSTANT_BUFFER;
		constantBufferDesc.CPUAccessFlags = 0;
		constantBufferDesc.MiscFlags = 0;
		constantBufferDesc.StructureByteStride = 0;

		VALIDATE(device->CreateBuffer(&constantBufferDesc, nullptr, &camera_constant_buffer_),
			L"Could not create CameraConstantBuffer");
	}

	// Create a constant buffer for joint colors
	{
		for (int i = 0; i < smpl::JOINT_COUNT; i++)
		{
			// encodes 27 states (we have 24)
			int x = i / 9;
			int y = (i / 3) % 3;
			int z = i % 3;
			// 0->0.0, 1->0.5, 2->1.0
			auto v = Eigen::Vector4f(
				static_cast<float>(x) / 2.f,
				static_cast<float>(y) / 2.f,
				static_cast<float>(z) / 2.f, 1.f);
			joint_colors_.push_back(v);
		}

		D3D11_BUFFER_DESC cb_joint_color_desc = { 0 };
		cb_joint_color_desc.ByteWidth = joint_colors_.size() * sizeof(Eigen::Vector4f);
		cb_joint_color_desc.Usage = D3D11_USAGE_DEFAULT;
		cb_joint_color_desc.BindFlags = D3D11_BIND_CONSTANT_BUFFER;
		cb_joint_color_desc.CPUAccessFlags = 0;
		cb_joint_color_desc.MiscFlags = 0;
		cb_joint_color_desc.StructureByteStride = 0;

		D3D11_SUBRESOURCE_DATA cb_joint_color_data;
		cb_joint_color_data.pSysMem = joint_colors_.data();
		cb_joint_color_data.SysMemPitch = 0;
		cb_joint_color_data.SysMemSlicePitch = 0;

		VALIDATE(device->CreateBuffer(&cb_joint_color_desc, &cb_joint_color_data, 
			&joint_color_constant_buffer_),
			L"Could not create JointColorConstantBuffer");
	}

	// Create SRV for the Skins
	{
		D3D11_BUFFER_DESC skin_buffer_desc = { 0 };
		skin_buffer_desc.ByteWidth = sizeof(smpl::Skin) * (unsigned int)smpl::VERTEX_COUNT;
		skin_buffer_desc.Usage = D3D11_USAGE_IMMUTABLE;
		skin_buffer_desc.BindFlags = D3D11_BIND_SHADER_RESOURCE;
		skin_buffer_desc.CPUAccessFlags = 0;
		skin_buffer_desc.MiscFlags = D3D11_RESOURCE_MISC_BUFFER_STRUCTURED;
		skin_buffer_desc.StructureByteStride = sizeof(smpl::Skin);

		D3D11_SUBRESOURCE_DATA skin_buffer_data;
		skin_buffer_data.pSysMem = generator_.GetSkins().data();
		skin_buffer_data.SysMemPitch = 0;
		skin_buffer_data.SysMemSlicePitch = 0;

		ID3D11Buffer* skin_buffer = nullptr;
		VALIDATE(device->CreateBuffer(&skin_buffer_desc, &skin_buffer_data,
			&skin_buffer), L"Could not create skin buffer");

		D3D11_SHADER_RESOURCE_VIEW_DESC srv_desc;
		srv_desc.Format = DXGI_FORMAT_UNKNOWN;
		srv_desc.ViewDimension = D3D_SRV_DIMENSION_BUFFER;
		srv_desc.Buffer.FirstElement = 0;
		srv_desc.Buffer.NumElements = generator_.GetSkins().size();

		VALIDATE(device->CreateShaderResourceView(skin_buffer, &srv_desc, &skin_buffer_view_),
			L"Could not create skin srv");

		SAFE_RELEASE(skin_buffer);
	}
}

void SmplModel::Draw()
{
	device_context_->UpdateSubresource(camera_constant_buffer_, 0, nullptr, camera_.GetDataPointer(), 0, 0);
	device_context_->UpdateSubresource(joint_color_constant_buffer_, 0, nullptr, joint_colors_.data(), 0, 0);
	device_context_->IASetInputLayout(input_layout_);

	// Set the vertex and index buffers, and specify the way they define geometry
	UINT stride = sizeof(smpl::float3);
	UINT offset = 0;
	device_context_->IASetVertexBuffers(0, 1, &vertex_buffer_, &stride, &offset);
	device_context_->IASetIndexBuffer(index_buffer_, DXGI_FORMAT_R32_UINT, 0);
	device_context_->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);

	// Set the vertex and pixel shader stage state
	device_context_->VSSetShader(vertex_shader_, nullptr, 0);
	device_context_->VSSetShaderResources(0, 1, &skin_buffer_view_);
	device_context_->VSSetConstantBuffers(0, 1, &joint_color_constant_buffer_);
	device_context_->GSSetShader(geometry_shader_, nullptr, 0);
	device_context_->GSSetConstantBuffers(0, 1, &camera_constant_buffer_);
	device_context_->PSSetShader(pixel_shader_, nullptr, 0);

	device_context_->DrawIndexed(indices_count_, 0, 0);
}

void SmplModel::Clear()
{
	SAFE_RELEASE(vertex_shader_);
	SAFE_RELEASE(pixel_shader_);
	SAFE_RELEASE(geometry_shader_);
	SAFE_RELEASE(input_layout_);
	SAFE_RELEASE(vertex_buffer_);
	SAFE_RELEASE(index_buffer_);
	SAFE_RELEASE(camera_constant_buffer_);
	SAFE_RELEASE(joint_color_constant_buffer_);
	SAFE_RELEASE(skin_buffer_view_);
}

void SmplModel::Dump(const std::string& filename)
{
	body_.Dump(filename);
}

void SmplModel::Generate(smpl::ShapeCoefficients& shape, smpl::PoseAxisAngleCoefficients& pose)
{
	body_ = generator_(shape, pose);
	UpdateBodyOnGPU();
}

void SmplModel::Generate(smpl::ShapeCoefficients& shape, smpl::PoseEulerCoefficients& pose)
{
	body_ = generator_(shape, pose);
	UpdateBodyOnGPU();
}

void SmplModel::Generate()
{
	smpl::ShapeCoefficients shape;
	smpl::PoseAxisAngleCoefficients pose;
	
	body_ = generator_(shape, pose);
}

void SmplModel::UpdateBodyOnGPU()
{
	D3D11_MAPPED_SUBRESOURCE resource;
	device_context_->Map(vertex_buffer_, 0, D3D11_MAP_WRITE_DISCARD, 0, &resource);
	memcpy(resource.pData, body_.vertices.data(), sizeof(smpl::float3) * (unsigned int)smpl::VERTEX_COUNT);
	device_context_->Unmap(vertex_buffer_, 0);
}