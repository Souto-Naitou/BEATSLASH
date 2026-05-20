class HSV
{
public:

    HSV() = default;
    HSV(float h, float s, float v) : h_(h), s_(s), v_(v) {}
    HSV(const HSV& other) : h_(other.h_), s_(other.s_), v_(other.v_) {}
    HSV(HSV&& other) noexcept : h_(other.h_), s_(other.s_), v_(other.v_) {}

    float& h() { return h_; }
    float& s() { return s_; }
    float& v() { return v_; }

    const float& h() const { return h_; }
    const float& s() const { return s_; }
    const float& v() const { return v_; }

private:
    float h_ = 0.0f; // Hue [0, 360)
    float s_ = 1.0f; // Saturation [0, 1]
    float v_ = 1.0f; // Value [0, 1]
};