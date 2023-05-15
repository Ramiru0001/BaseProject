//--------------------------------------------
//グローバル変数領域
//-------------------------------------------
enum {
	eBottom,	//車両部分
	eTop,		//砲塔部分
	eCannon,	//主砲部分
	ePartsMax,
};
//スタティックメッシュモデル
CModelObj tank_model[ePartsMax];

//戦車の各パーツの行列
CMatrix tank_matrix[ePartsMax];

//移動量
CVector3D move_vec(0, 0, 0);
//回転量
CVector3D rot_vec(0, 0, 0);

//戦車の各パーツの座標
CVector3D tank_pos[ePartsMax] = {
	CVector3D(0, 0, 0),
	CVector3D(0, 2.95f, 0),
	CVector3D(0, 0.66f, 1.97f)
};
//戦車の各パーツの回転値
CVector3D tank_rot[ePartsMax] = {
	CVector3D(0, 0, 0),
	CVector3D(0, 0, 0),
	CVector3D(0, 0, 0)
};


void MainLoop(void) {

	//--------------------------------------------------------------
	//ゲーム中の動きはここに書く
	//ゲーム中はこの関数_を1秒間に60回呼び出している
	//--------------------------------------------------------------

	//回転加速度
	const float rot_accel = 0.1f;
	//移動量
	const float move_accel = 0.02f;

	//回転速度
	const float rot_speed = 0.001f;

	//砲塔、主砲の回転
	CVector2D mouse_vec = CInput::GetMouseVec();
	tank_rot[eTop].y += mouse_vec.x * -rot_speed;
	tank_rot[eCannon].x += mouse_vec.y * rot_speed;
	tank_rot[eCannon].x = min(DtoR(0), max(DtoR(-30.0f), tank_rot[eCannon].x));


	//回転減速処理
	rot_vec *= 0.9f;
	//移動減速処理
	move_vec *= 0.9f;
	//回転処理	
	tank_rot[eBottom] += rot_vec;
	//移動処理
	tank_pos[eBottom] += move_vec;

	if (HOLD(CInput::eDown)) {
		//車両の回転
		if (HOLD(CInput::eLeft))
			rot_vec.y -= DtoR(rot_accel);
		if (HOLD(CInput::eRight))
			rot_vec.y += DtoR(rot_accel);
		move_vec -= tank_matrix[eBottom].GetFront() * move_accel;
	}
	else {
		//車両の回転
		if (HOLD(CInput::eLeft))
			rot_vec.y += DtoR(rot_accel);
		if (HOLD(CInput::eRight))
			rot_vec.y -= DtoR(rot_accel);
		//移動処理
		if (HOLD(CInput::eUp)) {
			move_vec += tank_matrix[eBottom].GetFront() * move_accel;
		}
	}


	//-------------------------------------------------
	//戦車の行列を設定する
	//★階層構造を構築する　[親×子](各パーツの行列は平行移動×回転の順で作る)
	tank_matrix[eBottom] =
		CMatrix::MTranselate(tank_pos[eBottom]) *
		CMatrix::MRotation(tank_rot[eBottom]);

	tank_matrix[eTop] = tank_matrix[eBottom] *
		CMatrix::MTranselate(tank_pos[eTop]) *
		CMatrix::MRotation(tank_rot[eTop]);

	tank_matrix[eCannon] = tank_matrix[eTop] *
		CMatrix::MTranselate(tank_pos[eCannon]) *
		CMatrix::MRotation(tank_rot[eCannon]);


	//-----------------------------------------------------
	//カメラの行列
	CMatrix matrix_camera = tank_matrix[eTop] *
		CMatrix::MTranselate(0, 3, -10);
	//モデル行列でカメラを設定（引数の行列を元に、内部でビュー行列に変換する）
	CCamera::GetCurrent()->SetTranseRot(matrix_camera);
	//-----------------------------------------------------


	//行列を指定して描画
	for (int i = 0; i < ePartsMax; i++) {
		tank_model[i].Render(tank_matrix[i]);
	}

	//世界の軸を表示
	for (int i = -5; i <= 5; i++) {
		Utility::DrawLine(CVector3D(-5, 0, i * 1.0f), CVector3D(5, 0, i * 1.0f), CVector4D(0.2, 0.2, 0.2, 1));
		Utility::DrawLine(CVector3D(i * 1.0f, 0, -5), CVector3D(i * 1.0f, 0, 5), CVector4D(0.2, 0.2, 0.2, 1));
	}
	Utility::DrawLine(CVector3D(0, 0, 0), CVector3D(100, 0, 0), CVector4D(1, 0, 0, 1));
	Utility::DrawLine(CVector3D(0, 0, 0), CVector3D(0, 100, 0), CVector4D(0, 1, 0, 1));
	Utility::DrawLine(CVector3D(0, 0, 0), CVector3D(0, 0, 100), CVector4D(0, 0, 1, 1));


}

