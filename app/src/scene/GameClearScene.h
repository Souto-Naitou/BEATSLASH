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

    void SetTime(int rank, uint32_t totalSeconds) { times_[rank] = totalSeconds; }

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
        GameParameter(float, timeX,     44.0f);  // mm 左端
        GameParameter(float, colonX,    50.5f);  // コロン中心
        GameParameter(float, ssX,       53.0f);  // ss 左端
        GameParameter(float, secLabelX, 60.0f);  // "sec" テキスト左端

        // フォントサイズ・数字間隔 (vh / px)
        GameParameter(float, fontSize,        5.0f);
        GameParameter(float, letterSpacing,   0.0f);  // NumericView の字間

        // コロンドット
        GameParameter(float, colonDotSize,    1.0f);  // ドットの一辺 (vh)
        GameParameter(float, colonDotGap,     1.5f);  // 上下ドットの中心からのオフセット (vh)

        // 行アニメーション
        GameParameter(float, animStartOffset, 70.0f);  // 開始時の右方オフセット (vw)
        GameParameter(float, animDuration,     0.5f);  // 1行のアニメーション時間 (秒)
        GameParameter(float, animStagger,      0.15f); // 行ごとの遅延 (秒)
    };

    LayoutParam layout_;

    std::array<AnimationTimeline<float>, kRankCount> rowTimelines_;

    // 表示する時間 (秒単位)
    std::array<uint32_t, kRankCount> times_ = {};

    std::unique_ptr<Tako::Sprite> pSpriteRanking_;
    std::array<std::unique_ptr<Tako::Sprite>, kRankCount> pSpriteNums_;
    std::array<NumericView, kRankCount> viewMins_;
    std::array<NumericView, kRankCount> viewSecs_;
    std::array<std::unique_ptr<Tako::Sprite>, kRankCount> pSpriteColons1_;  // 上ドット
    std::array<std::unique_ptr<Tako::Sprite>, kRankCount> pSpriteColons2_;  // 下ドット
    std::array<std::unique_ptr<Tako::Sprite>, kRankCount> pSpriteSecs_;
};
