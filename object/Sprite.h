#pragma once
#include "../math/Matrix4x4.h"
#include "../VertexData.h"
#include "../Transform.h"
#include "../TransformationMatrix.h"
#include "../base/WorldTransform.h"
#include "../base/ViewProjection.h"
#include "../base/DirectXCommon.h"
#include "../Material.h"
#include <d3d12.h>
#include <wrl.h>

class Sprite
{
public:
	/// 
	/// Default Method
	/// 
	
	// デストラクタ
	~Sprite();

	/// <summary>
	/// 初期化
	/// </summary>
	/// <param name="isBackGround">この画像を背景にするか前景にするか</param>
	void Initialize(bool isBackGround);

	/// <summary>
	/// 描画処理
	/// </summary>
	/// <param name="pos">座標を入力</param>
	/// <param name="textureNum">textureManagerで登録したenum型の番号を入れる</param>
	void Draw(int textureNum);

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

	/// <summary>
	/// 座標の変更
	/// </summary>
	/// <param name="leftTop">左上の座標</param>
	void SetPosition(Vector2 leftTop) {
		worldTransform_.translation_.x = leftTop.x;
		worldTransform_.translation_.y = leftTop.y;
	}

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
	ViewProjection viewProjection_;
};

