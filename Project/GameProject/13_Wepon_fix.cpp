//--------------------------------------------
//�O���[�o���ϐ��̈�
//-------------------------------------------

//���[�V�����L���f���N���X
CModelA3M model;
//���W
CVector3D pos(0, 0, -5);
//��]
CVector3D rot(0, 0, 0);

//�L�����N�^�[�̈ړ���
const float move_speed = 0.1f;

//��]
CVector3D cam_rot(0, 0, 0);


//����̃��f��
CModelObj gun_model;
//����̍s��
CMatrix gun_matrix;

//����̃��f��
CModelObj sword_model;
//����̍s��
CMatrix sword_matrix;



//TPS
void TPSUpdate() {

	//��]���x�W��
	float cam_speed = 0.002f;

	//���}�E�X����ŉ�]----
	CVector2D mouse_vec = CInput::GetMouseVec();
	cam_rot += CVector3D(mouse_vec.y, -mouse_vec.x, 0) * cam_speed;
	cam_rot.x = min(DtoR(45), max(DtoR(-45), cam_rot.x));


	//�L�����N�^�[�̉E�������ɃJ������ݒu����
	CMatrix cam_matrix = CMatrix::MTranselate(pos)								//�L�����N�^�[�̍s��
		* CMatrix::MTranselate(CVector3D(0, 1.5, 0)) * CMatrix::MRotation(cam_rot) //(���ւ̕��s�ړ�*��]�j
		* CMatrix::MTranselate(CVector3D(-1.5, 0.1, -2.0));							//(������E�����֕��s�ړ�)
	//�J�����̃��[���h�s��ŃJ������ݒ�
	CCamera::GetCurrent()->SetTranseRot(cam_matrix);


	//�����L�[�̓��͕����x�N�g��
	CVector3D key_dir(0, 0, 0);

	//�L�����N�^�[�̉�]�l���J�����̉�]�l�ɍ��킹��
	rot.y = cam_rot.y;

	//�����L�[������͕����x�N�g����ݒ�
	if (HOLD(CInput::eUp)) key_dir.z = 1;
	if (HOLD(CInput::eDown)) key_dir.z = -1;
	if (HOLD(CInput::eLeft)) key_dir.x = 1;
	if (HOLD(CInput::eRight)) key_dir.x = -1;

	//�L�[���͂������
	if (key_dir.LengthSq() > 0) {
		//���ړ����� ��]�s��~�L�[����
		CVector3D dir = CMatrix::MRotationY(rot.y) * key_dir;
		pos += dir * move_speed;
		model.ChangeAnimation(1);
	}
	else {
		model.ChangeAnimation(0);
	}

}
//ACT
void ACTUpdate() {
	//---------------------------------------
		//�����_�܂ł̋���
	static float cam_dist = 5.0f;
	//��]���x�W��
	float cam_speed = 0.001f;
	//�}�E�X�̈ړ���
	CVector2D mouse_vec = CInput::GetMouseVec();
	//�}�E�X�ړ�����J��������]
	//�}�E�X�ړ�Y��X����](�㉺)
	//�}�E�X�ړ�X��Y����](���E)
	cam_rot += CVector3D(mouse_vec.y, -mouse_vec.x, 0) * cam_speed;
	//�㉺�p����
	cam_rot.x = min(DtoR(60), max(DtoR(-35), cam_rot.x));
	//-180�`180�ɐ��K��
	cam_rot.y = Utility::NormalizeAngle(cam_rot.y);
	//��������
	cam_dist = min(10.0f, max(2.0f, cam_dist - CInput::GetMouseWheel()));


	//�ʒu�ƒ����_��ݒ�
	CVector3D cam_at = pos + CVector3D(0, 1.5, 0);
	CVector3D cam_pos = cam_at + CMatrix::MRotation(cam_rot).GetFront() * -cam_dist;
	CCamera::GetCurrent()->LookAt(cam_pos, cam_at, CVector3D(0, 1, 0));

	//----------------------------------------

	float cam_ang = 0;

	//�J�����̉�]�l������ꍇ
	cam_ang = cam_rot.y;

	//�����L�[�̃x�N�g��
	CVector3D key_dir(0, 0, 0);



	//�����L�[������͕����x�N�g����ݒ�
	if (HOLD(CInput::eUp)) key_dir.z = 1;
	if (HOLD(CInput::eDown)) key_dir.z = -1;
	if (HOLD(CInput::eLeft)) key_dir.x = 1;
	if (HOLD(CInput::eRight)) key_dir.x = -1;

	//���͂������
	if (key_dir.LengthSq() > 0) {
		//�L�[�̕����x�N�g�����p�x�ɋt�Z
		float key_ang = atan2(key_dir.x, key_dir.z);
		//�J�����̊p�x�{�L�[�̊p�x�ŃL�����N�^�[�̊p�x�����܂�
		rot.y = Utility::NormalizeAngle(key_ang + cam_ang);
		//�����x�N�g��
		CVector3D dir(sin(rot.y), 0, cos(rot.y));
		//�ړ�
		pos += dir * move_speed;
		model.ChangeAnimation(18);
	}
	else {
		model.ChangeAnimation(17);
	}

}

