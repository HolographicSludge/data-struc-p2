#pragma once

#include <string>

// Abstract base class: anything that can sit in a playlist.
// Derived classes decide how they describe themselves.
class MediaItem {
public:
    MediaItem(std::string title, int durationSeconds)
        : title_(std::move(title)), durationSeconds_(durationSeconds) {}
    virtual ~MediaItem() = default;

    const std::string& title() const { return title_; }
    int durationSeconds() const { return durationSeconds_; }

    // Pure virtual: each subclass supplies its own text.
    virtual std::string describe() const = 0;

protected:
    // Shared helper so subclasses format time the same way.
    std::string formattedDuration() const;

private:
    std::string title_;
    int durationSeconds_;
};
