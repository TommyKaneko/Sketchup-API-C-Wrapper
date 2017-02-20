//
//  Group.cpp
//
//  Sketchup C++ Wrapper for C API
//  Copyright (C) 2016  Hidetomo (Tom) Kaneko
//
//  This program is free software: you can redistribute it and/or modify
//  it under the terms of the GNU General Public License as published by
//  the Free Software Foundation, either version 3 of the License, or
//  (at your option) any later version.
//
//  This program is distributed in the hope that it will be useful,
//  but WITHOUT ANY WARRANTY; without even the implied warranty of
//  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
//  GNU General Public License for more details.
//
//  You should have received a copy of the GNU General Public License
//  along with this program.  If not, see <http://www.gnu.org/licenses/>.
//

#include "Group.hpp"

#include <cassert>

#include "String.hpp"
#include "ComponentDefinition.hpp"
#include "Entities.hpp"
#include "Transformation.hpp"


namespace CW {


SUGroupRef Group::create_group() {
	SUGroupRef group_ref = SU_INVALID;
  SU_RESULT res = SUGroupCreate(&group_ref);
  assert(res == SU_ERROR_NONE);
  return group_ref;
}


Group::Group():
	Group(create_group())
{}


Group::Group(SUGroupRef group):
	m_group(group),
  Entity(SUGroupToEntity(group))
{}


Group::operator SUGroupRef() const {
	return ref();
}


Group::operator SUGroupRef*() {
	return &m_group;
}


ComponentDefinition Group::definition() const {
  SUComponentDefinitionRef def_ref = SU_INVALID;
	SU_RESULT res = SUGroupGetDefinition(m_group, &def_ref);
  assert(res == SU_ERROR_NONE);
  return ComponentDefinition(def_ref);
}


Entities Group::entities() const {
	SUEntitiesRef entities = SU_INVALID;
  SUGroupGetEntities(m_group, &entities);
  return Entities(entities);
}


String Group::name() const {
	String string;
  SUStringRef * const string_ref = string;
	SU_RESULT res = SUGroupGetName(m_group, string_ref);
  assert(res == SU_ERROR_NONE);
  return string;
}


void Group::name(const String& string) {
	std::string name_string = string.std_string();
	SU_RESULT res = SUGroupSetName(m_group, name_string.c_str());
  assert(res == SU_ERROR_NONE);
}

Transformation Group::transformation() const {
	SUTransformation transform{};
	SUGroupGetTransform(m_group, &transform);
  return Transformation(transform);
}


void Group::transformation(const Transformation& transform) {
	SUTransformation transform_ref = transform.ref();
  SU_RESULT res = SUGroupSetTransform(m_group, &transform_ref);
  assert(res == SU_ERROR_NONE);
}


SUGroupRef Group::ref() const {
	return m_group;
}


} /* namespace CW */
