#include "gtest/gtest.h"

#include <vector>
#include <memory>
#include <string>

#include <iostream>

#include "Tree.h"

namespace {

TEST(TreeTest, Basic) {

  auto tn1 = std::make_shared<TreeNode>(std::basic_string("1"));
  auto tree = Tree(tn1);

  auto tn2 = std::make_shared<TreeNode>(std::basic_string("2"));
  auto tn3 = std::make_shared<TreeNode>(std::basic_string("3"));

  tree.addChild(tn1, tn2);
  tree.addChild(tn1, tn3);

  std::cout << tree.toNewick() << std::endl;

  EXPECT_EQ(1, 1);
}

}  // namespace
