#include "visual.h"

// �t���[�����[�N
#include <system\direct3d11.h>

void Entity::Visual::Render(void)
{
	auto d3d = Game::GetSystem<System::Direct3D11>();

	{//�g�p����V�F�[�_�[�̓o�^	
		d3d->context_->VSSetShader(this->shader_->vertex_shader_, nullptr, 0);
		d3d->context_->PSSetShader(this->shader_->pixel_shader_, nullptr, 0);
	}

	{// �V�F�[�_�[�̃R���X�^���g�o�b�t�@�[�Ɋe��f�[�^��n��
		D3D11_MAPPED_SUBRESOURCE data;
		if (SUCCEEDED(d3d->context_->Map(this->shader_->constant_buffer_->buffer_, 0, D3D11_MAP_WRITE_DISCARD, 0, &data)))
		{
			memcpy_s(data.pData, data.RowPitch, (void*)&this->shader_->constant_buffer_->pointer_, this->shader_->constant_buffer_->size_);
			d3d->context_->Unmap(this->shader_->constant_buffer_->buffer_, 0);
		}
	}

	{// ���̃R���X�^���g�o�b�t�@�[���g���V�F�[�_�[�̓o�^
		d3d->context_->VSSetConstantBuffers(1, 1, &this->shader_->constant_buffer_->buffer_);
		d3d->context_->PSSetConstantBuffers(1, 1, &this->shader_->constant_buffer_->buffer_);
	}

	{// ���_�C���v�b�g���C�A�E�g���Z�b�g
		d3d->context_->IASetInputLayout(this->shader_->vertex_layout_);
	}

	{// �v���~�e�B�u�E�g�|���W�[���Z�b�g
		d3d->context_->IASetPrimitiveTopology(this->vi_buffer_->topology_);
	}

	{// �o�[�e�b�N�X�o�b�t�@�[���Z�b�g
		UINT stride = this->vi_buffer_->vertex_size_;
		UINT offset = 0;

		d3d->context_->IASetVertexBuffers(0, 1, &this->vi_buffer_->vertex_buffer_, &stride, &offset);
	}

	{// �}�e���A���̐������A���ꂼ��̃}�e���A���̃C���f�b�N�X�o�b�t�@�|��`��
		for (size_t n = 0; n < this->materials_.size(); n++)
		{
			// �g�p����Ă��Ȃ��}�e���A���΍�
			if (this->materials_[n]->faces_num_ == 0)
				continue;

			// �C���f�b�N�X�o�b�t�@�[���Z�b�g
			UINT stride = this->vi_buffer_->index_size_;
			UINT offset = 0;

			d3d->context_->IASetIndexBuffer(this->materials_[n]->index_buffer_, DXGI_FORMAT_R32_UINT, 0);

			// �}�e���A���̊e�v�f���G�t�F�N�g�i�V�F�[�_�[�j�ɓn��
			D3D11_MAPPED_SUBRESOURCE data;
			if (SUCCEEDED(d3d->context_->Map(this->materials_[n]->constant_buffer_->buffer_, 0, D3D11_MAP_WRITE_DISCARD, 0, &data)))
			{
				memcpy_s(data.pData, data.RowPitch, (void*)&this->materials_[n]->constant_buffer_->pointer_, this->materials_[n]->constant_buffer_->size_);
				d3d->context_->Unmap(this->materials_[n]->constant_buffer_->buffer_, 0);
			}

			d3d->context_->VSSetConstantBuffers(2, 1, &this->materials_[n]->constant_buffer_->buffer_);
			d3d->context_->PSSetConstantBuffers(2, 1, &this->materials_[n]->constant_buffer_->buffer_);

			// �e�N�X�`���[���V�F�[�_�[�ɓn��
			d3d->context_->PSSetSamplers(0, 1, &this->materials_[n]->sampler_);
			d3d->context_->PSSetShaderResources(0, 1, &this->materials_[n]->textures_);

			// �v���~�e�B�u�������_�����O
			d3d->context_->DrawIndexed(this->materials_[n]->faces_num_ * this->vi_buffer_->vertices_per_face_, 0, 0);
		}
	}
}
