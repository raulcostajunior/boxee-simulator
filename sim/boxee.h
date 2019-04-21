#ifndef BOXEE_H
#define BOXEE_H

namespace sim {

enum class BoxeeState {
    OFF, // Powered off.
    BOOTING,
    ON_STANDARD,   // On and in standard mode (neither playing media nor handling text input).
    ON_PLAYING,    // On and playing some kind of media.
    ON_TEXT_ENTRY, // On and handling text input.
    SHUTTING_DOWN
};

enum class BoxeeMediaType {
    NONE, // No media playing (while not in playing mode).
    AUDIO,
    VIDEO,
    IMAGE
};

class Boxee
{
public:
    static Boxee &instance();

private:
    Boxee() = default;
    ~Boxee() = default;

    Boxee(const Boxee &) = delete;
    Boxee &operator=(const Boxee &) = delete;
    Boxee(Boxee &&) = delete;
    Boxee &operator=(const Boxee &&) = delete;
};

} // namespace sim

#endif // BOXEE_H
