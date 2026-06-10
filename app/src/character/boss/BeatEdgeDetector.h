#pragma once

class BeatClock;
namespace Tako { class BTBlackboard; }

/// <summary>
/// 拍境界（整数拍の切り替わり）を検知するクラス
/// ボスのリズム連携基盤。検知結果と拍情報はブラックボードへ書き込み、攻撃ノード等が参照できるようにする
/// </summary>
class BeatEdgeDetector
{
public:
    /// <summary>
    /// 検知状態のリセット
    /// </summary>
    void Reset();

    /// <summary>
    /// 拍境界の検知とブラックボードへの拍情報書き込み
    /// </summary>
    /// <param name="beatClock">ビートクロック（null許容）</param>
    /// <param name="blackboard">書き込み先ブラックボード（null許容）</param>
    /// <returns>このフレームで拍境界を跨いだ場合 true</returns>
    bool Update(const BeatClock* beatClock, Tako::BTBlackboard* blackboard = nullptr);

    bool IsBeatPassed() const { return beatPassed_; }

private:
    // 前フレームの整数拍番号（-1は未計測）
    int lastBeatIndex_ = -1;
    bool beatPassed_ = false;
};
