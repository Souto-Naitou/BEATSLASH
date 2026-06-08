#pragma once
#include "EnemiesOnField.h"

class BeatClock;
#include "EnemySpawner.h"
#include <array>

/**
 * @brief 敵の生成とステージごとの管理を行うクラス
 */
class EnemyManager
{
public:
	// 管理するステージの最大数(ある程度余裕を持って設定、必要に応じて調整)
	static constexpr uint32_t kMaxStages = 20;

	/**
	 * @brief コンストラクタ
	 * @param target プレイヤー（ターゲット）のポインタ
	 * @param beatClock ビートクロックのポインタ
	 */
	EnemyManager(const ICharacter* target, const BeatClock* beatClock);

	/**
	 * @brief 指定したアクティブステージの敵の更新処理を行う。
	 * @param activeStageIndex 更新対象のアクティブなステージインデックス
	 */
	void Update(uint32_t activeStageIndex);

	/**
	 * @brief 指定したアクティブステージの敵の描画処理を行う。
	 * @param activeStageIndex 描画対象のアクティブなステージインデックス
	 */
	void Draw(uint32_t activeStageIndex);

	/**
	 * @brief 指定したステージに敵を座標指定で生成する。
	 * @param stageIndex 生成対象のステージインデックス
	 * @param position 生成座標
	 */
	void SpawnEnemy(uint32_t stageIndex, const Tako::Vector3& position);

	/**
	 * @brief 指定したステージに敵をトランスフォーム指定で生成する。
	 * @param stageIndex 生成対象のステージインデックス
	 * @param transform トランスフォーム
	 */
	void SpawnEnemy(uint32_t stageIndex, const Tako::Transform& transform);

	/**
	 * @brief 指定したステージの敵が全滅しているか（空か）どうかを取得する。
	 * @param stageIndex 判定対象のステージインデックス
	 * @return 敵が全滅している場合は true、生存している敵がいる場合は false
	 */
	bool IsEmpty(uint32_t stageIndex) const;

	/**
	 * @brief デバッグ用のImGui描画を行う。
	 */
	void DrawImGui();

private:
	// ターゲット（プレイヤー）のポインタ
	const ICharacter* pTarget_ = nullptr;

	// ビートクロックのポインタ
	const BeatClock* pBeatClock_ = nullptr;

	// フィールド上の敵の管理
	std::array<EnemiesOnField, kMaxStages> enemiesOnField_;
	
	// 敵のスポナー
	EnemySpawner spawner_;
};

