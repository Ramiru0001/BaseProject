
class Effect {
public:
	CVector3D	m_pos;			//座標
	CVector3D	m_vec;			//移動量
	CVector3D	m_rot;			//回転値
	float		m_time;			//生存時間

	bool	m_kill;
public:
	/// <summary>
	/// コンストラクタ
	/// </summary>
	Effect(float time):m_pos(0,0,0),m_vec(0,0,0),m_rot(0,0,0),m_time(time),m_kill(false) {

	}
	/// <summary>
	/// デストラクタ
	/// </summary>
	virtual ~Effect() {

	}
	/// <summary>
	/// 削除
	/// </summary>
	void SetKill() { m_kill = true; }
	/// <summary>
	/// 更新
	/// </summary>
	virtual void Update() {}
	/// <summary>
	/// 描画
	/// </summary>
	virtual void Render() {}

};

class ParticleEffect :public Effect {
private:
	CModelObj	m_model;		//モデル
	CVector3D	m_vec_accel;	//移動量増加
	CVector3D	m_rot_vec;		//回転量
	int			m_blend;		//ブレンド種類
	bool		m_builbord;		//ビルボード描画設定
	float		m_scale;		//スケール
	float		m_scale_speed;	//スケール変化量
	float		m_alpha;		//アルファ値
	float		m_alpha_speed;	//アルファ値変化量
	int			m_time;			//表示時間
public:
	enum {
		eBlendAlpha,		//通常ブレンド
		eBlendAdd,			//加算ブレンド
	};

	/// <summary>
	/// コンストラクタ
	/// </summary>
	/// <param name="model">使用モデル</param>
	/// <param name="pos">出現位置</param>
	/// <param name="vec">初期移動ベクトル</param>
	/// <param name="accel">加速ベクトル</param>
	/// <param name="scale">初期大きさ</param>
	/// <param name="change_scale">大きさ変化量</param>
	/// <param name="alpha">不透明度変化量</param>
	/// <param name="change_alpha">不透明度変化量</param>
	/// <param name="blend">ブレンド方法</param>
	/// <param name="builbord">ビルボード描画</param>
	/// <param name="time">生存時間</param>
	ParticleEffect(const char* model, const CVector3D& pos, const CVector3D& vec, const CVector3D& accel,
		float scale, float change_scale, float alpha, float change_alpha, int blend, bool builbord, int time = -1)
		:Effect(time),
		m_alpha(1.0f),
		m_vec_accel(accel),
		m_scale(scale),
		m_scale_speed(change_scale),
		m_alpha_speed(change_alpha),
		m_blend(blend),
		m_builbord(builbord),
		m_time(time) {
		m_pos = pos;
		m_vec = vec;
		m_model = COPY_RESOURCE(model, CModelObj);
	}
	/// <summary>
	/// デストラクタ
	/// </summary>
	~ParticleEffect() {

	}
	/// <summary>
	/// 更新
	/// </summary>
	void Update() {
		m_vec += m_vec_accel;
		m_pos += m_vec;
		m_rot += m_rot_vec;
		m_scale += m_scale_speed;
		m_alpha += m_alpha_speed;
		if (m_time > 0) m_time--;
		if (m_alpha < 0 || m_scale < 0 || m_time == 0) SetKill();
	}
	/// <summary>
	/// 描画
	/// </summary>
	void Render() {
		CLight::SetLighting(false);
		//深度バッファへの書き込みOFF
		glDepthMask(GL_FALSE);
		switch (m_blend) {
		case eBlendAlpha:
			//通常ブレンドモードに
			glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
			break;
		case eBlendAdd:
			//加算ブレンドモードに
			glBlendFunc(GL_SRC_ALPHA, GL_ONE);
		}
		//座標設定
		m_model.SetPos(m_pos);
		//マテリアルのアルファ値設定
		m_model.GetMaterial(0)->m_alpha = m_alpha;
		//スケール設定
		m_model.SetScale(m_scale, m_scale, m_scale);
		//回転値設定
		m_model.SetRot(m_rot);
		if (m_builbord)
			//ビルボード描画
			m_model.RenderBuilbord(CCamera::GetCurrent()->GetBuilbordMatrix());
		else
			//通常描画
			m_model.Render();

		glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
		glDepthMask(GL_TRUE);
		CLight::SetLighting(true);
	}
};
class MagicEffect : public Effect {
	CModelObj	m_model;		//モデル
	float		m_scale;		//大きさ
	float		m_alpha;		//不透明度
public:
	
