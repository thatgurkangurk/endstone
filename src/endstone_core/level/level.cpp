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

#include "endstone/detail/level/level.h"

#include <entt/entt.hpp>

#include "endstone/detail/virtual_table.h"

namespace endstone::detail {

EndstoneLevel::EndstoneLevel(BedrockLevel &level) : level_(level) {}

std::string EndstoneLevel::getName() const
{
    return level_.getLevelId();
}

int EndstoneLevel::getTime() const
{
    return level_.getTime();
}

void EndstoneLevel::setTime(int time)
{
    level_.setTime(time);
}

namespace {
template <typename T, typename Any>
void hookEventHandler(Any &handler, std::size_t vmt_size)
{
    auto &target = static_cast<T &>(handler);
    auto &hook = entt::locator<VirtualTableHook<T>>::value_or(target, vmt_size);
    hook.install(target);
}
}  // namespace

void EndstoneLevel::hookEventHandlers()
{
    hookEventHandler<ScriptLevelGameplayHandler>(level_.getLevelEventCoordinator().getLevelGameplayHandler(),
                                                 _WIN32_LINUX_(11, 12));
}

};  // namespace endstone::detail
