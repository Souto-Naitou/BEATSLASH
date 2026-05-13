#pragma once
#include <ozSound/audio/SoundEngine.h>
#include <Windows.h>
#include <cstdint>
#include <limits>

/// <summary>
/// BPM と offset に基づいて拍を管理するクラス
/// 時刻ソースは QPC 自走モードと楽曲同期 (Sound) モードを切り替え可能
/// </summary>
class BeatManager
{
public:
    /// 時刻ソースの種別
    enum class CountMode
    {
        QPC,    ///< QPC 自走 (サウンドバッファ非依存)
        Sound   ///< SoundEngine::GetElapsedTime ベース (楽曲同期)
    };

    BeatManager();
    ~BeatManager() = default;

    //  ライフサイクル 

    /// BPM と offset を設定する。再生は開始しない
    void Initialize(float bpm, float offset = 0.0f);

    /// 拍カウントを開始する
    /// SetMusicSoundHandle で有効なハンドルが事前に設定されていれば Sound モード、
    /// そうでなければ QPC モードで開始する
    void Start();

    /// 拍カウントを停止する
    void Stop();

    /// 内部時刻と拍カウンタをリセットする (再生状態は維持)
    void Reset();

    /// 毎フレーム呼び出す。メトロノーム音の発火判定を行う
    void Update();

    //  設定 

    void SetBPM(float bpm);
    void SetOffset(float offset) { offset_ = offset; }
    void SetVolume(float volume) { volume_ = volume; }
    void SetEnableSound(bool e) { soundEnabled_ = e; }
    bool GetSoundEnabled() const { return soundEnabled_; }

    /// Sound モードで使用する楽曲のサウンドハンドルを設定する
    /// Start() 呼び出し前に設定すること
    void SetMusicSoundHandle(ozSound::SoundHandle handle);

    //  状態取得 

    /// 現在の時刻ソースモードを取得
    CountMode GetCountMode() const { return countMode_; }

    //  拍情報の取得 

    /// 1 拍あたりの秒数 (= 60 / BPM)
    float GetSecondsPerBeat() const { return 60.0f / bpm_; }

    /// 現在拍 (小数含む)
    float GetCurrentBeat() const;

    /// 現在進行中の整数拍番号 (floor)
    int   GetCurrentBeatIndex() const;

    /// 最も近い拍との差 (拍単位、範囲 [-0.5, +0.5))
    /// 正 = 拍を過ぎている / 負 = 拍より手前
    float GetDeltaToNearestBeat() const;

    /// 最も近い拍との差 (秒単位)
    float GetDeltaToNearestBeatSeconds() const;

private:
    // 内部時刻計算 (精度確保のため double で実装)
    double GetElapsedTimeD() const;
    double GetCurrentBeatD() const;
    double GetSecondsPerBeatD() const { return 60.0 / static_cast<double>(bpm_); }
    double ClockToSecondsD(int64_t clock) const;

    // 状態チェック 
    bool IsValid() const;
    bool IsHandleValid() const;

    // 拍進行判定 (副作用あり: 進んでいれば lastBeat_ を更新)
    bool ConsumeBeatIfAdvanced();

private:
    // 基本パラメータ
    float bpm_          = 120.0f;
    float offset_       = 0.0f;
    float volume_       = 0.5f;
    bool  soundEnabled_ = true;
    bool  playing_      = false;
    int   lastBeat_     = -1;

    // 時刻ソース
    CountMode     countMode_    = CountMode::QPC;
    LARGE_INTEGER qpcStart_     = {};
    LARGE_INTEGER qpcFrequency_ = {};

    // Sound モード用
    ozSound::SoundHandle musicHandle_ = (std::numeric_limits<uint32_t>::max)();
};