	/// <summary>
	/// コンストラクタ
	/// </summary>
	/// <param name="model">使用モデル</param>
	/// <param name="pos">出現座標</param>
	/// <param name="rot">初期回転値</param>
	/// <param name="scale">大きさ</param>
	/// <param name="time">生存時間</param>
	MagicEffect(const char* model, const CVector3D& pos, const CVector3D& rot, float scale, float time) :Effect(time) {
			m_pos = pos; 
			m_rot = rot;
			m_scale = scale;
			m_alpha = 1.0f;
			m_model = COPY_RESOURCE(model, CModelObj);
			m_rot = rot;
	}
	/// <summary>
	/// デストラクタ
	/// </summary>
	~MagicEffect(){
		}
	/// <summary>
	/// 更新
	/// </summary>
	void Update(){
		m_time--;
		//残り30Fから徐々に消える
		if (m_time < 30) {
			m_alpha -= 1.0f / 30;
		}
		//削除
		if (m_time <= 0) {
			SetKill();
		}
		//回転
		m_rot.y += DtoR(1.0f);
	}
	
	/// <summary>
	/// 描画
	/// </summary>
	void Render() {
		CLight::SetLighting(false);
		glDepthMask(GL_FALSE);
		m_model.SetRot(m_rot);
		m_model.SetPos(m_pos);
		m_model.SetScale(m_scale, m_scale, m_scale);
		m_model.GetMaterial(0)->m_alpha = m_alpha;
		m_model.Render();
		glDepthMask(GL_TRUE);
		CLight::SetLighting(true);
	}
};
class UVEffect : public Effect {
	CModelObj	m_model;		//モデル
	float		m_scale;		//大きさ
	float		m_alpha;		//不透明度
	CVector2D	m_st;			//UVスライド
	CVector2D	m_st_vec;		//UVスライド量
public:
	
	/// <summary>
	/// コンストラクタ
	/// </summary>
	/// <param name="model">モデル</param>
	/// <param name="pos">出現位置</param>
	/// <param name="rot">初期回転値</param>
	/// <param name="scale">大きさ</param>
	/// <param name="vec">UV移動量</param>
	/// <param name="time">生存時間</param>
	UVEffect(const char* model, const CVector3D& pos, const CVector3D& rot, float scale, const CVector2D& vec, float time)
		:Effect(time), m_st(0, 0), m_st_vec(vec) {
			m_pos = pos;
			m_scale = scale;
			m_rot = rot;
			m_alpha = 1.0f;
			m_model = COPY_RESOURCE(model, CModelObj);
	}
	/// <summary>
	/// デストラクタ
	/// </summary>
	~UVEffect() {

	}
	/// <summary>
	/// 更新
	/// </summary>
	void Update(){
		m_time--;
		//UVスライド
		m_st += m_st_vec;
		//削除
		if (m_time <= 0) {
			SetKill();
		}
	}
	/// <summary>
	/// 描画
	/// </summary>
	void Render(){
		CLight::SetLighting(false);
		glDepthMask(GL_FALSE);
		m_model.SetRot(m_rot);
		m_model.SetPos(m_pos);
		m_model.SetScale(m_scale, m_scale, m_scale);
		m_model.GetMaterial(0)->m_alpha = m_alpha;
		//テクスチャーのUVスライド値設定
		m_model.GetMaterial(0)->m_st = m_st;
		m_model.Render();
		glDepthMask(GL_TRUE);
		CLight::SetLighting(true);
	}
};


//--------------------------------------------
//グローバル変数領域
//-------------------------------------------

//エフェクトリスト
std::list<Effect*> m_list;

void ControlCamera();
void RenderSkybox();

