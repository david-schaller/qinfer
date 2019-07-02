#include <iostream>

#include "Tree.h"

void
Tree::addChild(std::shared_ptr<TreeNode> parent, std::shared_ptr<TreeNode> child){
  parent->m_children.push_back(std::shared_ptr<TreeNode>(child));
  child->m_parent = parent;
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
