/**
* \file BinaryTree.hpp
* \brief Header file for class BinaryTree
* \author Guillaume St-Onge
* \version 1.0
* \date 27/01/2018
*/

#ifndef BINARYTREE_HPP
#define BINARYTREE_HPP

#include <vector>
#include <unordered_map>
#include <cmath>

namespace net
{//start of namespace net

//Define new types
typedef unsigned int LeafIndex;

//Define a structure for the nodes of a binary tree
struct BinaryTreeNode
{
    BinaryTreeNode() : value(0.), parent(nullptr), child_left(nullptr),
    child_right(nullptr) {}
    double value;
    BinaryTreeNode* parent;
    BinaryTreeNode* child_left;
    BinaryTreeNode* child_right;
};

/**
* \class BinaryTree BinaryTree.hpp
* \brief Binary tree implementation where each node possess a value equal
* to the sum of the value of its children.
*/
class BinaryTree
{
public:
    //Constructor
    BinaryTree();
    BinaryTree(unsigned int n_leaves);
    BinaryTree(const BinaryTree& tree);

    //Assignement operator
    BinaryTree& operator=(const BinaryTree& tree);

    //Destructor
    ~BinaryTree();

    //Accessors
    bool is_root()
        {return current_node_->parent == nullptr;}
    bool is_leaf()
        {return current_node_->child_left == nullptr and 
            current_node_->child_right == nullptr ;}
    double get_value() const
        {return current_node_->value;}
    double get_value_left() const
        {return (current_node_->child_left)->value;}
    double get_value_right() const
        {return (current_node_->child_right)->value;}
    LeafIndex get_leaf_index() const
        {return leaves_index_map_.at(current_node_);}
    LeafIndex get_leaf_index(double r);

    //Mutators
    void reset_current_node()
        {current_node_ = root_;}
    void move_down_left()
        {current_node_ = current_node_->child_left;}
    void move_down_right()
        {current_node_ = current_node_->child_right;}
    void move_up()
        {current_node_ = current_node_->parent;}
    void update_value(LeafIndex leaf_index, double variation);
    void update_value(double variation);


private:
    //Members
    BinaryTreeNode* root_;
    BinaryTreeNode* current_node_;
    std::vector<BinaryTreeNode*> leaves_vector_;
    std::unordered_map<BinaryTreeNode*,LeafIndex> leaves_index_map_;

    //To be called by constructors and assignement operator
    BinaryTreeNode* branch(BinaryTreeNode* parent, int node_index, 
    int n_nodes);

    //to be called by destructor
    void destroy_tree(BinaryTreeNode* node);

};


}//end of namespace net

#endif /* BINARYTREE_HPP */