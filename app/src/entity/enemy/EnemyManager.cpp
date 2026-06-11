#include "EnemyManager.h"
#include <manager/BeatManager.h>

#ifdef _DEBUG
#include <DebugUIManager.h>
#include <imgui.h>
#endif // _DEBUG

EnemyManager::EnemyManager(const ICharacter* target, const BeatClock* beatClock, Tako::EmitterManager* emitterManager)
	: pTarget_(target)
	, pBeatClock_(beatClock)
	, pEmitterManager_(emitterManager)
	, spawner_(target, beatClock, emitterManager)
{
#ifdef _DEBUG
	Tako::DebugUIManager::GetInstance()->RegisterGameObject("EnemyManager", [this]() { this->DrawImGui(); });
#endif
	
}

void EnemyManager::Update(uint32_t activeStageIndex)
{
	if (activeStageIndex < kMaxStages)
	{
		enemiesOnField_[activeStageIndex].Update();
	}
}

void EnemyManager::Draw(uint32_t activeStageIndex)
{
	if (activeStageIndex < kMaxStages)
	{
		enemiesOnField_[activeStageIndex].Draw();
	}

	// アクティブなステージの番号を更新
	if (activeStageIndex != activeStageIndex_)
	{
		if (activeStageIndex_ < kMaxStages)
		{
			// 非アクティブになったステージの処理
			for (auto& enemy : enemiesOnField_[activeStageIndex_].GetEnemies())
			{
				// コライダーを無効化
				enemy->DisableCollider();
			}

			// アクティブになったステージの処理
			for (auto& enemy : enemiesOnField_[activeStageIndex].GetEnemies())
			{
				// ステートマシンの初期化
				enemy->InitializeStateMachine();
				// コライダーの有効化
				enemy->EnableCollider();
			}
		}
		// 番号の記録
		activeStageIndex_ = activeStageIndex;
	}
}

void EnemyManager::SpawnEnemy(uint32_t stageIndex, const Tako::Vector3& position)
{
	if (stageIndex < kMaxStages)
	{
		// 敵の生成
		auto enemy = spawner_.Spawn(position);

		// ステージがアクティブ時の処理
		if (stageIndex == activeStageIndex_) // ここでは仮に現在のアクティブなステージがアクティブとする
		{
			// ステートマシンの初期化
			enemy->InitializeStateMachine();
			// コライダーの有効化
			enemy->EnableCollider();
		}

		// 敵をフィールドに追加
		enemiesOnField_[stageIndex].Add(std::move(enemy));
	}
}

void EnemyManager::SpawnEnemy(uint32_t stageIndex, const Tako::Transform& transform)
{
	if (stageIndex < kMaxStages)
	{
		// 敵の生成
		auto enemy = spawner_.Spawn(transform);

		// ステージがアクティブ時の処理
		if (stageIndex == activeStageIndex_) // ここでは仮に現在のアクティブなステージがアクティブとする
		{
			// ステートマシンの初期化
			enemy->InitializeStateMachine();
			// コライダーの有効化
			enemy->EnableCollider();
		}

		// 敵をフィールドに追加
		enemiesOnField_[stageIndex].Add(std::move(enemy));
	}
}

bool EnemyManager::IsEmpty(uint32_t stageIndex) const
{
	if (stageIndex < kMaxStages)
	{
		return enemiesOnField_[stageIndex].IsEmpty();
	}
	return true;
}

void EnemyManager::DrawImGui()
{
#ifdef _DEBUG
	static int spawnStageIndex = 0;
	static Tako::Vector3 spawnPosition = { 0.0f, 10.0f, 0.0f };

	// 敵の総数を計算
	size_t totalEnemyCount = 0;
	for (uint32_t i = 0; i < kMaxStages; ++i)
	{
		totalEnemyCount += enemiesOnField_[i].GetCount();
	}

	ImGui::SeparatorText("Status");
	ImGui::Text("Total Enemies on Field: %zu", totalEnemyCount);

	ImGui::SeparatorText("Spawn Enemy");
	ImGui::SliderInt("Stage Index", &spawnStageIndex, 0, kMaxStages - 1);
	ImGui::InputFloat3("Position", &spawnPosition.x);

	if (ImGui::Button("Spawn"))
	{
		SpawnEnemy(static_cast<uint32_t>(spawnStageIndex), spawnPosition);
	}

	ImGui::SeparatorText("Stages Info");
	for (uint32_t i = 0; i < kMaxStages; ++i)
	{
		enemiesOnField_[i].DrawImGui(i);
	}
#endif
}
