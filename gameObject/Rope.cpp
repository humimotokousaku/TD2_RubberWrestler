#include "Rope.h"

Rope::Rope() {

}

Rope::~Rope() {

}

void Rope::Initialize(Model* model, Vector3 startPos, Vector3 endPos) {
	model_ = model;


	startPos_ = startPos;
	endPos_ = endPos;

	divisionCount_ = 50;
	isHitRope_ = false;
	isParent_ = false;

	Vector3 direction;
	direction = startPos_ - endPos_;

	direction /= (float)divisionCount_;

	direction *= -1;

	for (int i = 0; i < divisionCount_ + 1; i++) {
		std::unique_ptr<RopeNode> ropeNode = std::make_unique<RopeNode>();
		ropeNode->mass = 2.0f;
		if (i == 0 || i == divisionCount_) {
			ropeNode->isEdge = true;
		}
		else {
			ropeNode->isEdge = false;
		}
		ropeNode->isHit = false;
		ropeNode->velocity = { 0.0f, 0.0f, 0.0f };
		ropeNode->externalForce_ = { 0.0f, -0.1f, 0.0f };
		ropeNode->worldTransform.Initialize();
		ropeNode->worldTransform.translation_.x = startPos_.x + float(direction.x * i);
		ropeNode->worldTransform.translation_.y = startPos_.y + float(direction.y * i);
		ropeNode->worldTransform.translation_.z = startPos_.z + float(direction.z * i);
		ropeNodes_.push_back(std::move(ropeNode));
	}

	for (int i = 0; i < divisionCount_; i++) {
		std::unique_ptr<Spring> spring = std::make_unique<Spring>();
		auto ropeNodeIt = ropeNodes_.begin();
		for (int j = 0; j < i; j++) {
			ropeNodeIt++;
		}
		spring->node1 = ropeNodeIt->get();
		ropeNodeIt++;
		spring->node2 = ropeNodeIt->get();
		spring->restLength = (Length(direction)) / 100000.0;
		spring->stiffness = 5000;
		spring->dampingCoefficient = 0.25;
		spring->worldTransform.Initialize();
		Vector3 pos = (spring->node1->worldTransform.translation_ + spring->node2->worldTransform.translation_) / 2;
		spring->worldTransform.translation_ = pos;
		spring->worldTransform.scale_ *= 0.5f;
		springs_.push_back(std::move(spring));
	}
}

void Rope::Update() {
	if (!isParent_) {
		for (auto springIt = springs_.begin(); springIt != springs_.end(); springIt++) {
			Spring* spring = springIt->get();
			Vector3Double force = CalculateElasticForce(spring);

			if (!spring->node1->isEdge) {
				spring->node1->velocity += spring->node1->externalForce_ * spring->node1->mass;
				spring->node1->velocity += force / spring->node1->mass * kDeltaTime;
				Vector3Double dampingForce1 = -spring->dampingCoefficient * spring->node1->velocity;
				spring->node1->velocity += dampingForce1 / spring->node1->mass;
				spring->node1->worldTransform.translation_.x += (float)spring->node1->velocity.x * kDeltaTime;
				spring->node1->worldTransform.translation_.y += (float)spring->node1->velocity.y * kDeltaTime;
				spring->node1->worldTransform.translation_.z += (float)spring->node1->velocity.z * kDeltaTime;
			}

			if (!spring->node2->isEdge) {
				spring->node2->velocity += spring->node2->externalForce_ * spring->node2->mass;
				spring->node2->velocity -= force / spring->node2->mass * kDeltaTime;
				Vector3Double dampingForce2 = -spring->dampingCoefficient * spring->node2->velocity;
				spring->node2->velocity += dampingForce2 / spring->node2->mass;
				spring->node2->worldTransform.translation_.x += (float)spring->node2->velocity.x * kDeltaTime;
				spring->node2->worldTransform.translation_.y += (float)spring->node2->velocity.y * kDeltaTime;
				spring->node2->worldTransform.translation_.z += (float)spring->node2->velocity.z * kDeltaTime;
			}
		}
		for (auto ropeNodeIt = ropeNodes_.begin(); ropeNodeIt != ropeNodes_.end(); ropeNodeIt++) {
			RopeNode* ropeNode = ropeNodeIt->get();
			ropeNode->worldTransform.UpdateMatrix();
		}
		for (auto springIt = springs_.begin(); springIt != springs_.end(); springIt++) {
			Spring* spring = springIt->get();
			Vector3 pos = (spring->node1->worldTransform.translation_ + spring->node2->worldTransform.translation_) / 2.0f;
			spring->worldTransform.translation_ = pos;
			spring->worldTransform.UpdateMatrix();
		}
	}
	else {
		for (auto springIt = springs_.begin(); springIt != springs_.end(); springIt++) {
			Spring* spring = springIt->get();
			spring->worldTransform.UpdateMatrix();
		}
	}
}

