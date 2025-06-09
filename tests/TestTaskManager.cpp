#include "gtest/gtest.h"
#include "TaskManager.hpp"

TEST(TaskManagerTest, AddAndList) {
    TaskManager mgr;
    int id1 = mgr.addTask("First");
    int id2 = mgr.addTask("Second", "2025-08-01");
    auto all = mgr.listTasks();
    EXPECT_EQ(all.size(), 2);
    EXPECT_EQ(all[0].getId(), id1);
    EXPECT_EQ(all[1].getId(), id2);
}

TEST(TaskManagerTest, RemoveTask) {
    TaskManager mgr;
    int id1 = mgr.addTask("ToRemove");
    mgr.removeTask(id1);
    auto all = mgr.listTasks();
    EXPECT_TRUE(all.empty());
    // Негативный: удалить несуществующий
    EXPECT_THROW(mgr.removeTask(999), std::runtime_error);
}

TEST(TaskManagerTest, MarkDoneAndFilter) {
    TaskManager mgr;
    int id1 = mgr.addTask("A");
    int id2 = mgr.addTask("B");
    mgr.markDone(id1);
    auto doneList = mgr.listTasks(true);
    EXPECT_EQ(doneList.size(), 1);
    EXPECT_EQ(doneList[0].getId(), id1);
    auto pendingList = mgr.listTasks(false);
    EXPECT_EQ(pendingList.size(), 1);
    EXPECT_EQ(pendingList[0].getId(), id2);
    EXPECT_THROW(mgr.markDone(999), std::runtime_error);
}

TEST(TaskManagerTest, SearchByDescription) {
    TaskManager mgr;
    mgr.addTask("Buy milk");
    mgr.addTask("Read book");
    mgr.addTask("Buy bread");
    auto found = mgr.searchByDescription("buy");
    EXPECT_EQ(found.size(), 2);
}

TEST(TaskManagerTest, UpdateDueDate) {
    TaskManager mgr;
    int id1 = mgr.addTask("With date", "2025-09-01");
    mgr.updateDueDate(id1, "2025-10-01");
    auto all = mgr.listTasks();
    EXPECT_EQ(all[0].getDueDate().value(), "2025-10-01");
    EXPECT_THROW(mgr.updateDueDate(999, "2025-01-01"), std::runtime_error);
}

TEST(TaskManagerTest, TagsOperations) {
    TaskManager mgr;
    int id = mgr.addTask("Tag test");
    mgr.addTag(id, "home");
    mgr.addTag(id, "work");
    auto all = mgr.listTasks();
    EXPECT_EQ(all[0].getTags().size(), 2);
    mgr.removeTag(id, "home");
    auto all2 = mgr.listTasks();
    EXPECT_EQ(all2[0].getTags().size(), 1);
}