void MainLoop(void) {

	//--------------------------------------------------------------
	//ゲーム中の動きはここに書く
	//ゲーム中はこの関数_を1秒間に60回呼び出している
	//--------------------------------------------------------------
	
	//カメラ操作
	ControlCamera();
	//スカイボックス
	RenderSkybox();
	

	if (PUSH(CInput::eButton1)) {
		for (int i = 0; i < 2; i++) {
			const float speed = 0.002f;
			CVector3D vec(Utility::Rand(-speed, speed), Utility::Rand(0.0f, speed), Utility::Rand(-speed, speed));
			m_list.push_back(new ParticleEffect("SmokeB", CVector3D(0, 0, 0), vec, CVector3D(0,0.001f,0), 0.5, 0.1f,1.0f,-0.02f, ParticleEffect::eBlendAlpha,true));
		}
		for (int i = 0; i < 5; i++) {
			const float speed = 0.002f;
			CVector3D vec(Utility::Rand(-speed, speed), Utility::Rand(0.0f, speed), Utility::Rand(-speed, speed));
			m_list.push_back(new ParticleEffect("Bomb", CVector3D(0, 0, 0), vec, CVector3D(0, 0, 0), 1.0, 0.05f, 1.0f,-0.02f, ParticleEffect::eBlendAdd, true));
		}
		for (int i = 0; i < 40; i++) {
			const float speed = 0.05f;
			CVector3D vec(Utility::Rand(-speed, speed), Utility::Rand(-speed/2, speed), Utility::Rand(-speed, speed));
			m_list.push_back(new ParticleEffect("Bomb", CVector3D(0, 0, 0), vec, CVector3D(0, -0.001f, 0), 0.2, -0.005f, 1.5f,-0.01f, ParticleEffect::eBlendAdd, true));
		}
	}
	if (PUSH(CInput::eButton2)) {
		m_list.push_back(new MagicEffect("CircleR", CVector3D(0, 0, 0),CVector3D(DtoR(90), 0,0), 10.0, 120));
	}

	if (PUSH(CInput::eButton3)) {
		m_list.push_back(new UVEffect("Bullet", CVector3D(0, 0, 0), CVector3D(0, 0, 0), 10.0, CVector2D(0.01, 0), 120));
	}
	if (HOLD(CInput::eButton4)) {
		const float speed = 0.005f;
		CVector3D vec(Utility::Rand(-speed, speed), Utility::Rand(-speed, speed), Utility::Rand(-speed, speed));
		m_list.push_back(new ParticleEffect("Bomb", CVector3D(0, 0, 0), vec, CVector3D(0, 0.01f, 0), 1, -0.05,1.0f, -0.05, ParticleEffect::eBlendAdd, true));
		CLight::SetType(1, CLight::eLight_Point);
		CLight::SetPos(1, CVector3D(0, 0, 0));
		CLight::SetAttenuation(1, 0.05f);
		CLight::SetColor(1, CVector3D(0.2, 0.2, 0.2), CVector3D(0.8, 0.8, 0.8));
	}
	else {
		CLight::SetType(1, CLight::eLight_None);

	}


	//削除チェック
	auto it = m_list.begin(), end = m_list.end();
	while(it != end) {
		if ((*it)->m_kill) {
			delete* it;
			it = m_list.erase(it);
		} else {
			it++;
		}
	}


	//全て更新
	for (auto& v : m_list) {
		v->Update();
	}
	//全て描画
	for (auto& v : m_list) {
		v->Render();
	}


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
	CInput::SetButton(0, CInput::eButton1, '1');
	CInput::SetButton(0, CInput::eButton2, '2');
	CInput::SetButton(0, CInput::eButton3, '3');
	CInput::SetButton(0, CInput::eButton4, '4');
	CInput::SetButton(0, CInput::eButton5, '5');
	CInput::SetButton(0, CInput::eButton6, '6');
	CInput::SetButton(0, CInput::eButton7, '7');
	CInput::SetButton(0, CInput::eButton8, '8');
	CInput::SetButton(0, CInput::eButton9, '9');
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

	//各種エフェクト素材を読み込み
	ADD_RESOURCE("Bomb", CModel::CreateModel("Effect/Bomb.obj"));
	ADD_RESOURCE("Smoke", CModel::CreateModel("Effect/Smoke.obj"));
	ADD_RESOURCE("SmokeB", CModel::CreateModel("Effect/SmokeB.obj"));
	ADD_RESOURCE("Bullet", CModel::CreateModel("Effect/bullet.obj"));
	ADD_RESOURCE("CircleR", CModel::CreateModel("Effect/CircleR.obj"));

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
				//skybox->GetMaterial(i)->m_ambient = CVector4D(0.1f, 0.1f, 0.1f, 1.0f);
				//skybox->GetMaterial(i)->m_diffuse = CVector4D(0.1f, 0.1f, 0.1f, 1.0f);
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

void ControlCamera() {
	static CVector3D at(0, 0, 0);
	//カメラ回転値
	static CVector3D cam_rot(DtoR(30), DtoR(-180), 0);
	//注視点までの距離
	static float cam_dist = 10.0f;
	//追従カメラ
	//回転速度係数
	float cam_rot_speed = 0.01f;
	float cam_move_speed = 0.01f;
	//マウスの移動量
	CVector2D mouse_vec = CInput::GetMouseVec();
	if (HOLD(CInput::eMouseR)) {
		//マウス移動からカメラを回転
		//マウス移動Y→X軸回転(上下)
		//マウス移動X→Y軸回転(左右)
		cam_rot += CVector3D(mouse_vec.y, -mouse_vec.x, 0) * cam_rot_speed;
	}
	//上下角制限
	cam_rot.x = min(DtoR(60), max(DtoR(-35), cam_rot.x));
	//-180～180に正規化
	cam_rot.y = Utility::NormalizeAngle(cam_rot.y);
	//距離制限
	cam_dist = min(10.0f, max(2.0f, cam_dist - CInput::GetMouseWheel()));

	if (HOLD(CInput::eMouseC)) {
		at += CMatrix::MRotation(cam_rot) * CVector3D(mouse_vec.x, mouse_vec.y, 0) * cam_move_speed;
	}

	//位置と注視点を設定
	CVector3D cam_at = at;
	CVector3D cam_pos = cam_at + CMatrix::MRotation(cam_rot).GetFront() * -cam_dist;
	CCamera::GetCurrent()->LookAt(cam_pos, cam_at, CVector3D(0, 1, 0));
}