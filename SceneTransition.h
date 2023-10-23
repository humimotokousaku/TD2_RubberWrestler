#pragma once
#include "math/Matrix4x4.h"
#include "VertexData.h"
#include "Transform.h"
#include "TransformationMatrix.h"
#include "base/WorldTransform.h"
#include "base/ViewProjection.h"
#include "base/DirectXCommon.h"
#include "Manager/PipelineManager.h"
#include "Material.h"
#include <d3d12.h>
#include <wrl.h>

enum SceneChangeType {
	NONE_CHANGE,
	FADE_IN,
	FADE_OUT
};

class SceneTransition
{
public:
	/// 
	/// Default Method
	/// 

	// デストラクタ
	~SceneTransition();

	// シングルトン
	static SceneTransition* GetInstance();

	/// <summary>
	/// 初期化
	/// </summary>
	/// <param name="isBackGround">この画像を背景にするか前景にするか</param>
	void Initialize(bool isBackGround);

	/// <summary>
	/// 更新処理
	/// </summary>
	void Update();

	/// <summary>
	/// 描画処理
	/// </summary>
	/// <param name="pos">座標を入力</param>
	/// <param name="textureNum">textureManagerで登録したenum型の番号を入れる</param>
	void Draw(int textureNum, int blendNum);

	// 解放処理
	void Finalize();

	// メモリの確保やアドレス取得
	Microsoft::WRL::ComPtr<ID3D12Resource> CreateBufferResource(const Microsoft::WRL::ComPtr<ID3D12Device>& device, size_t sizeInBytes);
	void CreateVertexResource();
	void CreateVertexBufferView();
	void CreateMaterialResource();

	///
	///User Method
	/// 
	

	bool GetSceneChangeSignal() { return sceneChangeSignal_; }

private:
	// Material
	Material* materialData_;
	Microsoft::WRL::ComPtr<ID3D12Resource> materialResource_;
	Transform uvTransform_;
	Matrix4x4 uvTransformMatrix_;
	// Vertex
	Microsoft::WRL::ComPtr<ID3D12Resource> vertexResource_;
	D3D12_VERTEX_BUFFER_VIEW vertexBufferView_;
	VertexData* vertexData_;
	// index
	Microsoft::WRL::ComPtr<ID3D12Resource> indexResource_;
	D3D12_INDEX_BUFFER_VIEW indexBufferView_;
	uint32_t* indexData_ = nullptr;
	// Sprite
	Microsoft::WRL::ComPtr<ID3D12Resource> transformationMatrixResource_;
	TransformationMatrix* transformationMatrixData_;

	WorldTransform worldTransform_;

	bool isBackGround_ = false;
	ViewProjection viewProjection_;

public:
	static int sceneChangeType_;

	// シーンチェンジの信号
	bool sceneChangeSignal_;
};
