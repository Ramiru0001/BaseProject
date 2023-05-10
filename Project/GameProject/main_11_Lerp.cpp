#include <GLLibrary.h>
#define SCREEN_WIDTH 1920
#define SCREEN_HEIGHT 1080



//--------------------------------------------
//�O���[�o���ϐ��̈�
//-------------------------------------------


//�L���[�u�̈ʒu
CVector3D cube_pos[2] = {
	CVector3D(10, 0, 0),
	CVector3D(0, 0, 10),
};



//�J�����̈ʒu
CVector3D cam_pos(0, 0, 0);
//�J�����̒����_
CVector3D cam_at(0, 0, 0);

//���_����_����
void LerpPoint() {
	//�_����_�ւ̕��
	static float t = 0;
	t = min(1.0f, t + 0.01f);
	CVector3D p = cube_pos[0] * (1 - t) + cube_pos[1] * t;
	if (PUSH(CInput::eButton10)) {
		t = 0;
	}
	CCamera::GetCurrent()->LookAt(CVector3D(20, 20, 20), CVector3D(0, 0, 0), CVector3D(0, 1, 0));

	Utility::DrawCube(p, 1.0f, CVector4D(1, 0, 0, 1));


}
//���J�����̐؂�ւ�����
void LerpCamera() {
	static int idx = 0;
	if (PUSH(CInput::eMouseL)) {
		idx = (idx + 1) % 2;
	}
	//�������
	CVector3D cam_at_target = cube_pos[idx];
	CVector3D cam_pos_target = cam_at_target + CVector3D(0, 15, -15);
	cam_pos = cam_pos * 0.96f + cam_pos_target * 0.04f;
	cam_at = cam_at * 0.96f + cam_at_target * 0.04f;
	CCamera::GetCurrent()->LookAt(cam_pos, cam_at, CVector3D(0, 1, 0));


}

CModelA3M model;
//���W
CVector3D pos(0, 0, 0);
//��]
CVector3D rot(0, 0, 0);
//��]�̖ڕW�l
CVector3D rot_target(0, 0, 0);


//���L�����N�^�[�̉�]����
void LerpEuler() {
	//���L�����N�^�[�̉�]���Ԃ���
	//�����L�[�̓��͕����x�N�g��
	CVector3D key_dir(0, 0, 0);
	//�J�����̕����x�N�g��
	CVector3D cam_dir = CCamera::GetCurrent()->GetDir();
	//���͉�]�l
	float key_ang = 0;
	//�J�����̉�]�l
	float cam_ang = atan2(cam_dir.x, cam_dir.z);
	//�L�����N�^�[�̈ړ���
	const float move_speed = 0.1f;

	//�����L�[������͕����x�N�g����ݒ�
	if (HOLD(CInput::eUp)) key_dir.z = 1;
	if (HOLD(CInput::eDown)) key_dir.z = -1;
	if (HOLD(CInput::eLeft)) key_dir.x = 1;
	if (HOLD(CInput::eRight)) key_dir.x = -1;

	//�L�[���͂������
	if (key_dir.LengthSq() > 0) {
		//���͕���������͉�]�l���v�Z
		key_ang = atan2(key_dir.x, key_dir.z);
		//�J�����̉�]�l�Ɠ��͉�]�l����L�����N�^�[�̉�]�l������
		//������rot.y�ɐݒ肹����U�ڕW�l��ݒ肷��
		rot_target.y = cam_ang + key_ang;

		//�ړ�����
		CVector3D dir(sin(rot.y), 0, cos(rot.y));
		pos += dir * move_speed;

		model.ChangeAnimation(1);
	}
	else {
		model.ChangeAnimation(0);
	}
	//���[�V�����̍X�V
	model.UpdateAnimation();
	//------------------------------------------------------
	//���p�x�̕��
	//�p�x���������߂�i�ڕW�l-���ݒl�j
	//�p�x������-��(-180)�`��(180)�͈̔͂ɐ�����
	float a = Utility::NormalizeAngle(rot_target.y - rot.y);
	if (a > DtoR(120)) {
		//���Ɍ����Ȃ�U�^�[�������̕����ǂ�
	}
	//�����ɂ����
	rot.y += a * 0.1f;
	
	//���ʉ�]�ɂ��p�x���
	float rot_speed = DtoR(5);
	if (a > rot_speed)
		rot.y += rot_speed;
	else if (a < -rot_speed)
		rot.y -= rot_speed;
	else 
		rot.y += a;	
	//��Ԍ��-��(-180)�`��(180)�͈̔͂Ɋp�x�𐮂���
	rot.y = Utility::NormalizeAngle(rot.y);
	//--------------------------------------------------------



}
CModelObj rl_model;
//�����`���[�ʒu
CVector3D rl_pos(-10, 2, 0);
//�����`���[��]
CVector3D rl_rot(0, 0, 0);
//�e���˗p�J�E���^�[
int rl_cnt = 0;


