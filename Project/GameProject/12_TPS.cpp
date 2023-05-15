//--------------------------------------------
//�O���[�o���ϐ��̈�
//-------------------------------------------

bool ads = false;
CVector3D cam_rot(DtoR(45), 0, 0);
CVector3D cam_pos;
//�v���C���[�p�l�`���f��
CModelA3M p_model;
//��]�l
CVector3D p_rot(0, 0, 0);
//���W
CVector3D p_pos(0, 0, 0);
//�ړ������̃x�N�g��
CVector3D p_vec(0, 0, 0);
CVector3D key_dir(0, 0, 0);
//�L�����N�^�[�̈ړ���
const float move_speed = 0.1f;
//��]���x
float rot_speed = DtoR(5.0f);

//1�l�̃J�����Ȃ�
void UpdateCamera() {
	cam_pos = CVector3D(p_pos.x, p_pos.y + 2, p_pos.z);
	//��]���x�W��
	float cam_speed = 0.002f;

	//�}�E�X����ŉ�]
	CVector2D mouse_vec = CInput::GetMouseVec();
	//��]
	cam_rot += CVector3D(mouse_vec.y, -mouse_vec.x, 0) * cam_speed;
	//�㉺�p�x����
	cam_rot.x = min(DtoR(45), max(DtoR(-45), cam_rot.x));
	if (ads) {
		//��l��
		//�J�����ʒu�Ɖ�]�l�𔽉f
		CCamera::GetCurrent()->SetTranseRot(p_pos + CVector3D(0, 1.8, 0), cam_rot);
		
	} else {
		//�L�����N�^�[�̉E���z���ɃJ������ݒu����
		CMatrix cam_matrix = CMatrix::MTranselate(p_pos) * CMatrix::MTranselate(CVector3D(0, 1.5, 0)) * CMatrix::MRotation(cam_rot) * CMatrix::MTranselate(CVector3D(-0.5, 0.2, -2));
		//�J�������ʒu�ōs��Őݒ�
		CCamera::GetCurrent()->SetTranseRot(cam_matrix);
	}
}
void MainLoop(void) {

	//--------------------------------------------------------------
	//�Q�[�����̓����͂����ɏ���
	//�Q�[�����͂��̊֐�_��1�b�Ԃ�60��Ăяo���Ă���
	//--------------------------------------------------------------

	if (HOLD(CInput::eMouseR))
		ads = true;
	else
		ads = false;
	key_dir = CVector3D(0, 0, 0);
	//�L�����N�^�[����
	if (HOLD(CInput::eUp))key_dir.z = 1;
	if (HOLD(CInput::eDown))key_dir.z = -1;
	if (HOLD(CInput::eLeft))key_dir.x = 1;
	if (HOLD(CInput::eRight))key_dir.x = -1;
	//�L�[�������ꂽ�ꍇ�Ɉړ����鏈��
	if (key_dir.LengthSq() > 0) {
		CVector3D dir = CMatrix::MRotationY(p_rot.y) * key_dir;
		p_pos += dir * move_speed;
	}
	//w�������ꂽ��J�����̐��ʕ����Ɉړ�
	//CVector3D dir = CMatrix::MRotationY(p_rot.y) * CVector4D(0,0,1,0);
	//������������O�i
	/*if (HOLD(CInput::eUp)) {
		CVector3D dir = CMatrix::MRotationY(p_rot.y) * CVector4D(0, 0, 1, 0);
		p_pos += dir * move_speed;
		p_model.ChangeAnimation(1);
	}
	if (HOLD(CInput::eDown)){
		CVector3D dir = CMatrix::MRotationY(p_rot.y) * CVector4D(0, 0, -1, 0);
		p_pos += dir * move_speed;
		p_model.ChangeAnimation(1);
	}
	else {
		p_model.ChangeAnimation(0);
	}
	if (HOLD(CInput::eRight)) {
		CVector3D dir = CMatrix::MRotationY(p_rot.y) * CVector4D(-1, 0, 0, 0);
		p_pos += dir * move_speed;
		p_model.ChangeAnimation(2);
	}
	if (HOLD(CInput::eLeft)) {
		CVector3D dir = CMatrix::MRotationY(p_rot.y) * CVector4D(1, 0, 0, 0);
		p_pos += dir * move_speed;
		p_model.ChangeAnimation(3);
	}*/
	//��]
	//��
	if (HOLD(CInput::eButton5)) {
		p_rot.y += rot_speed;
	}
	//�E
	if (HOLD(CInput::eButton6)) {
		p_rot.y -= rot_speed;
	}

	//�J��������
	UpdateCamera();


	
	//�L�����N�^�[�\��
	//�A�j���[�V�����X�V
	p_model.UpdateAnimation();
	//�T�C�Y�ݒ�
	p_model.SetScale(0.01,0.01, 0.01);
	//�����ݒ�
	p_model.SetRot(p_rot);
	//���W�ݒ�
	p_model.SetPos(p_pos);
	//���̃{�[���𕔕��I�ɓ�����
	p_model.BindFrameMatrix(5, CMatrix::MRotation(cam_rot));
	//�`��
	p_model.Render();
	//���E�̎���\��
	Utility::DrawLine(CVector3D(0, 0, 0), CVector3D(100, 0, 0), CVector4D(1, 0, 0, 1));
	Utility::DrawLine(CVector3D(0, 0, 0), CVector3D(0, 100, 0), CVector4D(0, 1, 0, 1));
	Utility::DrawLine(CVector3D(0, 0, 0), CVector3D(0, 0, 100), CVector4D(0, 0, 1, 1));
	for (int i = -5; i <= 5; i++) {
		Utility::DrawLine(CVector3D(-5, 0, i * 1.0f), CVector3D(5, 0, i * 1.0f), CVector4D(0.2, 0.2, 0.2, 1));
		Utility::DrawLine(CVector3D(i * 1.0f, 0, -5), CVector3D(i * 1.0f, 0, 5), CVector4D(0.2, 0.2, 0.2, 1));
	}

	FONT_T()->Draw(0, 32, 0, 0, 0, "�}�E�X�E�Ŏ��_�ؑ�");

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
	CInput::SetButton(0, CInput::eButton4, VK_SPACE);
	CInput::SetButton(0, CInput::eButton5, 'Q');
	CInput::SetButton(0, CInput::eButton6, 'E');
	CInput::SetButton(0, CInput::eButton10, VK_RETURN);
	CInput::SetButton(0, CInput::eUp, 'W');
	CInput::SetButton(0, CInput::eDown, 'S');
	CInput::SetButton(0, CInput::eLeft, 'A');
	CInput::SetButton(0, CInput::eRight, 'D');
	CInput::SetButton(0, CInput::eMouseL, VK_LBUTTON);
	CInput::SetButton(0, CInput::eMouseR, VK_RBUTTON);
	CInput::SetButton(0, CInput::eMouseC, VK_MBUTTON);

	//�}�E�X��\��
	CInput::ShowCursor(false);
	CInput::SetMouseInside(true);
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
	ADD_RESOURCE("Amodel", CModel::CreateModel("Charactor/antman/antman.a3m"));
	p_model = COPY_RESOURCE("Amodel", CModelA3M);
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
	CCamera::GetCurrent()->Perspective(DtoR(45.0), (float)w / (float)h, 1.0, 10000.0);
	
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
	