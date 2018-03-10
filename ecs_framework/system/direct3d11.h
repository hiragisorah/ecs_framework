#pragma once

// エラー無効化(最低限)
#pragma warning(disable: 4005) //マクロ再定義

// フレームワーク
#include <game\system.h>
#include <system\window.h>
#include <utils\memory.h>

// API
#include <D3DX11.h>
#include <D3DX10.h>

// ライブラリファイル読込
#pragma comment(lib,"winmm.lib")
#pragma comment(lib,"d3dx10.lib")
#pragma comment(lib,"d3d11.lib")
#pragma comment(lib,"d3dx10.lib")
#pragma comment(lib,"d3dx11.lib")
#pragma comment(lib,"d3dCompiler.lib")

// 空クラス
namespace Entity
{
	class Visual;
}

namespace System
{
	class Direct3D11 : public ECS::System
	{
		friend ::Entity::Visual;

	private:
		ID3D11Device * device_;
		ID3D11DeviceContext * context_;
		IDXGISwapChain * swap_chain_;
		ID3D11Texture2D * back_buffer_ds_tex_;
		ID3D11RenderTargetView * back_buffer_rtv_;
		ID3D11DepthStencilView * back_buffer_dsv_;

	public:
		Direct3D11(void)
		{
			// windowシステムの取得
			auto window = Game::GetSystem<Window>();

			{// デバイスとスワップチェーンの作成
				DXGI_SWAP_CHAIN_DESC sd;
				ZeroMemory(&sd, sizeof(sd));
				sd.BufferCount = 1;
				sd.BufferDesc.Width = window->Width();
				sd.BufferDesc.Height = window->Height();
				sd.BufferDesc.Format = DXGI_FORMAT_R8G8B8A8_UNORM;
				sd.BufferDesc.RefreshRate.Numerator = 60;
				sd.BufferDesc.RefreshRate.Denominator = 1;
				sd.BufferUsage = DXGI_USAGE_RENDER_TARGET_OUTPUT;
				sd.OutputWindow = window->Hwnd();
				sd.SampleDesc.Count = 1;
				sd.SampleDesc.Quality = 0;
				sd.Windowed = true;

				D3D_FEATURE_LEVEL feature_levels = D3D_FEATURE_LEVEL_11_0;
				D3D_FEATURE_LEVEL * feature_level = nullptr;

				D3D11CreateDeviceAndSwapChain(nullptr, D3D_DRIVER_TYPE_HARDWARE, nullptr,
					0, &feature_levels, 1, D3D11_SDK_VERSION, &sd, &this->swap_chain_, &this->device_,
					feature_level, &this->context_);
			}

			{// 各種テクスチャーと、それに付帯する各種ビューを作成

			
				{// バックバッファーテクスチャーを取得
					ID3D11Texture2D * back_buffer_tex;
					swap_chain_->GetBuffer(0, __uuidof(ID3D11Texture2D), (LPVOID*)&back_buffer_tex);

					{// そのテクスチャーに対しレンダーターゲットビュー(RTV)を作成
						device_->CreateRenderTargetView(back_buffer_tex, nullptr, &this->back_buffer_rtv_);
						Utils::Memory::SafeRelease(back_buffer_tex);
					}
				}

				{// デプスステンシルビュー用のテクスチャーを作成
					D3D11_TEXTURE2D_DESC descDepth;
					descDepth.Width = window->Width();
					descDepth.Height = window->Height();
					descDepth.MipLevels = 1;
					descDepth.ArraySize = 1;
					descDepth.Format = DXGI_FORMAT_D32_FLOAT;
					descDepth.SampleDesc.Count = 1;
					descDepth.SampleDesc.Quality = 0;
					descDepth.Usage = D3D11_USAGE_DEFAULT;
					descDepth.BindFlags = D3D11_BIND_DEPTH_STENCIL;
					descDepth.CPUAccessFlags = 0;
					descDepth.MiscFlags = 0;
					device_->CreateTexture2D(&descDepth, nullptr, &this->back_buffer_ds_tex_);
					
					{// そのテクスチャーに対しデプスステンシルビュー(DSV)を作成
						device_->CreateDepthStencilView(this->back_buffer_ds_tex_, nullptr, &this->back_buffer_dsv_);
					}

					{// レンダーターゲットビューとデプスステンシルビューをパイプラインにセット
						context_->OMSetRenderTargets(1, &back_buffer_rtv_, this->back_buffer_dsv_);
					}
				}
			}

			{// ビューポートの作成
				D3D11_VIEWPORT vp;
				vp.Width = window->Width<float>();
				vp.Height = window->Height<float>();
				vp.MinDepth = 0.0f;
				vp.MaxDepth = 1.0f;
				vp.TopLeftX = 0;
				vp.TopLeftY = 0;
				
				{// ビューポートを設定
					context_->RSSetViewports(1, &vp);
				}
			}

			{// ラスタライザーの作成
				D3D11_RASTERIZER_DESC rdc;
				memset(&rdc, 0, sizeof(rdc));
				rdc.CullMode = D3D11_CULL_NONE;
				rdc.FillMode = D3D11_FILL_SOLID;
				ID3D11RasterizerState * rs = nullptr;
				device_->CreateRasterizerState(&rdc, &rs);
				
				{// ラスタライザーを設定
					context_->RSSetState(rs);
				}
			}
		}
		~Direct3D11(void)
		{
			Utils::Memory::SafeRelease(this->back_buffer_dsv_);
			Utils::Memory::SafeRelease(this->back_buffer_rtv_);
			Utils::Memory::SafeRelease(this->back_buffer_ds_tex_);
			Utils::Memory::SafeRelease(this->swap_chain_);
			Utils::Memory::SafeRelease(this->context_);
			Utils::Memory::SafeRelease(this->device_);
		}
		
		void Update(void) override
		{
			{// 画面クリア
				float ClearColor[4] = { 0,0,0.5,1 };// クリア色　RGBAの順
				this->context_->ClearRenderTargetView(this->back_buffer_rtv_, ClearColor);//カラーバッファクリア
				this->context_->ClearDepthStencilView(this->back_buffer_dsv_, D3D11_CLEAR_DEPTH, 1.0f, 0);//デプスステンシルバッファクリア
			}

			{// 描画

			}

			{// 画面更新
				this->swap_chain_->Present(1, 0);
			}
		}
	};
}