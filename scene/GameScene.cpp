#include "GameScene.h"
#include "TextureManager.h"
#include <cassert>
#include <random>
#include"AxisIndicator.h"
#include"WinApp.h"
#include"Vector3.h"
using namespace DirectX;

GameScene::GameScene() {}

GameScene::~GameScene() 
{ 
	delete sprite_; 
	delete model_;
}

void GameScene::Initialize() {

	dxCommon_ = DirectXCommon::GetInstance();
	input_ = Input::GetInstance();
	//audio_ = Audio::GetInstance();
	debugText_ = DebugText::GetInstance();
	textureHandle_ = TextureManager::Load("mario.jpg");
	sprite_ = Sprite::Create(textureHandle_, {100, 50});
	model_ = Model::Create();
	worldTransform_.Initialize();
	debugCamera_ = new DebugCamera(1280, 720);

	AxisIndicator::GetInstance()->SetVisible(true);

	AxisIndicator::GetInstance()->SetTargetViewProjection(&debugCamera_->GetViewProjection());
	
	viewProjection_.Initialize();
	//soundDataHandle_ = audio_->LoadWave("se_sad03.wav");
	//audio_->PlayWave(soundDataHandle_);
	//音楽再生
	//voiceHandle_ = audio_->PlayWave(soundDataHandle_, true);

	//X,Y,Zのスケーリング設定
	worldTransform_.scale_ = {5.0f, 5.0f, 5.0f};


	// X,Y,Z軸周りの回転角設定
	worldTransform_.rotation_ = {XM_PI / 4.0f, 0.0f,  0.0f};

	// X軸回転行列
	Matrix4 matRotX;

	matRotX = {1, 0, 0, 0, 
		0, cosf(XM_PI/4.0f), sinf(XM_PI/4.0f),0,
		0, -sinf(XM_PI/4.0f), cosf(XM_PI/4.0f), 0,
		0, 0, 0, 1};

	worldTransform_.matWorld_ = {1, 0, 0, 0, 0, 1, 0, 0, 0, 0, 1, 0, 0, 0, 0, 1};

	worldTransform_.matWorld_ *= matRotX;

	//行列の転送
	worldTransform_.TransferMatrix();



	// X,Y,Z軸周りの回転角設定
	worldTransform_.rotation_ = {0.0f, XM_PI / 4.0f, 0.0f};

	// Y軸回転行列
	Matrix4 matRotY;

	matRotY = {
	  cosf(XM_PI / 4.0f), 0, -sinf(XM_PI / 4.0f),0,
	  0, 1, 0, 0,
	  sinf(XM_PI / 4.0f), 0, cosf(XM_PI / 4.0f), 0,
	  0, 0, 0, 1};

	worldTransform_.matWorld_ = {1, 0, 0, 0, 0, 1, 0, 0, 0, 0, 1, 0, 0, 0, 0, 1};

	worldTransform_.matWorld_ *= matRotY;

	//行列の転送
	worldTransform_.TransferMatrix();


	// X,Y,Z軸周りの回転角設定
	worldTransform_.rotation_ = {0.0f, 0.0f, XM_PI / 4.0f};

	//Z軸回転行列
	Matrix4 matRotZ;

	matRotZ = 
	{cosf(XM_PI / 4.0f),sinf(XM_PI / 4.0f),0,0, 
	 -sinf(XM_PI / 4.0f),cosf(XM_PI / 4.0f),0,0,
	 0,0,1,0,
	 0,0,0,1};

	worldTransform_.matWorld_ = 
	{1, 0, 0, 0,
	 0, 1, 0, 0,
	 0, 0, 1, 0, 
	 0, 0, 0, 1};

	worldTransform_.matWorld_ *= matRotZ; 

	//行列の転送
	worldTransform_.TransferMatrix();


	// X,Y,Z軸周りの回転角設定
	//worldTransform_.rotation_ = {XM_PI / 4.0f, XM_PI / 4.0f, XM_PI / 4.0f};

	//Matrix4 matRot;

	//Matrix4 matRotX, matRotY, matRotZ;

	//matRotX = {
	//  1, 0, 0, 0,
	//  0, cosf(XM_PI / 4.0f), sinf(XM_PI / 4.0f), 0,
	//  0, -sinf(XM_PI / 4.0f), cosf(XM_PI / 4.0f),0,
	//  0, 0, 0, 1};


	//matRotY = {
	//	cosf(XM_PI / 4.0f), 0, -sinf(XM_PI / 4.0f), 0,
	//	0, 1, 0, 0,
	//    sinf(XM_PI / 4.0f), 0, cosf(XM_PI / 4.0f),  0,
	//	0, 0, 0, 1};


	//matRotZ = {
	//  cosf(XM_PI / 4.0f),sinf(XM_PI / 4.0f),0, 0,
	//  -sinf(XM_PI / 4.0f), cosf(XM_PI / 4.0f), 0,0,
	//  0, 0,1,0,
	//  0,0, 0,1};

	////各軸の回転行列を合成
	////matRot = matRotX * matRotY * matRotZ;

	//worldTransform_.matWorld_ = {1, 0, 0, 0, 0, 1, 0, 0, 0, 0, 1, 0, 0, 0, 0, 1};
	//worldTransform_.matWorld_ *= matRot;

	////行列の転送
	//worldTransform_.TransferMatrix();


	//X,Y,Z軸周りの平行移動設定
	worldTransform_.translation_ = {2.0f, 2.0f, 2.0f};
	
	//ワールドトランスフォームの初期化
	worldTransform_.Initialize();

	////カメラの視点座標を設定
	 viewProjection_.eye = {0, 0, -10};

	////カメラ上向きベクトルを設定
	//viewProjection_.up = {cosf(XM_PI / 4.0f), sinf(XM_PI / 4.0f), 0.0f};
	////ビュープロジェクションの初期化
	viewProjection_.Initialize();
}