void Init(void)
{
	glClearColor(1.0, 1.0, 1.0, 1.0);

	glEnable(GL_DEPTH_TEST);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
	glEnable(GL_BLEND);//ブレンドの有効化
	glEnable(GL_CULL_FACE);
	glCullFace(GL_BACK);

	//固定シェーダー用
	glEnable(GL_LIGHTING);
	glEnable(GL_LIGHT0);

	glEnable(GL_ALPHA_TEST);

	CFPS::SetFPS(60);
	//フレーム制御初期化
	CFPS::Init();
	//ボタンの設定
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

	//マウス非表示
	//	CInput::ShowCursor(false);
	//	CInput::SetMouseInside(true);
	CInput::Update();
	CInput::Update();


	//ライト設定
	CLight::SetType(0, CLight::eLight_Direction);
	CLight::SetPos(0, CVector3D(0, 200, 200));
	CLight::SetDir(0, CVector3D(-1, -2, 1).GetNormalize());
	CLight::SetColor(0, CVector3D(0.2f, 0.2f, 0.2f), CVector3D(0.8f, 0.8f, 0.8f));


	CLight::SetFogParam(CVector4D(1, 1, 1, 1), 700, 800);

	//カメラ初期化
	CCamera::GetCamera()->LookAt(CVector3D(5, 5, 5),
		CVector3D(0, 0, 0),
		CVector3D(0.0, 1.0, 0.0));

	SetCurrentDirectory("data");


	CShader::GetInstance("StaticMesh");
	CShader::GetInstance("SkinMesh");
	CSound::GetInstance();

	//-----------------------------------------------------
	//初期化の命令を書く
	//ゲーム起動時に一度だけ呼ばれる
	//-----------------------------------------------------


	ADD_RESOURCE("TankBottom", CModel::CreateModel("Charactor/Tank/Bottom.obj"));
	ADD_RESOURCE("TankTop", CModel::CreateModel("Charactor/Tank/Top.obj"));
	ADD_RESOURCE("TankGun", CModel::CreateModel("Charactor/Tank/Gun.obj"));

	tank_model[eBottom] = COPY_RESOURCE("TankBottom", CModelObj);
	tank_model[eTop] = COPY_RESOURCE("TankTop", CModelObj);
	tank_model[eCannon] = COPY_RESOURCE("TankGun", CModelObj);



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

	//画面解像度変動
	CCamera::GetCamera()->SetSize((float)w, (float)h);
	//画面解像度固定
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

//フルスクリーン?ウインドウモードの切り替え
//Alt+Enterで切り替える
void CheckFullScreen() {
	static int key_enter = 0;
	int key_enter_buf = key_enter;
	if (key_enter_buf ^ (key_enter = glfwGetKey(GL::window, GLFW_KEY_ENTER)) && key_enter && (glfwGetKey(GL::window, GLFW_KEY_LEFT_ALT) || glfwGetKey(GL::window, GLFW_KEY_RIGHT_ALT))) {
		GL::ChangeFullScreen(!GL::full_screen);
	}
}

int __main(int* argcp, char** argv) {
	// メモリリーク検出
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
		//各バッファーをクリア
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
