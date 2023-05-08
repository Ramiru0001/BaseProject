#include <GLLibrary.h>
#define SCREEN_WIDTH 1920
#define SCREEN_HEIGHT 1080



//--------------------------------------------
//�O���[�o���ϐ��̈�
//-------------------------------------------

//���[�V�����L���f���N���X
CModelA3M model;
//���W
CVector3D pos(0, 0, 0);
//��]
CVector3D rot(0, 0, 0);



//�^�[�Q�b�g���W
CVector3D target(0, 0, 0);

//�t�B�[���h���f��
CModelField* field;


void MainLoop(void) {

	//--------------------------------------------------------------
	//�Q�[�����̓����͂����ɏ���
	//�Q�[�����͂��̊֐�_��1�b�Ԃ�60��Ăяo���Ă���
	//--------------------------------------------------------------
		//�L�����N�^�[�ړ�
	//�ړ���
	const float move_speed = 0.05f;

	//�L�����N�^�[���^�[�Q�b�g�֌����Ĉړ�������
	//�^�[�Q�b�g�ւ̃x�N�g��
	CVector3D vec = target - pos;
	//��]�l���t�Z
	rot.y = atan2(vec.x, vec.z);
	//������1�ȏ�Ȃ�
	if (vec.Length() >= 1.0f) {
		//�ړ�����
		CVector3D dir(sin(rot.y), 0, cos(rot.y));
		//�ړ�
		pos += dir * move_speed;
		//1�ԁi�ҋ@�j�̃A�j���[�V�������Đ�
		model.ChangeAnimation(1);
	}
	else {
		//0�ԁi�ҋ@�j�̃A�j���[�V�������Đ�
		model.ChangeAnimation(0);
	}
	//�A�j���[�V�������X�V
	model.UpdateAnimation();

	{//�L�����N�^�[�̐ڒn����
		//�ڐG�_�Ɩ@��
		CVector3D cross, normal;
		//�t�B�[���h�Ɛ����i�L�����N�^�[�̓��������j�Ƃ̏Փ˔���
		if (field->CollisionRay(&cross, &normal, pos + CVector3D(0, 10, 0), pos + CVector3D(0, -10, 0))) {
			//�����̂ݐݒ�
			pos.y = cross.y;
		}
	}
	//�J�����ݒ�
	CCamera::GetCamera()->LookAt(pos + CVector3D(10, 20, 10),
		pos,
		CVector3D(0.0, 1.0, 0.0));

	//�L�����N�^�[�̍��W�A��]�A�X�P�[���ݒ�ƕ\��
	model.SetPos(pos);
	model.SetRot(rot);
	model.SetScale(0.01, 0.01, 0.01);
	model.Render();


	field->Render();
	Utility::DrawLine(CVector3D(0, 0, 0), CVector3D(100, 0, 0), CVector4D(1, 0, 0, 1));
	Utility::DrawLine(CVector3D(0, 0, 0), CVector3D(0, 100, 0), CVector4D(0, 1, 0, 1));
	Utility::DrawLine(CVector3D(0, 0, 0), CVector3D(0, 0, 100), CVector4D(0, 0, 1, 1));




	{
		static float distance = 0.99f;
		//distance += CInput::GetMouseWheel()*0.01f;
		//distance = max(0.9f,min(1.0f, distance));

		//���}�E�X�̃N���b�N�����ꏊ���w��(�X�N���[�����W�����[���h���W)
		//�}�E�X�̍��W�i�X�N���[�����W�j
		CVector2D m = CInput::GetMousePoint();
		//�X�N���[�����W�����[���h���W�ϊ�
		//���[���h���W�F��
		CVector3D m_w_near = Utility::ScreenToWorld(CVector3D(m.x, m.y, 0));
		//���[���h���W�F��
		CVector3D m_w_far = Utility::ScreenToWorld(CVector3D(m.x, m.y, distance));
		//�}�E�X�̃��[���h���W(��)��\��
		//Utility::DrawSphere(m_w_far, 1.0f, CVector4D(1, 0, 0, 1));
		

		//�}�E�X�̃��[���h���W�F�߂��烏�[���h���W�F���ւ̐�����
		//�t�B�[���h���f���Ƃ̏Փ�
		//�ڐG�_�Ɩ@��
		CVector3D cross, normal;
		//�t�B�[���h�Ɛ����Ƃ̏Փ˔���
		if (field->CollisionRay(&cross, &normal, m_w_near, m_w_far)) {
			//�ڐG�_�̕\��
			Utility::DrawCube(cross, 1.0f, CVector4D(1, 1, 0, 1));
			//�N���b�N�Ń^�[�Q�b�g�ʒu�X�V
			if (PUSH(CInput::eMouseL))
				target = cross;
		}
	}

	//�^�[�Q�b�g�̕\��
	Utility::DrawCube(target, 1.0f, CVector4D(0, 1, 0, 1));

	
	{//���L�����N�^�[�̍��W�ɖ��O��\������i���[���h���W���X�N���[�����W�j
		CVector3D pos2D;

		//���[���h���W���X�N���[�����W
		pos2D = Utility::WorldToScreen(pos);

		//�����̕\��
		FONT_T()->Draw(pos2D.x, pos2D.y, 1, 0, 0, "Player");

	}
	{//���^�[�Q�b�g�̍��W�ɖ��O��\������i���[���h���W���X�N���[�����W�j
		CVector3D pos2D;


		//���[���h���W���X�N���[�����W
		pos2D = Utility::WorldToScreen(pos);
		//�����̕\��
		FONT_T()->Draw(pos2D.x, pos2D.y, 1, 0, 0, "Target");

	}
}

