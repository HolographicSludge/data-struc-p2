#include "MediaItem.h"

#include <cstdio>

std::string MediaItem::formattedDuration() const {
    char buf[16];
    std::snprintf(buf, sizeof buf, "%d:%02d", durationSeconds_ / 60, durationSeconds_ % 60);
    return buf;
}
