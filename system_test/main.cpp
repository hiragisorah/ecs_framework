//フレームワーク
#include <game\game.h>
#include <system\window.h>
#include <system\direct3d11.h>

void main(void)
{
	//事前準備
	Game::AddSystem<System::Window>("ゲームタイトル", 1280, 720);
	Game::AddSystem<System::Direct3D11>();

	//起動
	Game::Run();
}