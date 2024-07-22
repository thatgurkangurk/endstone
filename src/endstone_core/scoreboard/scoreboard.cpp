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

#include "endstone/detail/scoreboard/scoreboard.h"

#include <stdexcept>

#include "bedrock/world/scores/objective_criteria.h"
#include "bedrock/world/scores/scoreboard.h"
#include "endstone/detail/actor/actor.h"
#include "endstone/detail/player.h"
#include "endstone/detail/scoreboard/objective.h"
#include "endstone/detail/scoreboard/score.h"
#include "endstone/detail/server.h"

namespace endstone::detail {

EndstoneScoreboard::EndstoneScoreboard(::Scoreboard &board) : board_(board) {}

std::unique_ptr<Objective> EndstoneScoreboard::addObjective(std::string name, Criteria::Type criteria)
{
    return addObjective(name, criteria, name);
}

std::unique_ptr<Objective> EndstoneScoreboard::addObjective(std::string name, Criteria::Type criteria,
                                                            std::string display_name)
{
    return addObjective(name, criteria, name, RenderType::Integer);
}

std::unique_ptr<Objective> EndstoneScoreboard::addObjective(std::string name, Criteria::Type criteria,
                                                            std::string display_name, RenderType render_type)
{
    auto &server = entt::locator<EndstoneServer>::value();
    auto *cr = board_.getCriteria(getCriteriaName(criteria));
    if (!cr) {
        server.getLogger().error("Unknown criteria: {}.", getCriteriaName(criteria));
        return nullptr;
    }

    auto *objective = board_.addObjective(name, display_name, *cr);
    if (!objective) {
        server.getLogger().error("Objective {} already exists.", name);
        return nullptr;
    }

    return std::make_unique<EndstoneObjective>(*this, *objective);
}

std::unique_ptr<Objective> EndstoneScoreboard::getObjective(std::string name) const
{
    if (auto *objective = board_.getObjective(name); objective) {
        return std::make_unique<EndstoneObjective>(const_cast<EndstoneScoreboard &>(*this), *objective);
    }
    return nullptr;
}

std::unique_ptr<Objective> EndstoneScoreboard::getObjective(DisplaySlot slot) const
{
    const auto *display_objective = board_.getDisplayObjective(getDisplaySlotName(slot));
    if (!display_objective) {
        return nullptr;
    }
    return std::make_unique<EndstoneObjective>(const_cast<EndstoneScoreboard &>(*this),
                                               const_cast<::Objective &>(display_objective->getObjective()));
}

std::vector<std::unique_ptr<Objective>> EndstoneScoreboard::getObjectives() const
{
    std::vector<std::unique_ptr<Objective>> result;
    board_.forEachObjective([&](auto &objective) {
        result.push_back(std::make_unique<EndstoneObjective>(const_cast<EndstoneScoreboard &>(*this), objective));
    });
    return result;
}

std::vector<std::unique_ptr<Objective>> EndstoneScoreboard::getObjectivesByCriteria(Criteria::Type criteria) const
{
    std::vector<std::unique_ptr<Objective>> result;
    board_.forEachObjective([&](auto &objective) {
        if (objective.getCriteria().getName() == EndstoneScoreboard::getCriteriaName(criteria)) {
            result.push_back(std::make_unique<EndstoneObjective>(const_cast<EndstoneScoreboard &>(*this), objective));
        }
    });
    return result;
}

std::vector<std::unique_ptr<Score>> EndstoneScoreboard::getScores(ScoreEntry entry) const
{
    std::vector<std::unique_ptr<Score>> result;
    board_.forEachObjective([&](auto &objective) {
        auto obj = std::make_unique<EndstoneObjective>(const_cast<EndstoneScoreboard &>(*this), objective);
        result.push_back(std::make_unique<EndstoneScore>(std::move(obj), entry));
    });
    return result;
}

void EndstoneScoreboard::resetScores(ScoreEntry entry)
{
    throw std::runtime_error("Not implemented.");
}

std::vector<ScoreEntry> EndstoneScoreboard::getEntries() const
{
    throw std::runtime_error("Not implemented.");
}

void EndstoneScoreboard::clearSlot(DisplaySlot slot)
{
    board_.clearDisplayObjective(getDisplaySlotName(slot));
}

std::string EndstoneScoreboard::getCriteriaName(Criteria::Type type)
{
    switch (type) {
    case Criteria::Type::Dummy:
        return "dummy";
    default:
        throw std::runtime_error("Unknown Criteria::Type!");
    }
}

std::string EndstoneScoreboard::getDisplaySlotName(DisplaySlot slot)
{
    switch (slot) {
    case DisplaySlot::BelowName:
        return "belowname";
    case DisplaySlot::PlayerList:
        return "list";
    case DisplaySlot::SideBar:
        return "sidebar";
    default:
        throw std::runtime_error("Unknown DisplaySlot!");
    }
}

const ::ScoreboardId &EndstoneScoreboard::getScoreboardId(ScoreEntry entry)
{
    return std::visit(
        entt::overloaded{[&](Player *player) -> const ::ScoreboardId & {
                             return board_.getScoreboardId(static_cast<EndstonePlayer *>(player)->getHandle());
                         },
                         [&](Actor *actor) -> const ::ScoreboardId & {
                             return board_.getScoreboardId(static_cast<EndstoneActor *>(actor)->getActor());
                         },
                         [&](const std::string &fake) -> const ::ScoreboardId & {
                             return board_.getScoreboardId(fake);
                         }},
        entry);
}

}  // namespace endstone::detail
