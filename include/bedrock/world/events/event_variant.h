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

#include <type_traits>
#include <variant>

namespace Details {
template <typename T>
class ValueOrRef {
    union Variant {
        const T *pointer;
        const T value;
        Variant() {}
        ~Variant() {}
    };

    explicit ValueOrRef(T value) : is_pointer_(false)
    {
        variant_.value = value;
    }

public:
    ~ValueOrRef()
    {
        if (!is_pointer_) {
            variant_.value.~T();
        }
    }

    T &value() const noexcept
    {
        return is_pointer_ ? *variant_.pointer : variant_.value;
    }

private:
    Variant variant_;
    const bool is_pointer_;
};
}  // namespace Details

template <typename... Events>
struct EventVariantImpl {
    std::variant<Details::ValueOrRef<Events>...> variant;
};

template <typename... Events>
struct ConstEventVariant {
    EventVariantImpl<std::add_const_t<Events>...> variant;
};

template <typename... Events>
struct MutableEventVariant {
    EventVariantImpl<Events...> variant;
};
