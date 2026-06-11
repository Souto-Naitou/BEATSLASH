#include "GameClearScene.h"

#include <manager/RankingManager.h>
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
    constexpr const char* kTexSec = "gameclear/sec.png";

    constexpr const char* kTexDigits[10] = {
        "numbers/number_0.dds", "numbers/number_1.dds",
        "numbers/number_2.dds", "numbers/number_3.dds",
        "numbers/number_4.dds", "numbers/number_5.dds",
        "numbers/number_6.dds", "numbers/number_7.dds",
        "numbers/number_8.dds", "numbers/number_9.dds",
    };

    // 今回追加された記録の強調色
    constexpr Tako::Vector4 kNewRecordColor = { 1.0f, 1.0f, 0.0f, 1.0f };
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

    // RankingManager からランキングデータを読み込む
    const auto& rankTimes = RankingManager::GetInstance()->GetTimes();
    const int newRecordIndex = RankingManager::GetInstance()->GetLastAddedIndex();
    for (int i = 0; i < kRankCount; ++i)
    {
        times_[i] = (rankTimes[i] < RankingManager::kEmpty)
                    ? static_cast<uint32_t>(rankTimes[i])
                    : UINT32_MAX;
    }

    const float commonDelay = 2.0f;

    for (int i = 0; i < kRankCount; ++i)
    {
        pSpriteNums_[i] = std::make_unique<Tako::Sprite>();
        pSpriteNums_[i]->Initialize(kTexNums[i]);
        pSpriteNums_[i]->SetAnchorPoint({ 0.5f, 0.5f });

        viewTimes_[i].Initialize(digitHandles, "GameClear_time_" + std::to_string(i));
        viewTimes_[i].SetFontSize(layout_.fontSize * 1_vh);

        pSpriteSecs_[i] = std::make_unique<Tako::Sprite>();
        pSpriteSecs_[i]->Initialize(kTexSec);
        pSpriteSecs_[i]->SetAnchorPoint({ 0.0f, 0.5f });

        // 今回追加された記録は秒数とsecラベルを強調色にする
        if (i == newRecordIndex)
        {
            viewTimes_[i].SetColor(kNewRecordColor);
            pSpriteSecs_[i]->SetColor(kNewRecordColor);
        }

        const float startOffset = layout_.animStartOffset * 1_vw;
        const float delay       = layout_.animStagger * i + commonDelay;

        AnimationTween<float> tween(delay, layout_.animDuration, startOffset, 0.0f);
        tween.SetTransitionFunction(Math::Easing::EaseOutCubic);

        rowTimelines_[i].ClearTween();
        rowTimelines_[i].AddTween(std::move(tween));
        rowTimelines_[i].Start(startOffset);
    }
}

void GameClearScene::Finalize()
{
    for (int i = 0; i < kRankCount; ++i)
        viewTimes_[i].Finalize();
}

void GameClearScene::Update()
{
    pSpriteRanking_->SetPos({ layout_.rankingX * 1_vw, layout_.rankingY * 1_vh });
    pSpriteRanking_->Update();

    for (int i = 0; i < kRankCount; ++i)
    {
        const float rowY    = (layout_.rowStartY + layout_.rowSpacing * i) * 1_vh;
        const float offsetX = rowTimelines_[i].Update();

        pSpriteNums_[i]->SetPos({ layout_.numX * 1_vw + offsetX, rowY });
        pSpriteNums_[i]->Update();

        if (times_[i] != UINT32_MAX)
        {
            auto& props         = viewTimes_[i].GetFontLayoutProperties();
            props.leftTop       = { layout_.timeX * 1_vw + offsetX, rowY };
            props.anchorPoint   = { 0.0f, 0.5f };
            props.letterSpacing = layout_.letterSpacing;
            viewTimes_[i].SetFontSize(layout_.fontSize * 1_vh);
            viewTimes_[i].SetNumber(times_[i]);
            viewTimes_[i].Update();

            pSpriteSecs_[i]->SetPos({ layout_.secLabelX * 1_vw + offsetX, rowY });
            pSpriteSecs_[i]->Update();
        }
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
        if (times_[i] != UINT32_MAX)
        {
            viewTimes_[i].Draw();
            pSpriteSecs_[i]->Draw();
        }
    }
}

void GameClearScene::DrawWithoutEffect()
{
}

void GameClearScene::DrawImGui()
{
}
