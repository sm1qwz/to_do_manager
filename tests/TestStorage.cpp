#include "gtest/gtest.h"
#include "Storage.hpp"
#include <filesystem>

namespace fs = std::filesystem;

TEST(StorageTest, JsonSaveLoad) {
    std::string tmp = "test_tasks.json";
    {
        // Создадим и сохраним 2 задачи
        std::vector<Task> tasks;
        Task t1("One");
        Task t2("Two", "2025-12-12");
        tasks.push_back(t1);
        tasks.push_back(t2);

        Storage st(tmp, "json");
        st.save(tasks);
    }
    {
        Storage st(tmp, "json");
        auto loaded = st.load();
        EXPECT_EQ(loaded.size(), 2);
        EXPECT_EQ(loaded[0].getDescription(), "One");
        EXPECT_TRUE(loaded[1].getDueDate().has_value());
    }
    fs::remove(tmp);
}

TEST(StorageTest, LoadNonexistentFileJson) {
    std::string nonexistent = "no_file.json";
    Storage st(nonexistent, "json");
    auto loaded = st.load();
    EXPECT_TRUE(loaded.empty());
}

TEST(StorageTest, SQLiteSaveLoad) {
    std::string tmpdb = "test_tasks.db";
    {
        std::vector<Task> tasks;
        Task t1("X");
        Task t2("Y", "2025-11-11");
        tasks.push_back(t1);
        tasks.push_back(t2);

        Storage st(tmpdb, "sqlite");
        st.save(tasks);
    }
    {
        Storage st(tmpdb, "sqlite");
        auto loaded = st.load();
        EXPECT_EQ(loaded.size(), 2);
        EXPECT_EQ(loaded[1].getDescription(), "Y");
    }
    fs::remove(tmpdb);
}