void Init(void)
{
	glClearColor(1.0, 1.0, 1.0, 1.0);

	glEnable(GL_DEPTH_TEST);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
	glEnable(GL_BLEND);//�u�����h�̗L����
	glEnable(GL_CULL_FACE);
	glCullFace(GL_BACK);

	//�Œ�V�F�[�_�[�p
	glEnable(GL_LIGHTING);
	glEnable(GL_LIGHT0);

	glEnable(GL_ALPHA_TEST);

	CFPS::SetFPS(60);
	//�t���[�����䏉����
	CFPS::Init();
	//�{�^���̐ݒ�
	CInput::Init();
	CInput::SetButton(0, CInput::eButton1, 'Z');
	CInput::SetButton(0, CInput::eButton2, 'X');
	CInput::SetButton(0, CInput::eButton3, 'C');
	CInput::SetButton(0, CInput::eButton4, 'V');
	CInput::SetButton(0, CInput::eButton5, VK_SPACE);
	CInput::SetButton(0, CInput::eButton10, VK_RETURN);
	CInput::SetButton(0, CInput::eUp, 'W');
	CInput::SetButton(0, CInput::eDown, 'S');
	CInput::SetButton(0, CInput::eLeft, 'A');
	CInput::SetButton(0, CInput::eRight, 'D');
	CInput::SetButton(0, CInput::eMouseL, VK_LBUTTON);
	CInput::SetButton(0, CInput::eMouseR, VK_RBUTTON);
	CInput::SetButton(0, CInput::eMouseC, VK_MBUTTON);

	//�}�E�X��\��
	//	CInput::ShowCursor(false);
	//	CInput::SetMouseInside(true);
	CInput::Update();
	CInput::Update();


	//���C�g�ݒ�
	CLight::SetType(0, CLight::eLight_Direction);
	CLight::SetPos(0, CVector3D(0, 200, 200));
	CLight::SetDir(0, CVector3D(-1, -2, 1).GetNormalize());
	CLight::SetColor(0, CVector3D(0.2f, 0.2f, 0.2f), CVector3D(0.8f, 0.8f, 0.8f));


	CLight::SetFogParam(CVector4D(1, 1, 1, 1), 700, 800);

	//�J����������
	CCamera::GetCamera()->LookAt(CVector3D(5, 5, 5),
		CVector3D(0, 0, 0),
		CVector3D(0.0, 1.0, 0.0));

	SetCurrentDirectory("data");


	CShader::GetInstance("StaticMesh");
	CShader::GetInstance("SkinMesh");
	CSound::GetInstance();

	SetCurrentDirectory("data");


	//-----------------------------------------------------
	//�������̖��߂�����
	//�Q�[���N�����Ɉ�x�����Ă΂��
	//-----------------------------------------------------

	
	ADD_RESOURCE("Antman", CModel::CreateModel("Charactor/antman/antman.a3m"));
	ADD_RESOURCE("Field", CModel::CreateModel("Field/Land/land.field"));
	ADD_RESOURCE("Marker", CImage::CreateImage("UI/marker.png"));

	model = COPY_RESOURCE("Antman", CModelA3M);
	//�t�B�[���h���f���͓ǂݍ��݃X���b�h�������Ă��邽�ߕ������Ȃ�
	field = GET_RESOURCE("Field", CModelField);
	field->SetScale(5, 5, 5);

}


