#include "GameScene.h"
#include "TextureManager.h"
#include "WinApp.h"
#include <cassert>
#include "AxisIndicator.h"
#include "PrimitiveDrawer.h"

using namespace DirectX;

GameScene::GameScene() {}

GameScene::~GameScene() 
{
	delete model_; 
	delete debugCamera_;
}

void GameScene::Initialize() {

	textureHandle_ = TextureManager::Load("mario.jpg");
	dxCommon_ = DirectXCommon::GetInstance();
	input_ = Input::GetInstance();
	audio_ = Audio::GetInstance();
	debugText_ = DebugText::GetInstance();
	sprite_ = Sprite::Create(textureHandle_, {100, 50});
	//3Dモデルの生成
	model_ = Model::Create();
	debugCamera_ = new DebugCamera(WinApp::kWindowWidth, WinApp::kWindowHeight);

	// X,Y,Zのスケーリング設定
	worldTransform_.scale_ = {5.0f, 5.0f, 5.0f};

	// X,Y,Z軸周りの回転角設定
	worldTransform_.rotation_ = {XM_PI / 4.0f, XM_PI / 4.0f, 0.0f};

	// X,Y,Z軸周りの平行移動設定
	worldTransform_.translation_ = {10.0f, 10.0f, 10.0f};
	//ワールドトランスフォームの初期化
	worldTransform_.Initialize();

	//ビュープロジェクションの初期化
	viewProjection_.Initialize();

	AxisIndicator::GetInstance()->SetVisible(true);

	AxisIndicator::GetInstance()->SetTargetViewProjection(&debugCamera_->GetViewProjection());

	PrimitiveDrawer::GetInstance()->SetViewProjection(&debugCamera_->GetViewProjection());
}

void GameScene::Update() 
{ 
	
	XMFLOAT2 position = sprite_->GetPosition();

	//座標を{2,0}移動
	position.x += 2.0f;
	position.y += 1.0f;

	//移動した座標をスプライトに反映
	sprite_->SetPosition(position);

	debugCamera_->Update(); 
	//デバック文字

	std::string strTranslasion_x =
	  std::string("Translasion_x:") + std::to_string(worldTransform_.translation_.x);

	std::string strTranslasion_y =
	  std::string("y:") + std::to_string(worldTransform_.translation_.y);

	std::string strTranslasion_z =
	  std::string("z:") + std::to_string(worldTransform_.translation_.z);

	std::string strRotation_x =
	  std::string("rotation__x:") + std::to_string(worldTransform_.rotation_.x);

	std::string strRotation_y = std::string("y:") + std::to_string(worldTransform_.rotation_.y);

	std::string strRotation_z = std::string("z:") + std::to_string(worldTransform_.rotation_.z);

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
}
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

	//model_->Draw(worldTransform_,viewProjection_,textureHandle_);
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

	// デバッグテキストの描画
	debugText_->DrawAll(commandList);
	//
	// スプライト描画後処理
	Sprite::PostDraw();

#pragma endregion
}
