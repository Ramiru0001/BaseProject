//--------------------------------------------
//グローバル変数領域
//-------------------------------------------


//モーション無しクラス
CModelObj model;
//座標
CVector3D pos(0, 0, 0);
//回転値
CVector3D rot(0, 0, 0);
CVector3D rot_default(0, 0, 0);
//スケール
CVector3D scale(1, 1, 1);
CVector3D scale_default(0.1f, 0.1f, 0.1f);

float alpha = 1.0f;

void ControlCamera(const CVector3D& at){
	//カメラ回転値
	static CVector3D cam_rot(DtoR(30), DtoR(-180), 0);
	//注視点までの距離
	static float cam_dist = 10.0f;
	//追従カメラ
	//回転速度係数
	float cam_speed = 0.01f;
	if (HOLD(CInput::eMouseR)) {
		//マウスの移動量
		CVector2D mouse_vec = CInput::GetMouseVec();
		//マウス移動からカメラを回転
		//マウス移動Y→X軸回転(上下)
		//マウス移動X→Y軸回転(左右)
		cam_rot += CVector3D(mouse_vec.y, -mouse_vec.x, 0) * cam_speed;
	}
	//上下角制限
	cam_rot.x = min(DtoR(60), max(DtoR(-35), cam_rot.x));
	//-180～180に正規化
	cam_rot.y = Utility::NormalizeAngle(cam_rot.y);
	//距離制限
	cam_dist = min(10.0f, max(2.0f, cam_dist + CInput::GetMouseWheel()));


	//位置と注視点を設定
	CVector3D cam_at = at;
	CVector3D cam_pos = cam_at + CMatrix::MRotation(cam_rot).GetFront() * -cam_dist;
	CCamera::GetCurrent()->LookAt(cam_pos, cam_at, CVector3D(0, 1, 0));
}

void RenderSkybox();

void MainLoop(void) {

	//--------------------------------------------------------------
	//ゲーム中の動きはここに書く
	//ゲーム中はこの関数_を1秒間に60回呼び出している
	//--------------------------------------------------------------
	{
		//スケール
		CVector3D scale_vec(0.1f, 0.1f, 0.1f);
		scale += scale_vec;
	}
	/*{
		//回転
		CVector3D rot_vec(0.0f, 0.0f, DtoR(1));
		rot += rot_vec;
	}*/
	/*{
		//移動
		CVector3D move_vec(0.0f, 0.1f, 0.0f);
		pos += move_vec;
	}*/
	 {
		//透明化
		float alpha_vec = -0.01f;
		alpha = max(0.0f, alpha + alpha_vec);
	}
	//リセット
	if (PUSH(CInput::eMouseL)) {
		scale = scale_default;
		pos = CVector3D(0, 0, 0);
		rot = rot_default;
		alpha = 1.0f;
	}

	ControlCamera(CVector3D(0, 1.5f, 0));
	//スカイボックス
	RenderSkybox();
	//キャラクターの座標、回転、スケール設定と表示
	//座標
	model.SetPos(pos);
	//回転値
	model.SetRot(rot);
	//スケールの変更
	model.SetScale(scale);
	//マテリアル（アルファ値）の設定
	model.GetMaterial(0)->m_alpha = alpha;
	//加算ブレンド
	//glBlendFunc(GL_SRC_ALPHA, GL_ONE);
	//深度書き込みOFF
	glDepthMask(GL_FALSE);
	//ライティングOFF
	CLight::SetLighting(false);
	//モデルの表示
	//model.Render();
	model.RenderBuilbord();
	//ライティングON
	CLight::SetLighting(true);
	//深度書き込みON
	//glDepthMask(GL_TRUE);
	//通常ブレンドモードに
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);




	//世界の軸を表示
	Utility::DrawLine(CVector3D(0, 0, 0), CVector3D(100, 0, 0), CVector4D(1, 0, 0, 1));
	Utility::DrawLine(CVector3D(0, 0, 0), CVector3D(0, 100, 0), CVector4D(0, 1, 0, 1));
	Utility::DrawLine(CVector3D(0, 0, 0), CVector3D(0, 0, 100), CVector4D(0, 0, 1, 1));

	for (int i = -5; i <= 5; i++) {
		Utility::DrawLine(CVector3D(-5, 0, i * 1.0f), CVector3D(5, 0, i * 1.0f), CVector4D(0.2, 0.2, 0.2, 1));
		Utility::DrawLine(CVector3D(i * 1.0f, 0, -5), CVector3D(i * 1.0f, 0, 5), CVector4D(0.2, 0.2, 0.2, 1));
	}
//	FONT_T()->Draw(0, 64, 1, 0, 0, "C[%.0f] K[%.0f] P[%.0f]", RtoD(cam_rot.y), RtoD(key_ang),RtoD(rot.y));
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

	SetCurrentDirectory("data");


	//-----------------------------------------------------
	//初期化の命令を書く
	//ゲーム起動時に一度だけ呼ばれる
	//-----------------------------------------------------


	ADD_RESOURCE("Effect", CModel::CreateModel("Effect/Bomb.obj"));
	model = COPY_RESOURCE("Effect", CModelObj);
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

void RenderSkybox() {
	static CModel* skybox = nullptr;
	if (!skybox) {
		ADD_RESOURCE("Sky", CModel::CreateModel("Field/Sky/Sky.obj"));
		skybox = GET_RESOURCE("Sky", CModel);
		//メインカメラの設定を複製
		*CCamera::GetCamera(CCamera::eSkyBox) = *CCamera::GetCamera(CCamera::eMainCamera);
		for (int i = 0; i < skybox->GetMaterialSize(); i++) {
			if (CTexture* t = skybox->GetMaterial(i)->mp_texture) {
				t->SetWrapST(GL_CLAMP_TO_EDGE);
				t->SetFilter(GL_LINEAR);
			}
		}
	}
	//デプステストOFF
	//ここでの描画は深度バッファへ影響しない
	glDisable(GL_DEPTH_TEST);
	//現在使用中のカメラを取得
	CCamera* back = CCamera::GetCurrent();
	//現在のカメラのビュー行列を取得
	CMatrix matrix = back->GetViewMatrix();
	//スカボックス用カメラを取得
	CCamera* c = CCamera::GetCamera(CCamera::eSkyBox);
	//ビュー行列の座標を無効に
	matrix.m03 = 0; matrix.m13 = 0; matrix.m23 = 0;
	//スカイボックス用のビュー行列設定
	c->SetViewMatrix(matrix);
	//スカイボックス用の投影行列設定(現在のカメラと同じ値にする)
	c->SetProjectionMatrix(back->GetProjectionMatrix());
	//使用するカメラをスカイボックス用に設定
	CCamera::SetCurrent(c);
	//ライティングOFF
	CLight::SetLighting(false);


	//スカボックス描画
	skybox->Render();


	//元の設定に戻す
	CLight::SetLighting(true);
	CCamera::SetCurrent(back);
	glEnable(GL_DEPTH_TEST);

}
