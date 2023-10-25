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

	static void SetPlayer(Player* player) { sPlayer_ = player; }
	static void SetEnemy(Enemy* enemy) { sEnemy_ = enemy; }

private:
	// ワールド変換データ
	WorldTransform worldTransform_;
	Model* model_;
	static Player* sPlayer_;
	static Enemy* sEnemy_;
};