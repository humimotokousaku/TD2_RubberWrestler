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
		WorldTransform worldTransform;
		Vector3 velocity;
	};

	struct Spring {
		RopeNode* node1;
		RopeNode* node2;
		float restLength;
		float stiffness;
		float dampingCoefficient;
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
	void Draw(const ViewProjection& viewProjection);

	void Clear();

private: //メンバ関数

	Vector3 CalculateElasticForce(Spring* spring);

private:
	int divisionCount_;
	Vector3 startPos_;
	Vector3 endPos_;
	std::list<std::unique_ptr<RopeNode>> ropeNodes_;
	std::list<std::unique_ptr<Spring>> springs_;
	Model* model_;

	const float kDeltaTime = 1.0f / 60.0f;
};



