#include "gtest/gtest.h"
#include "CLIParser.hpp"

TEST(CLIParserTest, ParseAdd) {
    const char* argv[] = {"prog", "add", "Task1", "--due=2025-05-05", "--tags=home,work", "--data-file=tasks.json", "--store-format=json"};
    int argc = 6;
    auto opts = CLIParser::parse(argc, const_cast<char**>(argv));
    EXPECT_EQ(opts.command, "add");
    EXPECT_EQ(opts.args.at("description"), "Task1");
    EXPECT_EQ(opts.args.at("due"), "2025-05-05");
    EXPECT_EQ(opts.args.at("tags"), "home,work");
    EXPECT_EQ(opts.dataFilePath, "tasks.json");
    EXPECT_EQ(opts.format, "json");
}

TEST(CLIParserTest, ParseRemove) {
    const char* argv[] = {"prog", "remove", "123", "--data-file=my.json"};
    int argc = 4;
    auto opts = CLIParser::parse(argc, const_cast<char**>(argv));
    EXPECT_EQ(opts.command, "remove");
    EXPECT_EQ(opts.args.at("id"), "123");
    EXPECT_EQ(opts.dataFilePath, "my.json");
}

TEST(CLIParserTest, ParseListDefault) {
    const char* argv[] = {"prog", "list"};
    int argc = 2;
    auto opts = CLIParser::parse(argc, const_cast<char**>(argv));
    EXPECT_EQ(opts.command, "list");
    EXPECT_EQ(opts.dataFilePath, "tasks.json");
    EXPECT_EQ(opts.format, "json");
}
