//linked_binary_tree.hpp
#pragma once

#include <iostream>
#include <vector>
#include <queue>
#include <algorithm>

template <typename E>
class LinkedBinaryTree{
    protected:
        class Node {
            public:
            E element;
            Node* parent;
            Node* left{nullptr};
            Node* right{nullptr};
            Node(E e, Node* p = nullptr)
            :element{e}, parent{p}{};
        };// end of Node class

    protected:
        Node* rt{nullptr}; // root
        int sz{0}; // size

    public:
        class Position {
            private:
                Node* node;
                friend class LinkedBinaryTree;
            public:
                // Wraps a Node* pointer 
                //(default is nullptr)
                Position(Node* nd=nullptr): node{nd} {};

                // operator (based on node pointer)
                bool operator==(Position other) const{
                    return node == other.node;
                }
                bool operator!=(Position other) const{
                    return node != other.node;
                }
                bool is_null() const{
                    return node == nullptr;
                }
                bool is_root() const{
                    return node->parent == nullptr;
                }
                bool is_external() const{
                    return node->left == nullptr && node->right == nullptr;
                }
                E& element(){ // get reference to element
                    return node->element;
                }
                const E& element() const{
                    return node->element;
                }

                Position parent() const{
                    return Position(node->parent);
                }
                Position left() const{
                    return Position(node->left);
                }
                Position right() const{
                    return Position(node->right);
                }
                
                std::vector<Position> children() const{
                    std::vector<Position> result;
                    if (node->left != nullptr)
                    {
                        result.push_back(Position(node->left));
                    }
                    if (node->right != nullptr)
                    {
                        result.push_back(Position(node->right));
                    }
                    return result;
                }
                int num_children() const{
                    int result = 0;
                    if (node->left != nullptr)
                    {
                        result++;
                    }
                    if (node->right != nullptr)
                    {
                        result++;
                    }
                    return result;
                }
        };  // end of Position class
        LinkedBinaryTree() = default; //tree with zero nodes
        int size() const{
            return sz;
        }
        bool empty() const{
            return sz == 0;
        }
        Position root() const{
            return Position(rt);
        }
        std::vector<Position> positions() const{
            std::vector<Position> gather;
            if (rt)
            {
                inorder(Position(rt), gather);
            }
            return gather;
        }
        void add_root(const E& e=E()){
            auto new_node = new Node{e, nullptr};
            rt = new_node;
            sz++;
        }
        void add_left(Position p, const E& e){
            auto new_node = new Node(e, p.node);
            p.node->left = new_node;
            sz++;
        }
        void add_right(Position p, const E& e){
            auto new_node = new Node(e, p.node);
            p.node->right = new_node;
            sz++;
        }

        void erase(Position p){
            Node* nd = p.node;
            Node* child{nd->left == nullptr ? nd->right : nd->left};

            if (child != nullptr)
            {
                child->parent = nd->parent;
            }

            if (nd == rt)
            {
                rt = child;
            }
            else if (nd->parent->left == nd)
            {
                nd->parent->left = child;
            }
            else
            {
                nd->parent->right = child;
            }
            sz--;
            delete nd;
        }

        void attach(Position p, LinkedBinaryTree& left, LinkedBinaryTree& right){
            Node* nd = p.node;

            nd->left = left.rt;
            nd->right = right.rt;
            sz += left.sz + right.sz;

            if (left.rt)
            {
                left.rt->parent = nd;
            }
            if (right.rt)
            {
                right.rt->parent = nd;
            }

            left.rt = right.rt = nullptr;
            left.sz = right.sz = 0;
        }
    // ------------- Rule-of-five support ----------------
    private:
        void tear_down(Node* nd){
            if (nd != nullptr)
            {
                tear_down(nd->left);
                tear_down(nd->right);
                delete nd;
            }
        }
        static Node* clone(Node* model){
            if (model == nullptr)
            {
                return nullptr;
            }

            Node* new_root = new Node(model->element);

            new_root->left = clone(model->left);
            if (new_root->left)
            {
                new_root->left->parent = new_root;
            }

            new_root->right = clone(model->right);
            if (new_root->right)
            {
                new_root->right->parent = new_root;
            }

            return new_root;
        }

    public:
        ~LinkedBinaryTree(){
            tear_down(rt);
        }
        
        // copy constructor and copy assignment
        LinkedBinaryTree(const LinkedBinaryTree& other)
        :sz{other.sz}, rt{clone(other.rt)}{}

        LinkedBinaryTree& operator=(const LinkedBinaryTree& other){
            if (this != &other)
            {
                tear_down(rt);
                rt = clone(other.rt);
                sz = other.sz;
            }
            return *this;
        }

        // move constructor and move assignment
        LinkedBinaryTree(LinkedBinaryTree&& other)
        :sz{other.sz}, rt{other.rt} {
            other.sz = 0;
            other.rt = nullptr;
        }

        LinkedBinaryTree& operator=(LinkedBinaryTree&& other){
            if (this != &other)
            {
                std::swap(sz, other.sz);
                std::swap(rt, other.rt);
            }
            return *this;
        }

    // ******************* Assignment ******************* 
    private:
        int recursion(Position p) const
        {
            if (p.is_null())
            {
                return 0;
            }
            int count = 0;
            if (p.is_external() && p.node->parent != nullptr && p.node == p.node->parent->left)
            {
                count++;
            }
            if (p.node->left)
            {
                count += recursion(Position(p.node->left));
            }
            if (p.node->right)
            {
                count += recursion(Position(p.node->right));
            }
            return count;
        }

        int breadthfirst(Position p, std::queue<Position> visited) const {
            int count = 0;
            if (!p.is_null()) {
                std::queue<Position> fringe;
                fringe.push(p);
                while (!fringe.empty()) {
                    Position curr = fringe.front();
                    fringe.pop();
                    visited.push(curr);
                    if (curr.is_external() && curr.node->parent != nullptr && curr.node == curr.node->parent->left)
                    {
                        count++;
                    }
                    for (Position c : curr.children())
                    {
                        fringe.push(c);
                    }
                }
            }
            return count;
        }

    public:
        int count_left_leaves() const{
            // ToDo (use recursion)
            //count nodes that are leaves in a binary tree and that are the left child of their respective parent. The root is not counted.
            Position p = Position(rt);
            return recursion(p);
        }

        int count_left_leaves_bfs() const {
            // ToDo (use breadth first search)
            //count nodes that are leaves in a binary tree and that are the left child of their respective parent. The root is not counted.
            Position p = Position(rt);
            std::queue<Position> visited;
            return breadthfirst(p, visited);
        }
};