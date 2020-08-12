#include "gtest/gtest.h"

#include "named_object.hpp"

#include <iostream>

int main(int argc, char **argv) {
    ::testing::InitGoogleTest(&argc, argv);
    // ::testing::GTEST_FLAG(filter) = "*Simple*";
    return RUN_ALL_TESTS();
}

struct AttachedObject : public NamedObject {
    static auto constexpr default_name = "default_object_name";

    AttachedObject() : NamedObject(default_name){}
    AttachedObject(std::string const & name) : NamedObject(name){}
};

TEST(NamedObject, RetrieveNullPointer) {
	ASSERT_EQ(NamedObject::find(AttachedObject::default_name), nullptr);
}

TEST(NamedObject, RetrievePointer) {
    AttachedObject attached_object;
	ASSERT_EQ(NamedObject::find(AttachedObject::default_name), &attached_object);
}

TEST(NamedObject, GetObjectReference) {
    AttachedObject attached_object;
	ASSERT_EQ(&NamedObject::get(AttachedObject::default_name), &attached_object);
}

TEST(NamedObject, GetName) {
    AttachedObject attached_object;
	ASSERT_EQ(attached_object.get_name(), AttachedObject::default_name);
}

TEST(NamedObject, ResetName) {
    auto constexpr new_name = "new_name";

    AttachedObject attached_object;
    attached_object.reset_name(new_name);

    ASSERT_DEATH(NamedObject::get(AttachedObject::default_name), "name missing");
	ASSERT_EQ(&NamedObject::get(new_name), &attached_object);
}

TEST(NamedObject, DeleteObject) {
    {   
        AttachedObject attached_object;
	    ASSERT_EQ(NamedObject::find(AttachedObject::default_name), &attached_object);
    }
    ASSERT_EQ(NamedObject::find(AttachedObject::default_name), nullptr);
}

TEST(NamedObject, MoveAssignment) {
    AttachedObject object_1("1"), object_2("2");

	ASSERT_EQ(NamedObject::find("1"), &object_1);
   	ASSERT_EQ(NamedObject::find("2"), &object_2);

    object_1 = std::move(object_2);
	ASSERT_EQ(NamedObject::find("1"), nullptr);
   	ASSERT_EQ(NamedObject::find("2"), &object_1);
}

TEST(NamedObject, MoveConstructor) {
    AttachedObject object_1("1");
	ASSERT_EQ(NamedObject::find("1"), &object_1);

    AttachedObject object_2(std::move(object_1));
	ASSERT_EQ(NamedObject::find("1"), &object_2);
}