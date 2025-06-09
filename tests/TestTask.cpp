#include "gtest/gtest.h"
#include "Task.hpp"

TEST(TaskTest, ConstructorAndGetters) {
    Task t("Test description");
    EXPECT_EQ(t.getDescription(), "Test description");
    EXPECT_FALSE(t.getDueDate().has_value());
    EXPECT_FALSE(t.isDone());
}

TEST(TaskTest, MarkDone) {
    Task t("Do something");
    t.markDone();
    EXPECT_TRUE(t.isDone());
}

TEST(TaskTest, SetDueDate) {
    Task t("Task with date");
    t.setDueDate("2025-06-10");
    ASSERT_TRUE(t.getDueDate().has_value());
    EXPECT_EQ(t.getDueDate().value(), "2025-06-10");
}

TEST(TaskTest, TagsManipulation) {
    Task t("Tag test");
    t.addTag("work");
    t.addTag("urgent");
    ASSERT_EQ(t.getTags().size(), 2);
    t.removeTag("work");
    ASSERT_EQ(t.getTags().size(), 1);
    EXPECT_EQ(t.getTags()[0], "urgent");
}

TEST(TaskTest, JsonSerialization) {
    Task t("Serialize me", "2025-07-01", {"a","b"});
    t.markDone();
    auto j = t.toJson();
    EXPECT_EQ(j["description"], "Serialize me");
    EXPECT_EQ(j["dueDate"], "2025-07-01");
    EXPECT_TRUE(j["done"]);
    ASSERT_TRUE(j.contains("tags"));
    EXPECT_EQ(j["tags"].size(), 2);

    Task t2 = Task::fromJson(j);
    EXPECT_EQ(t2.getDescription(), "Serialize me");
    EXPECT_EQ(t2.getDueDate().value(), "2025-07-01");
    EXPECT_TRUE(t2.isDone());
    EXPECT_EQ(t2.getTags().size(), 2);
}