void GameScene::Update() 
{

	//XMFLOAT2 position= sprite_->GetPosition();

	//視点の移動ベクトル
	Vector3 move = {0,0,0};

	//速さ
	const float kEyeSpeed = 0.2f;

	if (input_->PushKey(DIK_UP)) {
		move.z += kEyeSpeed;
	} 
	else if (input_->PushKey(DIK_DOWN)) {
		move.z -= kEyeSpeed;
	} 
	if (input_->PushKey(DIK_LEFT)) {
		move.x += kEyeSpeed;
	} 
	else if (input_->PushKey(DIK_RIGHT)) {
		move.x -= kEyeSpeed;
	}

	//視点の移動
	viewProjection_.eye += move;

	
	////向き
	XMFLOAT3 target = {sinf(worldTransform_.rotation_.y), 0, cosf(worldTransform_.rotation_.y)};


	//プレイヤーの移動ベクトル
	Vector3 playermove = {0, 0, 0};

	//速さ
	const float playerSpeed = 0.2f;

	if (input_->PushKey(DIK_W)) {
		playermove.z += playerSpeed;

	} else if (input_->PushKey(DIK_S)) {
		playermove.z -= playerSpeed;
	}
	if (input_->PushKey(DIK_A)) {
		playermove.x += playerSpeed;
	} else if (input_->PushKey(DIK_D)) {
		playermove.x -= playerSpeed;
	}

	//視点の移動
	worldTransform_.translation_ += playermove;
		// X軸回転行列
	Matrix4 matRotX;

	matRotX = {1, 0, 0, 0, 
		0, cosf(XM_PI/4.0f), sinf(XM_PI/4.0f),0,
		0, -sinf(XM_PI/4.0f), cosf(XM_PI/4.0f), 0,
		0, 0, 0, 1};

	worldTransform_.matWorld_ = {1, 0, 0, 0, 0, 1, 0, 0, 0, 0, 1, 0, 0, 0, 0, 1};

	worldTransform_.matWorld_ *= matRotX;

	//行列の転送
	worldTransform_.TransferMatrix();



	// X,Y,Z軸周りの回転角設定
	worldTransform_.rotation_ = {0.0f, XM_PI / 4.0f, 0.0f};

	// Y軸回転行列
	Matrix4 matRotY;

	matRotY = {
	  cosf(XM_PI / 4.0f), 0, -sinf(XM_PI / 4.0f),0,
	  0, 1, 0, 0,
	  sinf(XM_PI / 4.0f), 0, cosf(XM_PI / 4.0f), 0,
	  0, 0, 0, 1};

	worldTransform_.matWorld_ = {1, 0, 0, 0, 0, 1, 0, 0, 0, 0, 1, 0, 0, 0, 0, 1};

	worldTransform_.matWorld_ *= matRotY;

	//行列の転送
	worldTransform_.TransferMatrix();


	// X,Y,Z軸周りの回転角設定
	worldTransform_.rotation_ = {0.0f, 0.0f, XM_PI / 4.0f};

	//Z軸回転行列
	Matrix4 matRotZ;

	matRotZ = 
	{cosf(XM_PI / 4.0f),sinf(XM_PI / 4.0f),0,0, 
	 -sinf(XM_PI / 4.0f),cosf(XM_PI / 4.0f),0,0,
	 0,0,1,0,
	 0,0,0,1};

	worldTransform_.matWorld_ = 
	{1, 0, 0, 0,
	 0, 1, 0, 0,
	 0, 0, 1, 0, 
	 0, 0, 0, 1};

	worldTransform_.matWorld_ *= matRotZ; 

	worldTransform_.TransferMatrix();

	/*if (input_->PushKey(DIK_S)) {
		worldTransform_.translation_.x += target.x * 0.5;
		worldTransform_.translation_.y += target.y * 0.5;
		worldTransform_.translation_.z += target.z * 0.5;
	}

	if (input_->PushKey(DIK_W)) {
		worldTransform_.translation_.x -= target.x * 0.5;
		worldTransform_.translation_.y -= target.y * 0.5;
		worldTransform_.translation_.z -= target.z * 0.5;
	}*/

		
	//
	////カメラ
	//viewProjection_.eye.x = worldTransform_.translation_.x - target.x * 77;
	//viewProjection_.eye.y = worldTransform_.translation_.y - target.y * 77;
	//viewProjection_.eye.z = worldTransform_.translation_.z - target.z * 77;

	/////注視点
	viewProjection_.target.x = worldTransform_.translation_.x + target.x * 77;
	viewProjection_.target.y = worldTransform_.translation_.y + target.y * 77;
	viewProjection_.target.z = worldTransform_.translation_.z + target.z * 77;
	//

	viewProjection_.UpdateMatrix();
	debugCamera_->Update();
	if (input_->PushKey(DIK_Q)) {
		//worldTransform_.UpdateMatrix();
		// viewProjection_.UpdateMatrix();
	}
	

	//移動した座標をスプライトに反映
	//sprite_->SetPosition(position);

	//if (input_->TriggerKey(DIK_SPACE)) 
	//{
	//	//音楽停止
	//	audio_->StopWave(voiceHandle_);
	//}

	//デバック文字
	
	std::string strTranslasion_x =
	  std::string("Translasion_x:") + std::to_string(worldTransform_.translation_.x);

	std::string strTranslasion_y =
	  std::string("y:") + std::to_string(worldTransform_.translation_.y);

	std::string strTranslasion_z =
	  std::string("z:") + std::to_string(worldTransform_.translation_.z);

	std::string strRotation_x =
	  std::string("rotation__x:") + std::to_string(worldTransform_.rotation_.x);

	std::string strRotation_y = 
		std::string("y:") + std::to_string(worldTransform_.rotation_.y);

	std::string strRotation_z = 
		std::string("z:") + std::to_string(worldTransform_.rotation_.z);

	std::string strScale_x = std::string("scale_x:") + std::to_string(worldTransform_.scale_.x);

	std::string strScale_y = std::string("y:") + std::to_string(worldTransform_.scale_.y);

	std::string strScale_z = std::string("z:") + std::to_string(worldTransform_.scale_.z);

	//デバックテキストの表示
	debugText_->Print(strTranslasion_x, 50, 50, 1.0f);
	debugText_->Print(strTranslasion_y, 300, 50, 1.0f);
	debugText_->Print(strTranslasion_z, 450, 50, 1.0f);
	debugText_->Print(strRotation_x, 50, 100, 1.0f);
	debugText_->Print(strRotation_y, 300, 100, 1.0f);
	debugText_->Print(strRotation_z, 450, 100, 1.0f);
	debugText_->Print(strScale_x, 50, 150, 1.0f);
	debugText_->Print(strScale_y, 300, 150, 1.0f);
	debugText_->Print(strScale_z, 450, 150, 1.0f);
	debugText_->SetPos(50, 250);
	debugText_->Printf("eye:(%f,%f,%f)",viewProjection_.eye.x,viewProjection_.eye.y,viewProjection_.eye.z);

	
}