void Rope::Draw(const ViewProjection& viewProjection, uint32_t textureHandle) {
	for (auto springIt = springs_.begin(); springIt != springs_.end(); springIt++) {
		Spring* spring = springIt->get();
		model_->Draw(spring->worldTransform, viewProjection, textureHandle, kBlendModeNone);
	}
}

void Rope::Clear() {
	ropeNodes_.clear();
	springs_.clear();
}

Vector3Double Rope::CalculateElasticForce(Spring* spring) {
	Vector3 diff = spring->node1->worldTransform.translation_ - spring->node2->worldTransform.translation_;
	double currentLength = Length(diff);
	double deltaLength = currentLength - spring->restLength;
	double forceLength = -spring->stiffness * deltaLength;
	Vector3 forceDirection = Normalize(diff);
	Vector3Double elasticForce; 
	elasticForce.x = forceLength * forceDirection.x;
	elasticForce.y = forceLength * forceDirection.y;
	elasticForce.z = forceLength * forceDirection.z;
	Vector3Double relativeVelocity = spring->node1->velocity - spring->node2->velocity;
	Vector3Double dampingForce = -spring->dampingCoefficient * relativeVelocity;

	return elasticForce + dampingForce;
}

void Rope::TestSpring() {
	int i = 0;
	for (auto ropeNodeIt = ropeNodes_.begin(); ropeNodeIt != ropeNodes_.end(); ropeNodeIt++) {
		i++;
		if (i == 10) {
			RopeNode* ropeNode = ropeNodeIt->get();

			ropeNode->worldTransform.translation_.y = 20;
			ropeNode->worldTransform.translation_.z = 40;
		}
	}
}

void Rope::SetParent(Rope* parentRope) {
	auto SpringIt = this->GetListBeginSpring();
	for (auto parentRopeSpringIt = parentRope->GetListBeginSpring(); parentRopeSpringIt != parentRope->GetListEndSpring(); parentRopeSpringIt++, SpringIt++) {
		Rope::Spring* springNode = SpringIt->get();
		const Rope::Spring* parentSpringNode = parentRopeSpringIt->get();

		springNode->worldTransform.parent_ = &parentSpringNode->worldTransform;

		springNode->worldTransform.scale_.x = 1.0f;
		springNode->worldTransform.scale_.y = 1.0f;
		springNode->worldTransform.scale_.z = 1.0f;
	}
	isParent_ = true;
}

bool IsHitEnemy(Vector3 enemyPos, Rope::RopeNode* ropeNode) {
	Vector3 worldPos;
	worldPos.x = ropeNode->worldTransform.matWorld_.m[3][0];
	worldPos.y = ropeNode->worldTransform.matWorld_.m[3][1];
	worldPos.z = ropeNode->worldTransform.matWorld_.m[3][2];
	//詳細
	float ropeSize = 0.1f;
	float enemyWidth = 0.5f;
	//
	if (worldPos.x - ropeSize <= enemyPos.x + enemyWidth && enemyPos.x - enemyWidth <= worldPos.x + ropeSize) {
		if (worldPos.z - ropeSize <= enemyPos.z + enemyWidth && enemyPos.z - enemyWidth <= worldPos.z + ropeSize) {
			if (!ropeNode->isEdge) {
				return true;
			}
		}
	}
	return false;
}