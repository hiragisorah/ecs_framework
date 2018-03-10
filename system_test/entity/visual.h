#pragma once

// フレームワーク
#include <game\entity.h>

// 標準
#include <vector>

// API
#include <D3DX11.h>
#include <D3DX10.h>

struct ConstantBuffer
{
	ID3D11Buffer * buffer_;
	void * pointer_ = nullptr;
	unsigned int size_ = 0;
};

struct VertexAndIndexBuffer
{
	DWORD vertices_num_ = 0;
	DWORD faces_num_ = 0;
	ID3D11Buffer * vertex_buffer_ = nullptr;
	D3D11_PRIMITIVE_TOPOLOGY topology_ = D3D11_PRIMITIVE_TOPOLOGY_POINTLIST;
	unsigned int vertex_size_ = 0;
	unsigned int index_size_ = 0;
	unsigned int vertices_per_face_ = 1;
};

struct Shader
{
	ID3D11InputLayout * vertex_layout_ = nullptr;
	ID3D11VertexShader * vertex_shader_ = nullptr;
	ID3D11PixelShader * pixel_shader_ = nullptr;
	ConstantBuffer * constant_buffer_ = nullptr;
	ID3D11ShaderResourceView * textures_ = nullptr;
};

struct Material
{
	ConstantBuffer * constant_buffer_ = nullptr;
	ID3D11ShaderResourceView * textures_ = nullptr;
	ID3D11Buffer * index_buffer_ = nullptr;
	unsigned int faces_num_ = 0;
	ID3D11SamplerState * sampler_ = nullptr;
};

namespace Entity
{
	class Visual : public ECS::Entity
	{
	private:
		VertexAndIndexBuffer * vi_buffer_ = nullptr;
		Shader * shader_ = nullptr;
		std::vector<Material*> materials_;

	public:
		Visual(void)
		{
			
		}

	public:
		virtual void Render(void);
	};
}