//�葬�̊p�x���
float LerpAngle1(float now, float target, float speed) {
	float a = Utility::NormalizeAngle(target - now);
	if (a > speed)
		now += speed;
	else if (a < -speed)
		now -= speed;
	else
		now += a;
	return Utility::NormalizeAngle(now);
}
//�����ɂ��p�x���
float LerpAngle2(float now, float target, float t) {
	float a = Utility::NormalizeAngle(target - now);
	now += a * t;
	return Utility::NormalizeAngle(now);
}
class Missile {
private:
	//�~�T�C�����f��
	CModelObj m_model;
	CVector3D m_pos;
	CVector3D m_rot;
	int m_cnt;
public:
	Missile(const CVector3D& pos, const CVector3D& rot);
	void Update();
	void Render();
};

Missile::Missile(const CVector3D& pos, const CVector3D& rot) {
	m_model = COPY_RESOURCE("Missile", CModelObj);
	m_pos = pos;
	m_rot = rot;
	m_cnt = 0;
}
void Missile::Update() {

	//����2�b��ǐ�
	if (m_cnt++ > 60) {
		CVector3D v = pos - m_pos;
		v.Normalize();
		CVector3D target = CVector3D(-asin(v.y), atan2(v.x, v.z), 0);

		//���ʉ�]�ɂ��p�x���
		m_rot.y = LerpAngle1(m_rot.y, target.y, DtoR(1));
		m_rot.x = LerpAngle1(m_rot.x, target.x, DtoR(1));
		//�����ɂ����
		//m_rot.y = LerpAngle2(m_rot.y, target.y, 0.1f);
		//m_rot.x = LerpAngle2(m_rot.x, target.x, 0.1f);
	}
	float move_speed = 0.05f;
	m_pos += CMatrix::MRotation(m_rot).GetFront() * move_speed;


}
void Missile::Render() {
	m_model.SetPos(m_pos);
	m_model.SetRot(m_rot);
	m_model.SetScale(0.2f, 0.2f, 0.2f);
	m_model.Render();
}

std::list<Missile*> list;

void Luncher() {
	CVector3D v = pos - rl_pos;
	v.Normalize();
	CVector3D target = CVector3D(-asin(v.y),atan2(v.x, v.z),0);

	//���ʉ�]�ɂ��p�x���
	rl_rot.y = LerpAngle1(rl_rot.y, target.y, DtoR(1));
	rl_rot.x = LerpAngle1(rl_rot.x, target.x, DtoR(1));


	if (rl_cnt++ % 120 == 0) {
		list.push_back(new Missile(rl_pos, rl_rot));
	}
	//--
	rl_model.SetPos(rl_pos);
	rl_model.SetRot(rl_rot);
	rl_model.SetScale(2.0f, 2.0f, 2.0f);
	rl_model.Render();


}



void ControlCamera();

