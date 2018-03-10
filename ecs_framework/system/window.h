#pragma once

// フレームワーク
#include <game\system.h>
#include <game\game.h>
#include <system\input.h>

// 標準
#include <vector>
#include <unordered_map>
#include <string>
#include <typeinfo>

// API
#include <Windows.h>

namespace System
{
	class Window : public ECS::System
	{
	private:
		HINSTANCE hinstance_;
		HWND hwnd_;
		unsigned int width_;
		unsigned int height_;
		
	public:
		Window(std::string window_name = "[unnamed]", unsigned int width = 1280, unsigned int height = 720) : width_(width), height_(height)
		{
			{// キー入力システムの追加
				Game::AddSystem<Input::WM>();
			}

			{// ハンドル取得
				this->hinstance_ = GetModuleHandleA(nullptr);
			}

			{// ウィンドウクラスの登録
				WNDCLASSEX  wc;
				memset(&wc, 0, sizeof(wc));
				wc.cbSize = sizeof(wc);
				wc.style = CS_HREDRAW | CS_VREDRAW;
				wc.lpfnWndProc = WndProc;
				wc.hInstance = this->hinstance_;
				wc.hIcon = LoadIconA(nullptr, IDI_APPLICATION);
				wc.hCursor = LoadCursorA(nullptr, IDC_ARROW);
				wc.hbrBackground = (HBRUSH)GetStockObject(DKGRAY_BRUSH);
				wc.lpszClassName = window_name.c_str();
				wc.hIconSm = LoadIconA(nullptr, IDI_APPLICATION);
				RegisterClassExA(&wc);
			}

			{// ウィンドウの大きさの再調整、中央配置計算
				RECT rc;
				rc.left = 0;
				rc.top = 0;
				rc.right = static_cast<long>(this->width_);
				rc.bottom = static_cast<long>(this->height_);

				AdjustWindowRectEx(&rc, WS_OVERLAPPEDWINDOW, true, 0);

				int w = rc.right - rc.left;
				int h = rc.bottom - rc.top;

				RECT rc_desk;
				GetWindowRect(GetDesktopWindow(), &rc_desk);

				int x = rc_desk.right / 2 - w / 2;
				int y = rc_desk.bottom / 2 - h / 2;

				{// ウィンドウの生成、表示
					this->hwnd_ = CreateWindowExA(0, window_name.c_str(), window_name.c_str(), WS_OVERLAPPEDWINDOW,
						x, y, w, h, 0, 0, this->hinstance_, 0);

					ShowWindow(this->hwnd_, SW_SHOW);
				}
			}
		}

	public:
		HWND const Hwnd(void)
		{
			return this->hwnd_;
		}
		template<class _Cast = unsigned int>
		_Cast const Width(void)
		{
			return static_cast<_Cast>(this->width_);
		}
		template<class _Cast = unsigned int>
		_Cast const Height(void)
		{
			return static_cast<_Cast>(this->height_);
		}

	private:
		static LRESULT __stdcall WndProc(HWND hWnd, UINT iMsg, WPARAM wParam, LPARAM lParam)
		{
			auto input = Game::GetSystem<Input::WM>();

			switch (iMsg)
			{
			case WM_DESTROY:PostQuitMessage(0); break;
			case WM_MOUSEMOVE:input->MousePosX = LOWORD(lParam); input->MousePosY = LOWORD(lParam); break;
			case WM_LBUTTONDOWN:input->IsOldLClick = input->IsLClick; input->IsLClick = true; break;
			case WM_RBUTTONDOWN:input->IsOldRClick = input->IsRClick; input->IsRClick = true; break;
			case WM_LBUTTONUP:input->IsOldLClick = input->IsLClick; input->IsLClick = false; break;
			case WM_RBUTTONUP:input->IsOldRClick = input->IsRClick; input->IsRClick = false; break;
			case WM_KEYDOWN:input->IsOldKeys[wParam] = input->IsKeys[wParam]; input->IsKeys[wParam] = true; if (wParam == VK_ESCAPE) PostQuitMessage(0); break;
			case WM_KEYUP:input->IsOldKeys[wParam] = input->IsKeys[wParam]; input->IsKeys[wParam] = false; break;
			}
			return DefWindowProc(hWnd, iMsg, wParam, lParam);
		}

	public:
		bool MessageLoop(void)
		{
			MSG msg = { 0 };
			memset(&msg, 0, sizeof(msg));

			while (PeekMessageA(&msg, nullptr, 0, 0, PM_REMOVE))
			{
				TranslateMessage(&msg);
				DispatchMessageA(&msg);
				if (msg.message == WM_QUIT) return false;
			}
			return true;
		}

	public:
		void Update(void) override
		{
			if (!this->MessageLoop()) Game::Shutdown();
		}
	};
}