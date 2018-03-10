#include "visual.h"

// フレームワーク
#include <system\direct3d11.h>

void Entity::Visual::Render(void)
{
	auto d3d = Game::GetSystem<System::Direct3D11>();

	{//使用するシェーダーの登録	
		d3d->context_->VSSetShader(this->shader_->vertex_shader_, nullptr, 0);
		d3d->context_->PSSetShader(this->shader_->pixel_shader_, nullptr, 0);
	}

	{// シェーダーのコンスタントバッファーに各種データを渡す
		D3D11_MAPPED_SUBRESOURCE data;
		if (SUCCEEDED(d3d->context_->Map(this->shader_->constant_buffer_->buffer_, 0, D3D11_MAP_WRITE_DISCARD, 0, &data)))
		{
			memcpy_s(data.pData, data.RowPitch, (void*)&this->shader_->constant_buffer_->pointer_, this->shader_->constant_buffer_->size_);
			d3d->context_->Unmap(this->shader_->constant_buffer_->buffer_, 0);
		}
	}

	{// このコンスタントバッファーを使うシェーダーの登録
		d3d->context_->VSSetConstantBuffers(1, 1, &this->shader_->constant_buffer_->buffer_);
		d3d->context_->PSSetConstantBuffers(1, 1, &this->shader_->constant_buffer_->buffer_);
	}

	{// 頂点インプットレイアウトをセット
		d3d->context_->IASetInputLayout(this->shader_->vertex_layout_);
	}

	{// プリミティブ・トポロジーをセット
		d3d->context_->IASetPrimitiveTopology(this->vi_buffer_->topology_);
	}

	{// バーテックスバッファーをセット
		UINT stride = this->vi_buffer_->vertex_size_;
		UINT offset = 0;

		d3d->context_->IASetVertexBuffers(0, 1, &this->vi_buffer_->vertex_buffer_, &stride, &offset);
	}

	{// マテリアルの数だけ、それぞれのマテリアルのインデックスバッファ－を描画
		for (size_t n = 0; n < this->materials_.size(); n++)
		{
			// 使用されていないマテリアル対策
			if (this->materials_[n]->faces_num_ == 0)
				continue;

			// インデックスバッファーをセット
			UINT stride = this->vi_buffer_->index_size_;
			UINT offset = 0;

			d3d->context_->IASetIndexBuffer(this->materials_[n]->index_buffer_, DXGI_FORMAT_R32_UINT, 0);

			// マテリアルの各要素をエフェクト（シェーダー）に渡す
			D3D11_MAPPED_SUBRESOURCE data;
			if (SUCCEEDED(d3d->context_->Map(this->materials_[n]->constant_buffer_->buffer_, 0, D3D11_MAP_WRITE_DISCARD, 0, &data)))
			{
				memcpy_s(data.pData, data.RowPitch, (void*)&this->materials_[n]->constant_buffer_->pointer_, this->materials_[n]->constant_buffer_->size_);
				d3d->context_->Unmap(this->materials_[n]->constant_buffer_->buffer_, 0);
			}

			d3d->context_->VSSetConstantBuffers(2, 1, &this->materials_[n]->constant_buffer_->buffer_);
			d3d->context_->PSSetConstantBuffers(2, 1, &this->materials_[n]->constant_buffer_->buffer_);

			// テクスチャーをシェーダーに渡す
			d3d->context_->PSSetSamplers(0, 1, &this->materials_[n]->sampler_);
			d3d->context_->PSSetShaderResources(0, 1, &this->materials_[n]->textures_);

			// プリミティブをレンダリング
			d3d->context_->DrawIndexed(this->materials_[n]->faces_num_ * this->vi_buffer_->vertices_per_face_, 0, 0);
		}
	}
}
