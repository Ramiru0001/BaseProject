//--------------------------------------------
//グローバル変数領域
//-------------------------------------------
//モーション有モデルクラス
CModelA3M model;
//座標
CVector3D pos(0, 0, 0);
//回転
CVector3D rot(0, 0, 0);
//当たり判定用球の半径
float rad = 0.4f;
//カメラ回転
CVector3D cam_rot(0, 0, 0);
//プレイヤーの球の中心
CVector3D center(0, 0, 0);
//プレイヤーのカプセル
CVector3D lineS,lineE;


//球の座標
CVector3D s_pos(0,1.0f,5.0f);
//球の半径
float s_rad=1.0f;

//カプセルの視点、終点
CVector3D c_s(2.0f, 1.0f, 5.0f), c_e(2.0f, 2.0f, 5.0f);
//カプセルの半径
float c_rad=1.0f;

void MainLoop(void) {

	//--------------------------------------------------------------
	//ゲーム中の動きはここに書く
	//ゲーム中はこの関数_を1秒間に60回呼び出している
	//--------------------------------------------------------------



	//---------------------------------------
	// プレイヤー
	//---------------------------------------
	//キャラクターの移動量
	const float move_speed = 0.1f;
	float cam_ang = cam_rot.y;


	//方向キーのベクトル
	CVector3D key_dir(0, 0, 0);



	//方向キーから入力方向ベクトルを設定
	if (HOLD(CInput::eUp)) key_dir.z = 1;
	if (HOLD(CInput::eDown)) key_dir.z = -1;
	if (HOLD(CInput::eLeft)) key_dir.x = 1;
	if (HOLD(CInput::eRight)) key_dir.x = -1;

	//入力があれば
	if (key_dir.LengthSq() > 0) {
		//キーの方向ベクトルを角度に逆算
		float key_ang = atan2(key_dir.x, key_dir.z);
		//カメラの角度＋キーの角度でキャラクターの角度が決まる
		rot.y = Utility::NormalizeAngle(key_ang + cam_ang);
		//方向ベクトル
		CVector3D dir(sin(rot.y), 0, cos(rot.y));
		//移動
		pos += dir * move_speed;
		model.ChangeAnimation(18);
	}
	else {
		model.ChangeAnimation(17);
	}

	//モーションの更新
	model.UpdateAnimation();


	//■プレイヤーの球
	center = pos + CVector3D(0, rad, 0);

	//■プレイヤーカプセル
	//カプセル高さ
	float height = 1.8;
	//カプセル始点
	lineS = pos + CVector3D(0, height - rad, 0);
	//カプセル終点
	lineE = pos + CVector3D(0, rad, 0);



	//-------------------------------------------
	//■球、カプセルの当たり判定
	//------------------------------------------
	
	{
		//■プレイヤー球とテスト球の衝突
		//プレイヤー球からテスト球への方向
		CVector3D dir;
		//球同士の距離
		float dist;
		//接触判定
		if (CCollision::CollisionShpere(center, rad, s_pos, s_rad, &dist, &dir)) {
			//めり込み量
			float s = (rad + s_rad) - dist;
			//テスト球を押し戻し
			s_pos += dir * s;
		}
	}

	{
		//■プレイヤーのカプセルとテストカプセルの接触
		//カプセル上の最近点、カプセルからもう一つのカプセルの方向
		CVector3D c1, dir1, c2, dir2;
		//カプセル同士の距離
		float dist;
		//カプセル同士の衝突
		if (CCollision::CollisionCapsule(lineS, lineE, rad, c_s, c_e,c_rad, &dist, &c1, &dir1, &c2, &dir2)) {
			//めり込み量
			float s=(rad + c_rad) - dist;
			//プレイヤー→テストカプセル方向へめり込んだ分押し戻す
			//c_s += dir1 * s;
			//c_e += dir1 * s;
			pos += -dir1 * s;
		}
	}
	
	{
		//■プレイヤーのカプセルとテスト球の接触
		//カプセル上の球との最近点、カプセルから球への方向
		CVector3D cross, dir;
		//カプセルと球との距離
		float dist;
		//カプセルと球との衝突
		if (CCollision::CollisionCapsuleShpere(lineS, lineE, rad, s_pos, s_rad, &dist, &cross, &dir)) {
			//めり込み量
			float s = (rad + s_rad) - dist;
			//カプセル→球の方向へめり込んだ分押し戻す
			s_pos += dir * s;
		}

	}



	//---------------------------------------
	//カメラ
	//---------------------------------------
	//注視点までの距離
	static float cam_dist = 3.0f;
	//回転速度係数
	float cam_speed = 0.01f;
	//マウスの移動量
	CVector2D mouse_vec = CInput::GetMouseVec();
	//マウス移動からカメラを回転
	//マウス移動Y→X軸回転(上下)
	//マウス移動X→Y軸回転(左右)
	cam_rot += CVector3D(mouse_vec.y, -mouse_vec.x, 0) * cam_speed;
	//上下角制限
	cam_rot.x = min(DtoR(60), max(DtoR(-35), cam_rot.x));
	//-180～180に正規化
	cam_rot.y = Utility::NormalizeAngle(cam_rot.y);
	//距離制限
	cam_dist = min(10.0f, max(2.0f, cam_dist - CInput::GetMouseWheel()));


	//位置と注視点を設定
	CVector3D cam_at = pos + CVector3D(0, 1.5, 0);
	CVector3D cam_pos = cam_at + CMatrix::MRotation(cam_rot).GetFront() * -cam_dist;

	CCamera::GetCurrent()->LookAt(cam_pos, cam_at, CVector3D(0, 1, 0));


	//----------------------------------------


	//キャラクター表示
	model.SetPos(pos);
	model.SetRot(rot);
	model.SetScale(0.01f, 0.01f, 0.01f);
	model.Render();

	//プレイヤーカプセルの表示
	Utility::DrawCapsule(lineS, lineE, rad, CVector4D(1, 0, 0, 1));
	//プレイヤー球の表示
	//Utility::DrawSphere(center,rad,CVector4D(1,1,0, 1));


	//テスト球の表示
	Utility::DrawSphere(s_pos, s_rad, CVector4D(0, 1, 0, 1));
	//テストカプセルの表示
	Utility::DrawCapsule(c_s, c_e, c_rad, CVector4D(0, 0, 1, 1));


	//世界の軸を表示
	Utility::DrawLine(CVector3D(0, 0, 0), CVector3D(100, 0, 0), CVector4D(1, 0, 0, 1));
	Utility::DrawLine(CVector3D(0, 0, 0), CVector3D(0, 100, 0), CVector4D(0, 1, 0, 1));
	Utility::DrawLine(CVector3D(0, 0, 0), CVector3D(0, 0, 100), CVector4D(0, 0, 1, 1));

	for (int i = -5; i <= 5; i++) {
		Utility::DrawLine(CVector3D(-5, 0, i * 1.0f), CVector3D(5, 0, i * 1.0f), CVector4D(0.2, 0.2, 0.2, 1));
		Utility::DrawLine(CVector3D(i * 1.0f, 0, -5), CVector3D(i * 1.0f, 0, 5), CVector4D(0.2, 0.2, 0.2, 1));
	}



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

	//マウス非表示
	CInput::ShowCursor(false);
	CInput::SetMouseInside(true);
	CInput::Update();
	CInput::Update();


	//ライト設定
	CLight::SetType(0, CLight::eLight_Direction);
	CLight::SetPos(0, CVector3D(0, 200, 200));
	CLight::SetDir(0, CVector3D(-1, -2, 1).GetNormalize());
	CLight::SetColor(0, CVector3D(0.2f, 0.2f, 0.2f), CVector3D(0.8f, 0.8f, 0.8f));


	CLight::SetFogParam(CVector4D(1, 1, 1, 1), 1200, 1400);

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

	//モデルの読み込み
	ADD_RESOURCE("Antman", CModel::CreateModel("Charactor/antman/antman.a3m"));


	//モデル複製
	model = COPY_RESOURCE("Antman", CModelA3M);



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
