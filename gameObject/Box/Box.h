#pragma once
#include "../../base/Model.h"
#include "../../base/WorldTransform.h"
#include "../../components/Input.h"
#include "../../utility/GlobalVariables.h"
#include "../ICharacter.h"

#include <optional>

class Enemy;
class Player;

class Box
{
public:
	Box();
	~Box();

	void Initialize(Model* model, Vector3 pos, Vector3 size);

	void Update();

	void Draw(const ViewProjection& viewProjection, uint32_t textureHandle);

	void SetPlayer(Player* player) { player_ = player; }
	void SetEnemy(Enemy* enemy) { enemy_ = enemy; }

private:
	// ワールド変換データ
	WorldTransform worldTransform_;
	Model* model_;
	Player* player_;
	Enemy* enemy_;
};