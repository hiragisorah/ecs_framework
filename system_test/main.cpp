//�t���[�����[�N
#include <game\game.h>
#include <system\window.h>
#include <system\direct3d11.h>

void main(void)
{
	//���O����
	Game::AddSystem<System::Window>("�Q�[���^�C�g��", 1280, 720);
	Game::AddSystem<System::Direct3D11>();

	//�N��
	Game::Run();
}