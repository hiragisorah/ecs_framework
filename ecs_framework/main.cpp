// フレームワーク
#include <game\game.h>
#include <system\window.h>
#include <system\direct3d11.h>

int __stdcall WinMain(HINSTANCE, HINSTANCE, char*, int)
{
	{// メモリーリーク検出
#ifdef _DEBUG
		_CrtSetDbgFlag(_CRTDBG_ALLOC_MEM_DF | _CRTDBG_LEAK_CHECK_DF);
#endif
	}

	{// 事前準備
		Game::AddSystem<System::Window>("ゲームタイトル", 1280, 720);
		Game::AddSystem<System::Direct3D11>();
	}
	
	{// 起動
		Game::Run();
	}
}