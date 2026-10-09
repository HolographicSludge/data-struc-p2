#pragma once

#include "MediaItem.h"

class Podcast : public MediaItem {
public:
    Podcast(std::string title, std::string host, int episode, int durationSeconds)
        : MediaItem(std::move(title), durationSeconds),
          host_(std::move(host)), episode_(episode) {}

    std::string describe() const override;

private:
    std::string host_;
    int episode_;
};
