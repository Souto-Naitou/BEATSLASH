#include "GameClearScene.h"

#include <Input.h>
#include <SceneManager.h>
#include <SpriteBasic.h>
#include <TextureManager.h>
#include <utility/ViewportUnits.hpp>
#include <math/Easing.h>
#include <presentation/animation/AnimationTween.hpp>

namespace
{
    constexpr const char* kTexRanking = "gameclear/ranking.png";
    constexpr const char* kTexNums[5] = {
        "gameclear/1.png",
        "gameclear/2.png",
        "gameclear/3.png",
        "gameclear/4.png",
        "gameclear/5.png",
    };
    constexpr const char* kTexSec   = "gameclear/sec.png";
    constexpr const char* kTexColon = "white.png";

    constexpr const char* kTexDigits[10] = {
        "numbers/number_0.dds", "numbers/number_1.dds",
        "numbers/number_2.dds", "numbers/number_3.dds",
        "numbers/number_4.dds", "numbers/number_5.dds",
        "numbers/number_6.dds", "numbers/number_7.dds",
        "numbers/number_8.dds", "numbers/number_9.dds",
    };
}

void GameClearScene::Initialize()
{
    auto* tm = Tako::TextureManager::GetInstance();
    tm->LoadTexture(kTexRanking);
    tm->LoadTexture(kTexSec);
    for (const auto* path : kTexNums)
        tm->LoadTexture(path);
    for (const auto* path : kTexDigits)
        tm->LoadTexture(path);

    // NumericView 用の SRV ハンドルを一括取得
    std::array<NumericView::TextureHandleType, 10> digitHandles;
    for (int d = 0; d < 10; ++d)
        digitHandles[d] = tm->GetSRVIndex(kTexDigits[d]);

    pSpriteRanking_ = std::make_unique<Tako::Sprite>();
    pSpriteRanking_->Initialize(kTexRanking);
    pSpriteRanking_->SetAnchorPoint({ 0.5f, 0.5f });

    // 仮の時刻データ (秒単位) - 後でゲームの実データに差し替える
    times_ = { 75u, 92u, 103u, 118u, 145u };  // 1:15, 1:32, 1:43, 1:58, 2:25

    const float commonDelay = 2.0f;

    for (int i = 0; i < kRankCount; ++i)
    {
        // 順位数字 sprite
        pSpriteNums_[i] = std::make_unique<Tako::Sprite>();
        pSpriteNums_[i]->Initialize(kTexNums[i]);
        pSpriteNums_[i]->SetAnchorPoint({ 0.5f, 0.5f });

        // mm (分) NumericView
        viewMins_[i].Initialize(digitHandles, "GameClear_min_" + std::to_string(i));
        viewMins_[i].SetFontSize(layout_.fontSize * 1_vh);

        // ss (秒) NumericView
        viewSecs_[i].Initialize(digitHandles, "GameClear_sec_" + std::to_string(i));
        viewSecs_[i].SetFontSize(layout_.fontSize * 1_vh);

        // コロン (上ドット・下ドット)
        pSpriteColons1_[i] = std::make_unique<Tako::Sprite>();
        pSpriteColons1_[i]->Initialize(kTexColon);
        pSpriteColons1_[i]->SetAnchorPoint({ 0.5f, 0.5f });

        pSpriteColons2_[i] = std::make_unique<Tako::Sprite>();
        pSpriteColons2_[i]->Initialize(kTexColon);
        pSpriteColons2_[i]->SetAnchorPoint({ 0.5f, 0.5f });

        // "sec" テキスト sprite
        pSpriteSecs_[i] = std::make_unique<Tako::Sprite>();
        pSpriteSecs_[i]->Initialize(kTexSec);
        pSpriteSecs_[i]->SetAnchorPoint({ 0.0f, 0.5f });

        // 行ごとにスタッガーされたスライドインアニメーション (X オフセット: startOffset → 0)
        const float startOffset = layout_.animStartOffset * 1_vw;
        const float delay       = layout_.animStagger * i + commonDelay;
        const float duration    = layout_.animDuration;

        AnimationTween<float> tween(delay, duration, startOffset, 0.0f);
        tween.SetTransitionFunction(Math::Easing::EaseOutCubic);

        rowTimelines_[i].ClearTween();
        rowTimelines_[i].AddTween(std::move(tween));
        rowTimelines_[i].Start(startOffset);
    }
}

