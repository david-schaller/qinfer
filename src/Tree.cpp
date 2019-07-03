#include <iostream>
#include <stdexcept>

#include "Tree.h"

void
Tree::addChild(std::shared_ptr<TreeNode> parent, std::shared_ptr<TreeNode> child){
  parent->m_children.push_back(std::shared_ptr<TreeNode>(child));
  child->m_parent = parent->m_children.back();
}

std::string
Tree::toNewick() const {
  return constructNewick(m_root) + ";";
}

std::string
Tree::constructNewick(std::shared_ptr<TreeNode> node) const {
  if(!node->hasChildren()){
    return node->getValue();
  } else {
    std::string s = "(";
    for(auto child : node->getChildren()){
      s += constructNewick(child) + ",";
    }
    return s.substr(0, s.size()-1) + ")" + node->getValue();
  }
}

Tree
Tree::parseNewick(std::string newick){
  std::cout << "parseNewick" << std::endl;

  if(newick.find(';') != std::string::npos){
    newick.erase(newick.find(';'), newick.size());
  }

  auto tempRoot = std::make_shared<TreeNode>(std::basic_string("-1"));

  parseSubtree(tempRoot, newick);
}

void
Tree::parseSubtree(std::shared_ptr<TreeNode> subRoot, std::string subtreeString){
  auto children = splitChildren(subtreeString);
}

std::vector<std::string>
Tree::splitChildren(std::string childString){
  auto children = std::vector<std::string>();
  int stack = 0;
  std::string current = "";

  for(char c : childString){
    if((stack == 0) && (c == ',')){
      children.push_back(current);
      current.clear();
    } else if(c == '('){
      ++stack;
      current.append(1, c);
    } else if(c == ')'){
      if(stack <= 0){
        throw std::invalid_argument( "invalid Newick string!" );
      }
      --stack;
      current.append(1, c);
    } else {
      current.append(1, c);
    }
  }
  children.push_back(current);

  return children;
}
