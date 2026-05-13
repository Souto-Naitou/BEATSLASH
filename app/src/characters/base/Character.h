#pragma once
#include <Object3d.h>

class Character
{
public:
	Character() = default;
	virtual ~Character() = default;
	/// <summary>
	/// キャラクターの初期化
	/// </summary>
	virtual void Initialize();
	/// <summary>
	/// キャラクターの更新
	/// </summary>
	virtual void Update();
	/// <summary>
	/// キャラクターの描画
	/// </summary>
	virtual void Draw();

private:
	std::unique_ptr<Tako::Object3d> model_; ///< キャラクターの3Dモデル
	Tako::Transform transform_; ///< キャラクターのトランスフォーム
};
