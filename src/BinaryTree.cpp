/**
* \file BinaryTree.cpp
* \brief Methods for the class BinaryTree
* \author Guillaume St-Onge
* \version 1.0
* \date 03/02/2018
*/

#include <BinaryTree.hpp>
#include <iostream>
#include <set>

using namespace std;

namespace net
{//start of namespace net


/*---------------------------
 *      Constructor
 *---------------------------*/

/**
* \brief Default constructor of the class BinaryTree
*/
BinaryTree::BinaryTree() : 
    current_node_(nullptr), root_(nullptr), leaves_vector_(), leaves_index_map_()
{
}

/**
* \brief Constructor of the class BinaryTree
* \param[in] N number of leaves
*/
BinaryTree::BinaryTree(unsigned int n_leaves) : 
    current_node_(nullptr), root_(nullptr), leaves_vector_(), leaves_index_map_()
{
    if (n_leaves < 1)
    {
        cout << "Impossible tree" << endl;
    }
    else
    {
        BinaryTreeNode* root = new BinaryTreeNode;
        root_ = root;
        current_node_ = root; //assign current node to the root
        unsigned int n_nodes = 2*n_leaves - 1; //number of nodes for 
                                               //a full binary tree
        root->child_left = branch(root, 1, n_nodes);
        root->child_right = branch(root, 2, n_nodes);
    }
}

/**
* \brief Copy constructor of the class BinaryTree
* \param[in] tree Binary tree to copy its behavior
*/
BinaryTree::BinaryTree(const BinaryTree& tree) :
    current_node_(nullptr), root_(nullptr), leaves_vector_(), leaves_index_map_()
{
    //Construct a new tree with n_leaves
    unsigned int n_leaves = tree.leaves_vector_.size();
    BinaryTreeNode* root = new BinaryTreeNode;
    root_ = root;
    current_node_ = root; //assign current node to the root
    unsigned int n_nodes = 2*n_leaves - 1; //number of nodes for 
                                           //a full binary tree
    root->child_left = branch(root, 1, n_nodes);
    root->child_right = branch(root, 2, n_nodes);

    //Give the same values to the leaves
    LeafIndex leaf_index = 0;
    for (auto iter = tree.leaves_vector_.begin(); 
        iter < tree.leaves_vector_.end(); ++iter)
    {
        update_value(leaf_index, (*iter)->value);
    }
}

/*---------------------------
 *     Assignement
 *---------------------------*/
/**
* \brief Overload the assignement operator 
*/

BinaryTree& BinaryTree::operator=(const BinaryTree& tree)
{
    //destroy the current tree
    this->destroy_tree(root_);

    //Construct a new tree with n_leaves
    unsigned int n_leaves = tree.leaves_vector_.size();
    BinaryTreeNode* root = new BinaryTreeNode;
    root_ = root;
    current_node_ = root; //assign current node to the root
    unsigned int n_nodes = 2*n_leaves - 1; //number of nodes for 
                                           //a full binary tree
    root->child_left = branch(root, 1, n_nodes);
    root->child_right = branch(root, 2, n_nodes);

    //Give the same values to the leaves
    LeafIndex leaf_index = 0;
    for (auto iter = tree.leaves_vector_.begin(); 
        iter < tree.leaves_vector_.end(); ++iter)
    {
        update_value(leaf_index, (*iter)->value);
    }

    return *this;
}

/*---------------------------
 *     Destructor
 *---------------------------*/
/**
* \brief Destructor of the class
*/
BinaryTree::~BinaryTree()
{
    destroy_tree(root_);
}

/*---------------------------
 *     Accessors
 *---------------------------*/
 /**
* \brief Get the leaf index associated to the cumulative fraction r
* \param[in] r real value in [0,1)
*/
LeafIndex BinaryTree::get_leaf_index(double r)
{
    double cumul = 0;
    double total_value = get_value();
    while (not is_leaf())
    {
        if (r <= (cumul + get_value_left())/total_value)
        {
            move_down_left();
        }
        else
        {
            cumul += get_value_left();
            move_down_right();
        }
    }
    LeafIndex chosen_leaf = get_leaf_index();
    reset_current_node();
    return chosen_leaf;
}

/*---------------------------
 *     Mutators
 *---------------------------*/

/**
* \brief Recursive method to construct a full binary tree
* \param[in] parent parent node
* \param[in] node_index index associated to a new node created
* \param[in] n_nodes total number of nodes in the full tree
*/
BinaryTreeNode* BinaryTree::branch(BinaryTreeNode* parent, int node_index, 
    int n_nodes)
{

    BinaryTreeNode* node;
    if(node_index < n_nodes)
    {
        //Branch and create childs if possible
        node = new BinaryTreeNode;
        node->parent = parent;
        node->child_left = branch(node, 2*node_index+1, n_nodes);
        node->child_right = branch(node, 2*node_index+2, n_nodes);
    }
    else
    {
        node = nullptr;
        if (leaves_index_map_.find(parent) == leaves_index_map_.end())
        {
            //the parent node is identified as a leaf
            leaves_index_map_[parent] = leaves_vector_.size();
            leaves_vector_.push_back(parent);
        }
    }

    return node;
}

/**
* \brief Recursively destroy the subtree associated to the node
* \param[in] node Node of the tree
*/
void BinaryTree::destroy_tree(BinaryTreeNode* node)
{
  if(node != nullptr)
  {
    destroy_tree(node->child_left);
    destroy_tree(node->child_right);
    delete node;
  }
}

void BinaryTree::update_value(LeafIndex leaf_index, double variation)
{
    current_node_ = leaves_vector_[leaf_index];
    (current_node_->value) += variation;
    while(not is_root())
    {
        move_up();
        (current_node_->value) += variation;
    }
}

void BinaryTree::update_value(double variation)
{
    if (is_leaf())
    {
        (current_node_->value) += variation;
        while(not is_root())
        {
            move_up();
            (current_node_->value) += variation;
        }
    }
    else
    {
        cout << "not a leaf" << endl;
    }
}

}//end of namespace net