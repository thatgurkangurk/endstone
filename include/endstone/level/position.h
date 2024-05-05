// Copyright (c) 2024, The Endstone Project. (https://endstone.dev) All Rights Reserved.
//
// Licensed under the Apache License, Version 2.0 (the "License");
// you may not use this file except in compliance with the License.
// You may obtain a copy of the License at
//
//     http://www.apache.org/licenses/LICENSE-2.0
//
// Unless required by applicable law or agreed to in writing, software
// distributed under the License is distributed on an "AS IS" BASIS,
// WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
// See the License for the specific language governing permissions and
// limitations under the License.

#pragma once

#include <memory>
#include <optional>

#include "endstone/util/vector.h"

namespace endstone {

class Level;
class Dimension;

/**
 * @brief Represents a 3-dimensional position in a dimension within a level.
 */
class Position : public Vector<double> {
public:
    Position(const std::shared_ptr<Level> &level, const std::shared_ptr<Dimension> &dimension, double x, double y,
             double z)
        : Vector(x, y, z)
    {
        if (level) {
            level_ = level;
        }
        if (dimension) {
            dimension_ = dimension;
        }
    }

    /**
     * Gets the level that this position resides in
     *
     * @return Level that contains this position
     */
    [[nodiscard]] std::shared_ptr<Level> getLevel() const
    {
        if (!level_.has_value()) {
            return nullptr;
        }

        if (level_.value().expired()) {
            throw std::runtime_error("Level unloaded");
        }
        return level_.value().lock();
    }

    /**
     * Sets the level that this position resides in
     *
     * @param level New level that this position resides in
     */
    void setLevel(const std::shared_ptr<Level> &level)
    {
        level_ = level;
    }

    /**
     * Gets the dimension that this position resides in
     *
     * @return Dimension that contains this position
     */
    [[nodiscard]] std::shared_ptr<Dimension> getDimension() const
    {
        if (!dimension_.has_value()) {
            return nullptr;
        }

        if (dimension_.value().expired()) {
            throw std::runtime_error("Dimension unloaded");
        }
        return dimension_.value().lock();
    }

    /**
     * Sets the dimension that this position resides in
     *
     * @param dimension New dimension that this position resides in
     */
    void setDimension(const std::shared_ptr<Dimension> &dimension)
    {
        dimension_ = dimension;
    }

private:
    std::optional<std::weak_ptr<Level>> level_;
    std::optional<std::weak_ptr<Dimension>> dimension_;
};

}  // namespace endstone