void MainLoop(void) {

	//--------------------------------------------------------------
	//�Q�[�����̓����͂����ɏ���
	//�Q�[�����͂��̊֐�_��1�b�Ԃ�60��Ăяo���Ă���
	//--------------------------------------------------------------
	static int mode = 0;
	if (PUSH(CInput::eButton1)) mode = 0;
	if (PUSH(CInput::eButton2)) mode = 1;
	if (PUSH(CInput::eButton3)) mode = 2;
	if (PUSH(CInput::eButton4)) mode = 3;
	switch (mode) {
	case 0:
		//�_����_�ւ̕��
		LerpPoint();
		break;
	case 1:
		//�J�����؂�ւ��̕��
		LerpCamera();
		break;
	case 2:
		//�L�����N�^�[��]�̕��
		LerpEuler();
		ControlCamera();
		break;
	case 3:
		//���P�b�g�����`���[�̒葬����ƃ~�T�C���̋��`���
		Luncher();
		LerpEuler();
		ControlCamera();
		break;
	}

	//�~�T�C���\��
	for (auto it : list) {
		it->Update();
		it->Render();
	}

	//�L���[�u�\��
	for (int i = 0; i < 2; i++)
		Utility::DrawCube(cube_pos[i], 1.0f, CVector4D(0, 1, 0, 1));


	//�L�����N�^�[�\��
	model.SetPos(pos);
	model.SetRot(rot);
	model.SetScale(0.01f, 0.01f, 0.01f);
	model.Render();

	//���E�̎���\��
	Utility::DrawLine(CVector3D(0, 0, 0), CVector3D(100, 0, 0), CVector4D(1, 0, 0, 1));
	Utility::DrawLine(CVector3D(0, 0, 0), CVector3D(0, 100, 0), CVector4D(0, 1, 0, 1));
	Utility::DrawLine(CVector3D(0, 0, 0), CVector3D(0, 0, 100), CVector4D(0, 0, 1, 1));

	for (int i = -5; i <= 5; i++) {
		Utility::DrawLine(CVector3D(-5, 0, i * 1.0f), CVector3D(5, 0, i * 1.0f), CVector4D(0.2, 0.2, 0.2, 1));
		Utility::DrawLine(CVector3D(i * 1.0f, 0, -5), CVector3D(i * 1.0f, 0, 5), CVector4D(0.2, 0.2, 0.2, 1));
	}
	static const char* strings[] = {
		"�_����_�ւ̕��",
		"�J�����؂�ւ��̕�ԁ@�N���b�N�ŃJ�����؂�ւ�",
		"�L�����N�^�[��]��ԁ@WASD�L�[�ňړ�",
		"���P�b�g�����`���[�ƃ~�T�C���̕��"
	};
	FONT_T()->Draw(0, 48, 0, 0, 0, strings[mode]);


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

	//���f���̓ǂݍ���
	ADD_RESOURCE("Antman", CModel::CreateModel("Charactor/antman/antman.a3m"));
	ADD_RESOURCE("Missile", CModel::CreateModel("Wepon/Missile/Missile.obj"));
	ADD_RESOURCE("RocketLauncher", CModel::CreateModel("Wepon/RocketLauncher/rocket_launcher_gun.obj"));

	//���f������
	model = COPY_RESOURCE("Antman", CModelA3M);

	rl_model = COPY_RESOURCE("RocketLauncher", CModelObj);


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

void ControlCamera() {
	static CVector3D at(0, 0, 0);
	//�J������]�l
	static CVector3D cam_rot(DtoR(45), DtoR(-125), 0);
	//�����_�܂ł̋���
	static float cam_dist = 20.0f;
	//�Ǐ]�J����
	//��]���x�W��
	float cam_rot_speed = 0.01f;
	float cam_move_speed = 0.01f;
	//�}�E�X�̈ړ���
	CVector2D mouse_vec = CInput::GetMouseVec();
	if (HOLD(CInput::eMouseR)) {
		//�}�E�X�ړ�����J��������]
		//�}�E�X�ړ�Y��X����](�㉺)
		//�}�E�X�ړ�X��Y����](���E)
		cam_rot += CVector3D(mouse_vec.y, -mouse_vec.x, 0) * cam_rot_speed;
	}
	//�㉺�p����
	cam_rot.x = min(DtoR(60), max(DtoR(-35), cam_rot.x));
	//-180�`180�ɐ��K��
	cam_rot.y = Utility::NormalizeAngle(cam_rot.y);
	//��������
	cam_dist = min(30.0f, max(2.0f, cam_dist - CInput::GetMouseWheel()));

	if (HOLD(CInput::eMouseC)) {
		at += CMatrix::MRotation(cam_rot) * CVector3D(mouse_vec.x, mouse_vec.y, 0) * cam_move_speed;
	}

	//�ʒu�ƒ����_��ݒ�
	CVector3D cam_at = at;
	CVector3D cam_pos = cam_at + CMatrix::MRotation(cam_rot).GetFront() * -cam_dist;
	CCamera::GetCurrent()->LookAt(cam_pos, cam_at, CVector3D(0, 1, 0));
}