void Release()
{
	CLoadThread::ClearInstance();
	CSound::ClearInstance();
	CResourceManager::ClearInstance();
}

static void ResizeCallback(GLFWwindow* window, int w, int h)
{
	glViewport(0, 0, w, h);

	//��ʉ𑜓x�ϓ�
	CCamera::GetCamera()->SetSize((float)w, (float)h);
	//��ʉ𑜓x�Œ�
	//CCamera::GetCamera()->SetSize(SCREEN_WIDTH, SCREEN_HEIGHT);

	CCamera::GetCamera()->Viewport(0, 0, w, h);
	CCamera::GetCurrent()->Perspective(DtoR(45.0), (float)w / (float)h, 0.2, 10000.0);
	
	glfwGetWindowPos(window, &GL::window_x, &GL::window_y);
	GL::UpdateWindowRect(GL::window_x, GL::window_y, w, h);
	CInput::UpdateClipCursor(true);

}
static void WheelCallback(GLFWwindow* _window, double _offsetx, double _offsety) {
	CInput::AddMouseWheel((int)_offsety);

}
static void PosCallback(GLFWwindow* _window, int x, int y) {
	GL::window_x = x;
	GL::window_y = y;
	GL::UpdateWindosRect(x, y, GL::window_width, GL::window_height);
	CInput::UpdateClipCursor(true);

}
static void FocusCallback(GLFWwindow* _window, int f) {
	CInput::UpdateClipCursor(f);
	GL::focus = f;
}

static void error_callback(int error, const char* description)
{
	printf("Error: %s\n", description);
}

//�t���X�N���[��?�E�C���h�E���[�h�̐؂�ւ�
//Alt+Enter�Ő؂�ւ���
void CheckFullScreen() {
	static int key_enter = 0;
	int key_enter_buf = key_enter;
	if (key_enter_buf ^ (key_enter = glfwGetKey(GL::window, GLFW_KEY_ENTER)) && key_enter && (glfwGetKey(GL::window, GLFW_KEY_LEFT_ALT) || glfwGetKey(GL::window, GLFW_KEY_RIGHT_ALT))) {
		GL::ChangeFullScreen(!GL::full_screen);
	}
}