void GameClearScene::Finalize()
{
    for (int i = 0; i < kRankCount; ++i)
    {
        viewMins_[i].Finalize();
        viewSecs_[i].Finalize();
    }
}

void GameClearScene::Update()
{
    pSpriteRanking_->SetPos({ layout_.rankingX * 1_vw, layout_.rankingY * 1_vh });
    pSpriteRanking_->Update();

    for (int i = 0; i < kRankCount; ++i)
    {
        const float rowY    = (layout_.rowStartY + layout_.rowSpacing * i) * 1_vh;
        const float offsetX = rowTimelines_[i].Update();

        // 順位数字
        pSpriteNums_[i]->SetPos({ layout_.numX * 1_vw + offsetX, rowY });
        pSpriteNums_[i]->Update();

        // 分・秒 を計算
        const uint32_t totalSec = times_[i];
        const uint32_t mm       = totalSec / 60u;
        const uint32_t ss       = totalSec % 60u;

        // mm NumericView
        auto& minProps         = viewMins_[i].GetFontLayoutProperties();
        minProps.leftTop       = { layout_.timeX * 1_vw + offsetX, rowY };
        minProps.anchorPoint   = { 0.0f, 0.5f };
        minProps.letterSpacing = layout_.letterSpacing;
        viewMins_[i].SetNumber(mm);
        viewMins_[i].Update();

        // コロン (上ドット・下ドット)
        const float dotSize   = layout_.colonDotSize * 1_vh;
        const float dotOffset = layout_.colonDotGap  * 1_vh;
        const float colonPx   = layout_.colonX * 1_vw + offsetX;

        pSpriteColons1_[i]->SetPos({ colonPx, rowY - dotOffset });
        pSpriteColons1_[i]->SetSize({ dotSize, dotSize });
        pSpriteColons1_[i]->Update();

        pSpriteColons2_[i]->SetPos({ colonPx, rowY + dotOffset });
        pSpriteColons2_[i]->SetSize({ dotSize, dotSize });
        pSpriteColons2_[i]->Update();

        // ss NumericView
        auto& secProps         = viewSecs_[i].GetFontLayoutProperties();
        secProps.leftTop       = { layout_.ssX * 1_vw + offsetX, rowY };
        secProps.anchorPoint   = { 0.0f, 0.5f };
        secProps.letterSpacing = layout_.letterSpacing;
        viewSecs_[i].SetNumber(ss);
        viewSecs_[i].Update();

        // "sec" テキスト
        pSpriteSecs_[i]->SetPos({ layout_.secLabelX * 1_vw + offsetX, rowY });
        pSpriteSecs_[i]->Update();
    }

    if (Tako::Input::GetInstance()->TriggerKey(DIK_SPACE) ||
        Tako::Input::GetInstance()->TriggerKey(DIK_ESCAPE))
    {
        Tako::SceneManager::GetInstance()->ChangeScene("title", Tako::TransitionManager::EffectType::Fade, 0.5f);
    }
}

void GameClearScene::Draw()
{
    Tako::SpriteBasic::GetInstance()->SetCommonRenderSetting();

    pSpriteRanking_->Draw();
    for (int i = 0; i < kRankCount; ++i)
    {
        pSpriteNums_[i]->Draw();
        viewMins_[i].Draw();
        pSpriteColons1_[i]->Draw();
        pSpriteColons2_[i]->Draw();
        viewSecs_[i].Draw();
        pSpriteSecs_[i]->Draw();
    }
}

void GameClearScene::DrawWithoutEffect()
{
}

void GameClearScene::DrawImGui()
{
}