void GameScene::Draw() {

	// コマンドリストの取得
	ID3D12GraphicsCommandList* commandList = dxCommon_->GetCommandList();

#pragma region 背景スプライト描画
	// 背景スプライト描画前処理
	Sprite::PreDraw(commandList);

	/// <summary>
	/// ここに背景スプライトの描画処理を追加できる
	/// </summary>
	
	// スプライト描画後処理
	Sprite::PostDraw();
	// 深度バッファクリア
	dxCommon_->ClearDepthBuffer();
#pragma endregion

#pragma region 3Dオブジェクト描画
	// 3Dオブジェクト描画前処理
	Model::PreDraw(commandList);

	/// <summary>
	/// ここに3Dオブジェクトの描画処理を追加できる
	/// </summary>
	/// //3Dモデル描画
	model_->Draw(worldTransform_, viewProjection_, textureHandle_);
	model_->Draw(worldTransform_, debugCamera_->GetViewProjection(), textureHandle_);
	// 3Dオブジェクト描画後処理
	Model::PostDraw();
#pragma endregion

#pragma region 前景スプライト描画
	// 前景スプライト描画前処理
	Sprite::PreDraw(commandList);

	/// <summary>
	/// ここに前景スプライトの描画処理を追加できる
	/// </summary>
	//sprite_->Draw();

	// デバッグテキストの描画
	debugText_->DrawAll(commandList);
	//
	// スプライト描画後処理
	Sprite::PostDraw();

#pragma endregion
}
