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

#include <cpptrace/cpptrace.hpp>

#include "endstone/detail/os.h"
#include "endstone/endstone.h"

namespace endstone::detail {

inline void print_crash_dump(const std::string &message, std::size_t skip = 0)
{
    printf("=== ENDSTONE CRASHED! - PLEASE REPORT THIS AS AN ISSUE ON GITHUB ===\n");
    printf("Os: %s\n", os::get_name().c_str());
    printf("Endstone version: %s\n", ENDSTONE_VERSION);
    printf("Api version: %s\n", ENDSTONE_API_VERSION);
    printf("Description: %s\n", message.c_str());
    cpptrace::generate_trace(skip + 1).print_with_snippets();
}

void register_signal_handler();

}  // namespace endstone::detail
