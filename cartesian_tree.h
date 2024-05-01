//
// Created by Kinstintin on 28.04.2024.
//

#ifndef FAST_CARTESIAN_TREE
#define FAST_CARTESIAN_TREE
#include <random>
#include <unordered_map>

std::uniform_int_distribution<int> uniform(1, INT32_MAX);
std::random_device dev;
std::mt19937_64 rng(dev());


template <typename TKey,
        typename Compare = std::less<TKey>>
class cartesian_tree{
private:
    struct Node{
        Node* parent;
        Node* minimum;
        Node* left;
        Node* right;
        TKey key;
        int prior;
        int count;
        explicit Node(const TKey &value): key(value){
            count = 1;
            left = nullptr;
            right = nullptr;
            minimum = this;
            parent = nullptr;
            prior = uniform(rng);
        }
    };
    void renew(Node* &node){
        if (!node){
            return;
        }

        node->minimum = node->left != nullptr ? node->left->minimum : node;
        if (node -> left){
            node -> left -> parent = node;
        }
        if (node -> right){
            node -> right -> parent = node;
        }
    }
    std::tuple <Node*, Node*> split(Node *tree,const TKey &key){
        Node *left = nullptr, *right = nullptr;
        if (!tree){
            return std::make_tuple(left, right);
        }
        auto cmp = Compare();
        if (cmp(tree -> key, key)) { // tree->key < key
            left = tree;
            std::tie(left -> right, right) = split(tree->right, key);
        } else{ // tree->key > key
            right = tree;
            std::tie(left, right->left) = split(tree -> left, key);
        }
        renew(left);
        renew(right);
        if (left){
            left -> parent = nullptr;
        }
        if (right){
            right -> parent = nullptr;
        }
        return std::make_tuple(left, right);
    }
    Node* merge(Node *tree1, Node *tree2){
        if (!tree1){
            return tree2;
        }
        if (!tree2){
            return tree1;
        }

        if (tree1 -> prior > tree2 -> prior){
            tree1 -> right = merge(tree1 -> right, tree2);
            renew(tree1);
            return tree1;
        }
        tree2 -> left = merge(tree1, tree2 -> left);
        renew(tree2);
        return tree2;
    }
    void push_min(Node *node){
        if  (!node){
            return;
        }
        node-> minimum = node -> left -> minimum;
        push_min(node -> parent);
    };
    void replace_with_right(Node *node){ // consider right exists

        if (node -> parent){
            node -> parent -> left = node -> right;
            node -> right -> parent = node -> parent;
        } else{
            root = node -> right;
            root -> parent = nullptr;
        }
    }
    void dealloc_entity(Node *node){
        table.erase(node->key);
        delete node;
    }
    void update_min(Node *min_node){
        for(;;) {
            if (min_node->count > 0) {
                min_node->minimum = min_node;
                push_min(min_node->parent);
                return;
            }
            if (min_node->right) {
                replace_with_right(min_node);
                Node *m = min_node->right->minimum;
                dealloc_entity(min_node);
                min_node = m;
            } else {
                if (min_node->parent) {
                    min_node->parent->left = nullptr;
                    Node *parent = min_node->parent;
                    dealloc_entity(min_node);
                    min_node = parent;
                } else {
                    dealloc_entity(min_node);
                    root = nullptr;
                    root -> minimum = root;
                    return;
                }
            }
        }
    }
    Node *root = nullptr;
    std::unordered_map <TKey, Node*> table;
public:
    void erase(const TKey &x){
        auto iter = table.find(x);
        if (iter != table.end()){
            iter->second->count--;
        }
    }
    void insert(const TKey &x){
        auto iter = table.find(x);
        if (iter != table.end()){
            iter->second->count++;
            return;
        }
        Node *element = new Node(x);
        Node *less, *greater;

        std::tie(less, greater) = split(root, x);
        root = merge(merge(less, element), greater);
        table[x]= element;
    }
    TKey find_min(){
        update_min(root -> minimum);
        return root -> minimum -> key;
    }

};
#endif
