// �t���[�����[�N
#include <game\game.h>
#include <system\window.h>
#include <system\direct3d11.h>

int __stdcall WinMain(HINSTANCE, HINSTANCE, char*, int)
{
	{// �������[���[�N���o
#ifdef _DEBUG
		_CrtSetDbgFlag(_CRTDBG_ALLOC_MEM_DF | _CRTDBG_LEAK_CHECK_DF);
#endif
	}

	{// ���O����
		Game::AddSystem<System::Window>("�Q�[���^�C�g��", 1280, 720);
		Game::AddSystem<System::Direct3D11>();
	}
	
	{// �N��
		Game::Run();
	}
}