int __main(int* argcp, char** argv) {
	// ���������[�N���o
	//	_CrtSetDbgFlag(_CRTDBG_ALLOC_MEM_DF | _CRTDBG_LEAK_CHECK_DF | _CRTDBG_CHECK_ALWAYS_DF);
	//_CrtSetDbgFlag(_CRTDBG_ALLOC_MEM_DF | _CRTDBG_LEAK_CHECK_DF);


	//OpenGL4.3
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);


	glfwSetErrorCallback(error_callback);
	if (!glfwInit()) return -1;
	//	glutInit(argcp, argv);

	GL::window = glfwCreateWindow(SCREEN_WIDTH, SCREEN_HEIGHT, "Simple", nullptr, nullptr);
	glfwGetWindowSize(GL::window, &GL::window_width, &GL::window_height);
	glfwGetWindowPos(GL::window, &GL::window_x, &GL::window_y);

	glfwSetFramebufferSizeCallback(GL::window, ResizeCallback);
	glfwSetScrollCallback(GL::window, WheelCallback);
	glfwSetWindowFocusCallback(GL::window, FocusCallback);
	glfwSetWindowPosCallback(GL::window, PosCallback);
	if (!GL::window) {
		glfwTerminate();
		return -1;
	}

	glfwMakeContextCurrent(GL::window);
	glfwSwapInterval(1);
	ResizeCallback(GL::window, SCREEN_WIDTH, SCREEN_HEIGHT);

	GLenum err = glewInit();
	if (err == GLEW_OK) {
		printf("%s\n", glewGetString(GLEW_VERSION));
	}
	else {
		printf("%s\n", glewGetErrorString(err));
		getchar();
		return -1;
	}
	HDC glDc = wglGetCurrentDC();
	GL::hWnd = WindowFromDC(glDc);

	Init();
	while (!glfwWindowShouldClose(GL::window)) {
		CheckFullScreen();


		CInput::Update();
		//�e�o�b�t�@�[���N���A
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

		MainLoop();


		glfwSwapBuffers(GL::window);


		CFPS::Wait();

		char title[32];
		sprintf_s(title, "math fps:%d dt:%.3f", CFPS::GetFPS(), CFPS::GetDeltaTime());
		glfwSetWindowTitle(GL::window, title);

		glfwPollEvents();
		if (glfwGetKey(GL::window, GLFW_KEY_ESCAPE)) {
			glfwSetWindowShouldClose(GL::window, GL_TRUE);
		}

	}

	glfwTerminate();




	Release();
	return 0;
}

INT WINAPI WinMain(HINSTANCE, HINSTANCE, LPSTR argv, INT argc)
{
	return __main(&__argc, __argv);
}

int main(int argc, char** argv)
{
	return __main(&argc, argv);
}

void RenderSkybox() {
	static CModel* skybox = nullptr;
	if (!skybox) {
		ADD_RESOURCE("Sky", CModel::CreateModel("Field/Sky/Sky.obj"));
		skybox = GET_RESOURCE("Sky", CModel);
		//���C���J�����̐ݒ�𕡐�
		*CCamera::GetCamera(CCamera::eSkyBox) = *CCamera::GetCamera(CCamera::eMainCamera);
		for (int i = 0; i < skybox->GetMaterialSize(); i++) {
			if (CTexture* t = skybox->GetMaterial(i)->mp_texture) {
				t->SetWrapST(GL_CLAMP_TO_EDGE);
				t->SetFilter(GL_LINEAR);
			}
		}
	}
	//�f�v�X�e�X�gOFF
	//�����ł̕`��͐[�x�o�b�t�@�։e�����Ȃ�
	glDisable(GL_DEPTH_TEST);
	//���ݎg�p���̃J�������擾
	CCamera* back = CCamera::GetCurrent();
	//���݂̃J�����̃r���[�s����擾
	CMatrix matrix = back->GetViewMatrix();
	//�X�J�{�b�N�X�p�J�������擾
	CCamera* c = CCamera::GetCamera(CCamera::eSkyBox);
	//�r���[�s��̍��W�𖳌���
	matrix.m03 = 0; matrix.m13 = 0; matrix.m23 = 0;
	//�X�J�C�{�b�N�X�p�̃r���[�s��ݒ�
	c->SetViewMatrix(matrix);
	//�X�J�C�{�b�N�X�p�̓��e�s��ݒ�(���݂̃J�����Ɠ����l�ɂ���)
	c->SetProjectionMatrix(back->GetProjectionMatrix());
	//�g�p����J�������X�J�C�{�b�N�X�p�ɐݒ�
	CCamera::SetCurrent(c);
	//���C�e�B���OOFF
	CLight::SetLighting(false);


	//�X�J�{�b�N�X�`��
	skybox->Render();


	//���̐ݒ�ɖ߂�
	CLight::SetLighting(true);
	CCamera::SetCurrent(back);
	glEnable(GL_DEPTH_TEST);

}
