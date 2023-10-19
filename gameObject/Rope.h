#pragma once
#include <list>
#include "../object/Sphere.h"
#include "../base/Model.h"
#include "../base/WorldTransform.h"
#include "../math/Vector2.h"
#include "../math/Vector3.h"
#include "../math/Vector4.h"
#include "../math/Matrix3x3.h"
#include "../math/Matrix4x4.h"

class Rope
{
public:
	Rope();
	~Rope();

	struct RopeNode {
		float mass;
		bool isEdge;
		bool isHit;
		WorldTransform worldTransform;
		Vector3Double velocity;
	};

	struct Spring {
		RopeNode* node1;
		RopeNode* node2;
		double restLength;
		double stiffness;
		double dampingCoefficient;
		WorldTransform worldTransform;
	};

	/// <summary>
	/// 初期化
	/// </summary>
	void Initialize(Model* model, Vector3 startPos, Vector3 endPos);

	/// <summary>
	/// 毎フレーム処理
	/// </summary>
	void Update();

	/// <summary>
	/// 描画
	/// </summary>
	void Draw(const ViewProjection& viewProjection, uint32_t textureHandle);

	void Clear();

	void TestSpring();

public: //ゲッターセッター

	inline std::list<std::unique_ptr<RopeNode>>::iterator GetListBeginSpring() { return ropeNodes_.begin(); }
	inline std::list<std::unique_ptr<RopeNode>>::iterator GetListEndSpring() { return ropeNodes_.end(); }
	inline bool IsEdgeNode(const RopeNode& ropeNode) { return ropeNode.isEdge; }
	inline bool IsHitNode(const RopeNode* ropeNode) { return ropeNode->isHit; }
	inline void SetIsHitNode(RopeNode* ropeNode, bool isHit) { ropeNode->isHit = isHit; }
	inline Vector3 GetWorldPos(const RopeNode& ropeNodes) {
		// ワールド座標を入れる変数
		Vector3 worldPos;
		// ワールド行列の平行移動成分を取得
		worldPos.x = ropeNodes.worldTransform.matWorld_.m[3][0];
		worldPos.y = ropeNodes.worldTransform.matWorld_.m[3][1];
		worldPos.z = ropeNodes.worldTransform.matWorld_.m[3][2];
		return worldPos;
	}

private: //メンバ関数

	Vector3Double CalculateElasticForce(Spring* spring);

private:
	int divisionCount_;
	Vector3 startPos_;
	Vector3 endPos_;
	std::list<std::unique_ptr<RopeNode>> ropeNodes_;
	std::list<std::unique_ptr<Spring>> springs_;
	Model* model_;

	const float kDeltaTime = 1.0f / 60.0f;
};



