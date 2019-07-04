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

  std::string newick = tree.toNewick();

  std::cout << newick << std::endl;

  newick = "(A,B,((C,E),D));"; //"(A:0.1,B:0.2,(C:0.3,D:0.4):0.5);";

  Tree tree2 = Tree::parseNewick(newick);
  std::string newick2 = tree2.toNewick();
  std::cout << newick2 << std::endl;

  EXPECT_EQ(newick, newick2);
}

}  // namespace
