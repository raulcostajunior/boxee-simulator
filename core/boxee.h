#ifndef BOXEE_H
#define BOXEE_H

#include <cstdint>

namespace core {

enum class BoxeeState : uint8_t {
    OFF, // Powered off.
    BOOTING,
    ON_STANDARD,   // On and in standard mode (neither playing media nor handling text input).
    ON_PLAYING,    // On and playing some kind of media.
    ON_TEXT_ENTRY, // On and handling text input.
    SHUTTING_DOWN
};

enum class BoxeeMediaType : uint8_t {
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

} // namespace core

#endif // BOXEE_H
