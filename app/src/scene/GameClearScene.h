#pragma once

#include <BaseScene.h>
#include <Sprite.h>
#include <debug/GameParameter.h>
#include <presentation/animation/AnimationTimeline.hpp>
#include <font/NumericView.h>
#include <memory>
#include <array>

class GameClearScene : public Tako::BaseScene
{
public:
    void Initialize() override;
    void Finalize() override;
    void Update() override;
    void Draw() override;
    void DrawWithoutEffect() override;
    void DrawImGui() override;

private:
    static constexpr int kRankCount = 5;

    EnableDebug("GameClearScene");

    struct LayoutParam
    {
        // RANKING タイトル位置 (vw/vh)
        GameParameter(float, rankingX,   50.0f);
        GameParameter(float, rankingY,   18.0f);

        // 行レイアウト (vh)
        GameParameter(float, rowStartY,  36.0f);
        GameParameter(float, rowSpacing, 10.0f);

        // 列X位置 (vw)
        GameParameter(float, numX,      38.0f);  // 順位数字
        GameParameter(float, timeX,     50.0f);  // 秒数 NumericView 左端
        GameParameter(float, secLabelX, 60.0f);  // "sec" テキスト左端

        // フォントサイズ・字間
        GameParameter(float, fontSize,      5.0f);
        GameParameter(float, letterSpacing, 0.0f);

        // 行アニメーション
        GameParameter(float, animStartOffset, 70.0f);
        GameParameter(float, animDuration,     0.5f);
        GameParameter(float, animStagger,      0.15f);
    };

    LayoutParam layout_;

    std::array<AnimationTimeline<float>, kRankCount> rowTimelines_;

    // 表示する時間 (秒単位)。UINT32_MAX = 未記録
    std::array<uint32_t, kRankCount> times_ = {};

    std::unique_ptr<Tako::Sprite> pSpriteRanking_;
    std::array<std::unique_ptr<Tako::Sprite>, kRankCount> pSpriteNums_;
    std::array<NumericView, kRankCount> viewTimes_;
    std::array<std::unique_ptr<Tako::Sprite>, kRankCount> pSpriteSecs_;
};
