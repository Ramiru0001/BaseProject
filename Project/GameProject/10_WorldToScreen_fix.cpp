#include <GLLibrary.h>
#define SCREEN_WIDTH 1920
#define SCREEN_HEIGHT 1080



//--------------------------------------------
//グローバル変数領域
//-------------------------------------------

//モーション有モデルクラス
CModelA3M model;
//座標
CVector3D pos(0, 0, 0);
//回転
CVector3D rot(0, 0, 0);



//ターゲット座標
CVector3D target(0, 0, 0);

//フィールドモデル
CModelField* field;


void MainLoop(void) {

	//--------------------------------------------------------------
	//ゲーム中の動きはここに書く
	//ゲーム中はこの関数_を1秒間に60回呼び出している
	//--------------------------------------------------------------
		//キャラクター移動
	//移動量
	const float move_speed = 0.05f;

	//キャラクターをターゲットへ向けて移動させる
	//ターゲットへのベクトル
	CVector3D vec = target - pos;
	//回転値を逆算
	rot.y = atan2(vec.x, vec.z);
	//距離が1以上なら
	if (vec.Length() >= 1.0f) {
		//移動方向
		CVector3D dir(sin(rot.y), 0, cos(rot.y));
		//移動
		pos += dir * move_speed;
		//1番（待機）のアニメーションを再生
		model.ChangeAnimation(1);
	}
	else {
		//0番（待機）のアニメーションを再生
		model.ChangeAnimation(0);
	}
	//アニメーションを更新
	model.UpdateAnimation();

	{//キャラクターの接地処理
		//接触点と法線
		CVector3D cross, normal;
		//フィールドと線分（キャラクターの頭→足元）との衝突判定
		if (field->CollisionRay(&cross, &normal, pos + CVector3D(0, 10, 0), pos + CVector3D(0, -10, 0))) {
			//高さのみ設定
			pos.y = cross.y;
		}
	}
	//カメラ設定
	CCamera::GetCamera()->LookAt(pos + CVector3D(10, 20, 10),
		pos,
		CVector3D(0.0, 1.0, 0.0));

	//キャラクターの座標、回転、スケール設定と表示
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

		//■マウスのクリックした場所を指す(スクリーン座標→ワールド座標)
		//マウスの座標（スクリーン座標）
		CVector2D m = CInput::GetMousePoint();
		//スクリーン座標→ワールド座標変換
		//ワールド座標：近
		CVector3D m_w_near = Utility::ScreenToWorld(CVector3D(m.x, m.y, 0));
		//ワールド座標：遠
		CVector3D m_w_far = Utility::ScreenToWorld(CVector3D(m.x, m.y, distance));
		//マウスのワールド座標(遠)を表示
		//Utility::DrawSphere(m_w_far, 1.0f, CVector4D(1, 0, 0, 1));
		

		//マウスのワールド座標：近からワールド座標：遠への線分と
		//フィールドモデルとの衝突
		//接触点と法線
		CVector3D cross, normal;
		//フィールドと線分との衝突判定
		if (field->CollisionRay(&cross, &normal, m_w_near, m_w_far)) {
			//接触点の表示
			Utility::DrawCube(cross, 1.0f, CVector4D(1, 1, 0, 1));
			//クリックでターゲット位置更新
			if (PUSH(CInput::eMouseL))
				target = cross;
		}
	}

	//ターゲットの表示
	Utility::DrawCube(target, 1.0f, CVector4D(0, 1, 0, 1));

	
	{//■キャラクターの座標に名前を表示する（ワールド座標→スクリーン座標）
		CVector3D pos2D;

		//ワールド座標→スクリーン座標
		pos2D = Utility::WorldToScreen(pos);

		//文字の表示
		FONT_T()->Draw(pos2D.x, pos2D.y, 1, 0, 0, "Player");

	}
	{//■ターゲットの座標に名前を表示する（ワールド座標→スクリーン座標）
		CVector3D pos2D;


		//ワールド座標→スクリーン座標
		pos2D = Utility::WorldToScreen(pos);
		//文字の表示
		FONT_T()->Draw(pos2D.x, pos2D.y, 1, 0, 0, "Target");

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

	
	ADD_RESOURCE("Antman", CModel::CreateModel("Charactor/antman/antman.a3m"));
	ADD_RESOURCE("Field", CModel::CreateModel("Field/Land/land.field"));
	ADD_RESOURCE("Marker", CImage::CreateImage("UI/marker.png"));

	model = COPY_RESOURCE("Antman", CModelA3M);
	//フィールドモデルは読み込みスレッドが動いているため複製しない
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

	//画面解像度変動
	CCamera::GetCamera()->SetSize((float)w, (float)h);
	//画面解像度固定
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
