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

#include "endstone/permissions/permissions.h"

#include <pybind11/pybind11.h>
#include <pybind11/stl.h>

#include "endstone/permissions/permissible.h"
#include "endstone/permissions/permission_attachment.h"
#include "endstone/permissions/permission_attachment_info.h"
#include "endstone/permissions/permission_default.h"
#include "endstone/permissions/server_operator.h"
#include "endstone/plugin/plugin.h"

namespace py = pybind11;

namespace endstone::detail {

void init_permissions(py::module &m)
{
    py::class_<ServerOperator>(m, "ServerOperator")
        .def_property("op", &ServerOperator::isOp, &ServerOperator::setOp, "The operator status of this object");

    py::enum_<PermissionDefault>(m, "PermissionDefault")
        .value("TRUE", PermissionDefault::True)
        .value("FALSE", PermissionDefault::False)
        .value("OP", PermissionDefault::Operator)
        .value("OPERATOR", PermissionDefault::Operator)
        .value("NOT_OP", PermissionDefault::Operator)
        .value("NOT_OPERATOR", PermissionDefault::NotOperator);

    py::class_<Permissible, ServerOperator>(m, "Permissible")
        .def("is_permission_set", py::overload_cast<std::string>(&Permissible::isPermissionSet, py::const_),
             py::arg("name"), "Checks if a permissions is set by name.")
        .def("is_permission_set", py::overload_cast<const Permission &>(&Permissible::isPermissionSet, py::const_),
             py::arg("perm"), "Checks if a permissions is set by permission.")
        .def("has_permission", py::overload_cast<std::string>(&Permissible::hasPermission, py::const_), py::arg("name"),
             "Checks if a permissions is available by name.")
        .def("has_permission", py::overload_cast<const Permission &>(&Permissible::hasPermission, py::const_),
             py::arg("perm"), "Checks if a permissions is available by permission.")
        .def("add_attachment", py::overload_cast<Plugin &, const std::string &, bool>(&Permissible::addAttachment),
             py::arg("plugin"), py::arg("name"), py::arg("value"), py::return_value_policy::reference,
             "Adds a new PermissionAttachment.")
        .def("add_attachment", py::overload_cast<Plugin &>(&Permissible::addAttachment), py::arg("plugin"),
             py::return_value_policy::reference, "Adds a new PermissionAttachment.")
        .def("remove_attachment", &Permissible::removeAttachment, py::arg("attachment"),
             "Removes a given PermissionAttachment.")
        .def("recalculate_permissions", &Permissible::recalculatePermissions, "Recalculates the permissions.")
        .def_property_readonly("effective_permissions", &Permissible::getEffectivePermissions,
                               py::return_value_policy::reference_internal, "Gets effective permissions.");
}

}  // namespace endstone::detail
