#ifndef TREE_H
#define TREE_H

#include <memory>
#include <vector>
#include <string>

class TreeNode {
public:
  friend class Tree;
  TreeNode(std::string value)
    : m_value(value) { };

  inline std::string getValue() const { return m_value; };
  inline bool hasParent() const { return !m_parent.expired(); };
  inline bool hasChildren() const { return !m_children.empty(); };
  inline const std::vector<std::shared_ptr<TreeNode>>& getChildren() const {
    return m_children;
  };

  std::shared_ptr<TreeNode> addChild(std::string value,
                                     std::shared_ptr<TreeNode> parent);

private:
  std::string m_value;
  std::vector<std::shared_ptr<TreeNode>> m_children;
  std::weak_ptr<TreeNode> m_parent;
};


class Tree {
public:
  Tree(std::shared_ptr<TreeNode> root)
    : m_root(root) { };

  void addChild(std::shared_ptr<TreeNode> parent, std::shared_ptr<TreeNode> child);
  std::string toNewick() const;

  static Tree parseNewick(std::string newick);

private:
  std::shared_ptr<TreeNode> m_root;
  std::string constructNewick(std::shared_ptr<TreeNode> node) const;

  static void parseSubtree(std::shared_ptr<TreeNode> subRoot, std::string subtreeString);
  static std::vector<std::string> splitChildren(std::string childString);
};

#endif /* TREE_H */
