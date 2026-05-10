#pragma once
#include <ozSound/audio/SoundEngine.h>

#include <string>
#include <cstdint>
#include <memory>

/// <summary>
/// ビート管理クラス
/// </summary>
class BeatManager
{
// 制御クラス？
public:
    BeatManager();
    ~BeatManager() = default;

    // 初期化 - BPMとオフセットを設定
    void Initialize(float bpm, float offset = 0.0f, const std::string& soundPath = "resources/audio/Metronome.wav");

    // 更新
    void Update();

    // 拍動作の開始
    void Start();

    // 拍動作の停止
    void Stop();

    // リセット
    void Reset();

    // 現在の拍数を取得（小数部分も含む）
    float GetCurrentBeat() const;

    // 最寄りの整数拍数を取得
    int GetNearestBeat() const;

    // 新しい拍かどうかをチェック
    bool IsNewBeat();

    // 拍が発生したかどうかをチェック（特定のタイミング範囲内）
    bool IsBeatTriggered(float tolerance = 0.05f) const;

    // BPM変更
    void SetBPM(float bpm);

    // 開始オフセット設定
    void SetOffset(float offset) { offset_ = offset; }

    // ボリューム設定
    void SetVolume(float volume) { volume_ = volume; }

    bool GetSoundEnabled() const { return soundEnabled_; }

    /// ========================================
    /// Setter
    /// ========================================
    void SetEnableSound(bool enable) { soundEnabled_ = enable; }

    void SetMusicSoundHandle(ozSound::SoundHandle soundHandle);

    // 1拍あたりの秒数を計算
    float GetSecondsPerBeat() const { return 60.0f / bpm_; }
private:

    bool IsValid() const;

private:
    float bpm_ = 120.0f;       // 1分あたりの拍数
    float offset_ = 0.0f;      // 開始オフセット（秒）
    int lastBeat_ = 0;        // 最後に処理した拍数
    bool playing_ = false;     // 再生中かどうか

    // サウンド関連
    ozSound::SoundHandle metornomeHandle_ = 0; // メトロノームのサウンドハンドル

    ozSound::SoundHandle musicHandle_ = (std::numeric_limits<uint32_t>::max)(); // 音楽のサウンドハンドル

    //std::shared_ptr<ozSound::VoiceInstance> musicVoiceInstance_= nullptr; // 音楽のボイスインスタンス

    float volume_ = 0.5f;
    bool soundEnabled_ = true;

};