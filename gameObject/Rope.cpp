#include "Rope.h"

Rope::Rope() {

}

Rope::~Rope() {

}

void Rope::Initialize(Model* model, Vector3 startPos, Vector3 endPos) {
	model_ = model;


	startPos_ = startPos;
	endPos_ = endPos;

	divisionCount_ = 30;

	Vector3 direction;
	direction = startPos_ - endPos_;

	direction /= (float)divisionCount_;

	direction *= -1;

	for (int i = 0; i < divisionCount_ + 1; i++) {
		std::unique_ptr<RopeNode> ropeNode = std::make_unique<RopeNode>();
		ropeNode->mass = 1.0f;
		if (i == 0 || i == divisionCount_) {
			ropeNode->isEdge = true;
		}
		else {
			ropeNode->isEdge = false;
		}
		ropeNode->velocity = { 0.0f, 0.0f, 0.0f };
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
		spring->restLength = (Length(direction) * 0.15f) / 10000.0f;
		spring->stiffness = 5000;
		spring->dampingCoefficient = 0.5f;
		spring->worldTransform.Initialize();
		Vector3 pos = (spring->node1->worldTransform.translation_ + spring->node2->worldTransform.translation_) / 2;
		spring->worldTransform.translation_ = pos;
		spring->worldTransform.scale_ *= 0.5f;
		springs_.push_back(std::move(spring));
	}
}

void Rope::Update() {
	for (auto springIt = springs_.begin(); springIt != springs_.end(); springIt++) {
		Spring* spring = springIt->get();
		Vector3 force = CalculateElasticForce(spring);
		Vector3 gravity = { 0.0f, -5.0f, 0.0f };

		if (!spring->node1->isEdge) {
			spring->node1->velocity += gravity * spring->node1->mass;
			spring->node1->velocity += force / spring->node1->mass * kDeltaTime;
			Vector3 dampingForce1 = -spring->dampingCoefficient * spring->node1->velocity;
			spring->node1->velocity += dampingForce1 / spring->node1->mass;
			spring->node1->worldTransform.translation_ += spring->node1->velocity * kDeltaTime;
		}

		if (!spring->node2->isEdge) {
			spring->node2->velocity += gravity * spring->node2->mass;
			spring->node2->velocity -= force / spring->node2->mass * kDeltaTime;
			Vector3 dampingForce2 = -spring->dampingCoefficient * spring->node2->velocity;
			spring->node2->velocity += dampingForce2 / spring->node2->mass;
			spring->node2->worldTransform.translation_ += spring->node2->velocity * kDeltaTime;
		}
	}

	for (auto springIt = springs_.begin(); springIt != springs_.end(); springIt++) {
		Spring* spring = springIt->get();
		Vector3 pos = (spring->node1->worldTransform.translation_ + spring->node2->worldTransform.translation_) / 2.0f;
		spring->worldTransform.translation_ = pos;
		spring->worldTransform.UpdateMatrix();
	}


	for (auto ropeNodeIt = ropeNodes_.begin(); ropeNodeIt != ropeNodes_.end(); ropeNodeIt++) {
		RopeNode* ropeNode = ropeNodeIt->get();
		ropeNode->worldTransform.UpdateMatrix();
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

Vector3 Rope::CalculateElasticForce(Spring* spring) {
	Vector3 diff = spring->node1->worldTransform.translation_ - spring->node2->worldTransform.translation_;
	float currentLength = Length(diff);
	float deltaLength = currentLength - spring->restLength;
	float forceLength = -spring->stiffness * deltaLength;
	Vector3 forceDirection = Normalize(diff);
	Vector3 elasticForce = forceLength * forceDirection;
	Vector3 relativeVelocity = spring->node1->velocity - spring->node2->velocity;
	Vector3 dampingForce = -spring->dampingCoefficient * relativeVelocity;

	return elasticForce + dampingForce;
}

void Rope::TestSpring() {
	int i = 0;
	for (auto ropeNodeIt = ropeNodes_.begin(); ropeNodeIt != ropeNodes_.end(); ropeNodeIt++) {
		i++;
		if (i == 10) {
			RopeNode* ropeNode = ropeNodeIt->get();

			ropeNode->worldTransform.translation_.y = 20;
			ropeNode->worldTransform.translation_.z = 10;
		}
	}
}