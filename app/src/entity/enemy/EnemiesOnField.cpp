#include "EnemiesOnField.h"

#include <character/enemy/Enemy.h>
#include <string>

#ifdef _DEBUG
#include <imgui.h>
#endif // _DEBUG

void EnemiesOnField::Add(std::unique_ptr<Enemy> enemy)
{
    if (enemy)
    {
        enemies_.push_back(std::move(enemy));
    }
}

void EnemiesOnField::Update()
{
    for (auto it = enemies_.begin(); it != enemies_.end(); )
    {
        if ((*it) && (*it)->IsAlive())
        {
            (*it)->Update();
            ++it;

        }
        else
        {
            // 死亡している敵をリストから削除
            it = enemies_.erase(it);
        }
    }
}

void EnemiesOnField::Draw()
{
    for (auto& enemy : enemies_)
    {
        if (enemy)
        {
            enemy->Draw();
        }
    }
}

bool EnemiesOnField::IsEmpty() const
{
    return enemies_.empty();
}

void EnemiesOnField::DrawImGui(uint32_t stageIndex)
{
#ifdef _DEBUG
	size_t count = GetCount();
	if (count > 0)
	{
		std::string stageLabel = "Stage " + std::to_string(stageIndex) + " (" + std::to_string(count) + " enemies)";
		if (ImGui::TreeNode(stageLabel.c_str()))
		{
			int enemyIndex = 0;
			for (auto& enemy : enemies_)
			{
				if (enemy)
				{
					std::string enemyLabel = "Enemy " + std::to_string(enemyIndex) + " (Address: " + std::to_string(reinterpret_cast<uintptr_t>(enemy.get())) + ")";
					if (ImGui::TreeNode(enemyLabel.c_str()))
					{
						enemy->DrawImGui();
						ImGui::TreePop();
					}
					enemyIndex++;
				}
			}
			ImGui::TreePop();
		}
	}
#endif
}
