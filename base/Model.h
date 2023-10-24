#pragma once
#include "../math/Matrix4x4.h"
#include "../VertexData.h"
#include "../Transform.h"
#include "WorldTransform.h"
#include "ViewProjection.h"
#include "../Material.h"
#include "MaterialData.h"
#include "../Manager/PipelineManager.h"
#include <d3d12.h>

class Model
{
public:
	~Model() = default;

	void Initialize(const std::string& directoryPath, const std::string& filename);

	/// <summary>
	/// 描画
	/// </summary>
	/// <param name="worldTransform">ワールドトランスフォーム</param>
	/// <param name="viewProjection">ビュープロジェクション</param>
	/// <param name="textureNumber">enumの番号を入れる</param>
	/// <param name="blendNum">ブレンドの種類を選ぶ</param>
	/// <param name="color">RGBAの順に入れる</param>
	void Draw(const WorldTransform& worldTransform, const ViewProjection& viewProjection, uint32_t textureNumber, int blendNum, Vector4 color);

	static Model* CreateModelFromObj(const std::string& directoryPath, const std::string& filename);

private:
	ModelData GetModelData() { return modelData_; }

	Microsoft::WRL::ComPtr<ID3D12Resource> CreateBufferResource(const Microsoft::WRL::ComPtr<ID3D12Device>& device, size_t sizeInBytes);

	void CreateVertexResource();

	void CreateVertexBufferView();

	void CreateMaterialResource();

	ModelData LoadObjFile(const std::string& directoryPath, const std::string& filename);

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

	ModelData modelData_;
};

