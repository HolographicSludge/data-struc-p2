#include "Song.h"

std::string Song::describe() const {
    return "[Song] \"" + title() + "\" by " + artist_ + " (" + formattedDuration() + ")";
}
