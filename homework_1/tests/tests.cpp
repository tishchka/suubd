// Copyright 2021 Oganes Mirzoyan mirzoyan.oganes@mail.ru

#include <gtest/gtest.h>

#include <csv.hpp>
#include <cli.hpp>

#include <fstream>
#include <iostream>
#include <string>
#include <vector>

TEST(CSV, Write) {
  CSV csv;
  std::vector<std::string> v = {"Andrew", "2", "MIT", "Algebra", "5", "Calculus", "5"};
  for (const auto& x : v) {
    csv.Add(x);
  }
  EXPECT_EQ(csv.Write("test.txt"), true);
}

TEST(CSV, Read) {
  CSV csv;
  std::vector<std::vector<std::string>> v = {{"Andrew", "2", "MIT", "Algebra", "5", "Calculus", "5"},
                                             {"Richard", "5", "ICS", "Algebra", "5", "Calculus", "5", "Analysis", "4"}};

  for (const auto& x : v) {
    for (const auto& xx : x) {
      csv.Add(xx);
    }
    csv.NewRow();
  }

  EXPECT_EQ(csv.Write("test.txt"), true);

  size_t i = 0;
  std::ifstream input("test.txt");
  while (csv.Parse(input)) {
    EXPECT_EQ(csv.Amount(), v[i].size());
    ++i;
  }
}

TEST(CLI, test) {
  CLI cli;
  cli("tesst", "tessst");
}
