#include "Podcast.h"

std::string Podcast::describe() const {
    return "[Podcast] \"" + title() + "\" ep. " + std::to_string(episode_) +
           " hosted by " + host_ + " (" + formattedDuration() + ")";
}
