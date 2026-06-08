#include "EnemyManager.h"
#include <imgui.h>
#include <DebugUIManager.h>

EnemyManager::EnemyManager(const ICharacter* target)
	: pTarget_(target)
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
}

void EnemyManager::SpawnEnemy(uint32_t stageIndex, const Tako::Vector3& position)
{
	if (stageIndex < kMaxStages)
	{
		auto enemy = spawner_.Spawn(position);
		enemiesOnField_[stageIndex].Add(std::move(enemy));
	}
}

void EnemyManager::SpawnEnemy(uint32_t stageIndex, const Tako::Transform& transform)
{
	if (stageIndex < kMaxStages)
	{
		auto enemy = spawner_.Spawn(transform);
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
	static float spawnPosition[3] = { 0.0f, 150.0f, 0.0f };

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
	ImGui::InputFloat3("Position", spawnPosition);

	if (ImGui::Button("Spawn"))
	{
		SpawnEnemy(static_cast<uint32_t>(spawnStageIndex), Tako::Vector3{ spawnPosition[0], spawnPosition[1], spawnPosition[2] });
	}

	ImGui::SeparatorText("Stages Info");
	for (uint32_t i = 0; i < kMaxStages; ++i)
	{
		enemiesOnField_[i].DrawImGui(i);
	}
#endif
}