void MainLoop(void) {

	//--------------------------------------------------------------
	//�Q�[�����̓����͂����ɏ���
	//�Q�[�����͂��̊֐�_��1�b�Ԃ�60��Ăяo���Ă���
	//--------------------------------------------------------------

	static bool sw = 1;
	if (PUSH(CInput::eMouseL)) sw = !sw;
	if (sw)
		TPSUpdate();
	else
		ACTUpdate();

	
	//���[�V�����̍X�V
	model.UpdateAnimation();




	//�L�����N�^�[�\��
	model.SetPos(pos);
	model.SetRot(rot);
	model.SetScale(0.01f, 0.01f, 0.01f);
	if(sw)
		//TPS
		model.BindFrameMatrix(5, CMatrix::MRotation(cam_rot));
	else
		//ACT
		model.UnbindFrameMatrix(5);
	model.Render();

	//������̕`��
	//����̃��[���h�s��=�{�[���̍s��@�~�@����̃��[�J���s��i���s�ړ��~��]�~�X�P�[���j
	if (sw) {
		//TPS
		gun_matrix = model.GetFrameMatrix(31) * CMatrix::MTranselate(-18.00f, -5.00f, 8.600) * CMatrix::MRotationX(DtoR(0)) * CMatrix::MRotationY(DtoR(-90)) * CMatrix::MRotationZ(DtoR(-90));
		sword_matrix = model.GetFrameMatrix(3) * CMatrix::MTranselate(18, -4.98, -21) * CMatrix::MRotationX(DtoR(48)) * CMatrix::MRotationY(DtoR(-102)) * CMatrix::MRotationZ(DtoR(-90)) * CMatrix::MScale(0.5f, 0.5f, 0.5f);
	}else{
		//ACT
		gun_matrix = model.GetFrameMatrix(4) * CMatrix::MTranselate(20.0f, 20.0f, -4.7f) * CMatrix::MRotationX(DtoR(0)) * CMatrix::MRotationY(DtoR(-114)) * CMatrix::MRotationZ(DtoR(90));
		sword_matrix = model.GetFrameMatrix(31) * CMatrix::MTranselate(-10.4, -5.3, -3.6) * CMatrix::MRotationX(DtoR(0)) * CMatrix::MRotationY(DtoR(0)) * CMatrix::MRotationZ(DtoR(0)) * CMatrix::MScale(0.5f, 0.5f, 0.5f);
	}
	//�s����w�肵�ĕ`��
	gun_model.Render(gun_matrix);
	sword_model.Render(sword_matrix);


	//���E�̎���\��
	for (int i = -5; i <= 5; i++) {
		Utility::DrawLine(CVector3D(-5, 0, i * 1.0f), CVector3D(5, 0, i * 1.0f), CVector4D(0.2, 0.2, 0.2, 1));
		Utility::DrawLine(CVector3D(i * 1.0f, 0, -5), CVector3D(i * 1.0f, 0, 5), CVector4D(0.2, 0.2, 0.2, 1));
	}
	Utility::DrawLine(CVector3D(0, 0, 0), CVector3D(100, 0, 0), CVector4D(1, 0, 0, 1));
	Utility::DrawLine(CVector3D(0, 0, 0), CVector3D(0, 100, 0), CVector4D(0, 1, 0, 1));
	Utility::DrawLine(CVector3D(0, 0, 0), CVector3D(0, 0, 100), CVector4D(0, 0, 1, 1));



	FONT_T()->Draw(0, 24, 1, 0, 0, "�E�N���b�N�F����ؑ�");
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
	CInput::SetButton(0, CInput::eButton1, '1');
	CInput::SetButton(0, CInput::eButton2, '2');
	CInput::SetButton(0, CInput::eButton3, '3');
	CInput::SetButton(0, CInput::eButton4, '4');
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
	CInput::ShowCursor(false);
	CInput::SetMouseInside(true);
	CInput::Update();
	CInput::Update();


	//���C�g�ݒ�
	CLight::SetType(0, CLight::eLight_Direction);
	CLight::SetPos(0, CVector3D(0, 200, 200));
	CLight::SetDir(0, CVector3D(-1, -2, 1).GetNormalize());
	CLight::SetColor(0, CVector3D(0.4f, 0.4f, 0.4f), CVector3D(0.6f, 0.6f, 0.6f));


	CLight::SetFogParam(CVector4D(1, 1, 1, 1), 700, 800);

	//�J����������
	CCamera::GetCamera()->LookAt(CVector3D(5, 5, 5),
		CVector3D(0, 0, 0),
		CVector3D(0.0, 1.0, 0.0));

	SetCurrentDirectory("data");


	CShader::GetInstance("StaticMesh");
	CShader::GetInstance("SkinMesh");
	CSound::GetInstance();


	//-----------------------------------------------------
	//�������̖��߂�����
	//�Q�[���N�����Ɉ�x�����Ă΂��
	//-----------------------------------------------------

	//���f���̓ǂݍ���
	ADD_RESOURCE("Antman", CModel::CreateModel("Charactor/antman/antman.a3m"));
	ADD_RESOURCE("Field", CModel::CreateModel("Field/Land/land.field"));
	ADD_RESOURCE("Scar", CModel::CreateModel("Wepon/Scar/Scar.obj"));
	ADD_RESOURCE("Sword", CModel::CreateModel("Wepon/Sword/Sword.obj"));


	//���f������
	model = COPY_RESOURCE("Antman", CModelA3M);
	gun_model = COPY_RESOURCE("Scar", CModelObj);
	sword_model = COPY_RESOURCE("Sword", CModelObj);



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
	CCamera::GetCurrent()->Perspective(DtoR(60.0), (float)w / (float)h, 1.0, 10000.0);
	
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
