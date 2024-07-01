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

#include <nlohmann/json.hpp>
namespace endstone::detail::devtools {

struct VanillaData {
    nlohmann::json block_types;
    nlohmann::json block_states;
    nlohmann::json block_tags;
    nlohmann::json materials;
    nlohmann::json items;
    nlohmann::json item_tags;
    nlohmann::json creative_items;

    static VanillaData *get();
};

}  // namespace endstone::detail::devtools
