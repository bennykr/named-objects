#include "gtest/gtest.h"

#include "named_object.hpp"

#include <iostream>

int main(int argc, char **argv) {
    ::testing::InitGoogleTest(&argc, argv);
    // ::testing::GTEST_FLAG(filter) = "*Simple*";
    return RUN_ALL_TESTS();
}

// unattached object with default name
struct Object : public NamedObject {}; 

struct AttachedObject : public NamedObject {
    static auto constexpr name = "attached_object";

    AttachedObject() : NamedObject("attached_object"){}
};

TEST(NamedObject, RetrievePointer) {
    AttachedObject attached_object;
	ASSERT_EQ(&attached_object, NamedObject::find(AttachedObject::name));
}

TEST(NamedObject, RetrieveNullPointer) {
	ASSERT_DEATH(NamedObject::get(AttachedObject::name), "name missing");

    AttachedObject attached_object;
    attached_object.detach_name();
   	ASSERT_DEATH(NamedObject::get(AttachedObject::name), "name missing");
}

TEST(NamedObject, Attach) {
	Object unnamed_object;

    std::string const name = "new_name";
    unnamed_object.attach_name(name);

    ASSERT_EQ(&unnamed_object, NamedObject::find(name));
}

TEST(NamedObject, SetName){
    AttachedObject attached_object;
    attached_object.set_name("new_name");
    ASSERT_EQ(attached_object.get_name(), "new_name");
}

