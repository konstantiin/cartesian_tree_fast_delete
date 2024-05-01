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
        Node* par;
        Node* minim;
        Node* left;
        Node* right;
        TKey key;
        int prior;
        int count;
        explicit Node(const TKey &value){
            key = value;
            count = 1;
            left = nullptr;
            right = nullptr;
            minim = this;
            par = nullptr;
            prior = uniform(rng);
        }
    };
    void renew(Node* &n){
        if (!n)
            return;

        n->minim = n->left != nullptr ? n->left->minim : n;
        if (n -> left)
            n -> left -> par = n;
        if (n -> right)
            n -> right -> par = n;
    }
    std::tuple <Node*, Node*> split(Node *tree,const TKey &key){
        Node *left = nullptr, *right = nullptr;
        if (!tree)
            return std::make_tuple(left, right);
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
        if (left)
            left -> par = nullptr;
        if (right)
            right -> par = nullptr;
        return std::make_tuple(left, right);
    }
    Node* merge(Node *tree1, Node *tree2){
        if (!tree1)
            return tree2;
        if (!tree2)
            return tree1;

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
        if  (!node)
            return;
        node-> minim = node -> left -> minim;
        push_min(node -> par);
    };
    void replace_with_right(Node *node){ // consider right exists

        if (node -> par){
            node -> par -> left = node -> right;
            node -> right -> par = node -> par;
        } else{
            root = node -> right;
            root -> par = nullptr;
        }
    }
    void dealloc_entity(Node *node){
        table.erase(node->key);
        delete node;
    }
    void update_min(Node *minimal){
        for(;;) {
            if (minimal->count > 0) {
                minimal->minim = minimal;
                push_min(minimal->par);
                return;
            }
            if (minimal->right) {
                replace_with_right(minimal);
                Node *m = minimal->right->minim;
                dealloc_entity(minimal);
                minimal = m;
            } else {
                if (minimal->par) {
                    minimal->par->left = nullptr;
                    Node *par = minimal->par;
                    dealloc_entity(minimal);
                    minimal = par;
                } else {
                    dealloc_entity(minimal);
                    root = nullptr;
                    root -> minim = root;
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
        update_min(root -> minim);
        return root -> minim -> key;
    }

};
#endif
