//
// Created by Kinstintin on 28.04.2024.
//

#ifndef UNTITLED_MYSET_H
#define UNTITLED_MYSET_H
#include <random>
#include <unordered_map>

static std::uniform_int_distribution<int> uniform(1, INT32_MAX);
std::random_device dev;
static std::mt19937_64 rng(dev());


template <typename T,
        typename Compare = std::less<T>>
class Myset{
private:
    struct Node{
        Node* par;
        Node* minim;
        Node* left;
        Node* right;
        T key;
        int prior;
        int count;
        explicit Node(const T &value){
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
    std::tuple <Node*, Node*> split(Node *tree,const T &key){
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
    Node* merge(Node *t1, Node *t2){
        if (!t1)
            return t2;
        if (!t2)
            return t1;

        if (t1 -> prior > t2 -> prior){
            t1 -> right = merge(t1 -> right, t2);
            renew(t1);
            return t1;
        }
        t2 -> left = merge(t1, t2 -> left);
        renew(t2);
        return t2;
    }
    void push_min(Node *n){
        if  (!n)
            return;
        n-> minim = n -> left -> minim;
        push_min(n -> par);
    };
    void replace_with_right(Node *t){ // consider right exists

        if (t -> par){
            t -> par -> left = t -> right;
            t -> right -> par = t -> par;
        } else{
            root = t -> right;
            root -> par = nullptr;
        }
    }
    void dealloc_entity(Node *n){
        table.erase(n->key);
        delete n;
    }
    void upd_min(Node *mi){
        Node* t = mi;
        for(;;) {
            if (t->count > 0) {
                t->minim = t;
                push_min(t->par);
                return;
            }
            if (t->right) {
                replace_with_right(t);
                Node *m = t->right->minim;
                dealloc_entity(t);
                t = m;
            } else {
                if (t->par) {
                    t->par->left = nullptr;
                    Node *par = t->par;
                    dealloc_entity(t);
                    t = par;
                } else {
                    dealloc_entity(t);
                    root = nullptr;
                    root -> minim = root;
                    return;
                }
            }
        }
    }
    Node *root = nullptr;
    std::unordered_map <T, Node*> table;
public:
    void erase(const T x){
        if (table.contains(x)){
            table[x]->count--;
        }
    }
    void insert(const T x){
        if (table.contains(x)){
            table[x] -> count++;
            return;
        }
        Node *element = new Node(x);
        Node *less, *greater;

        std::tie(less, greater) = split(root, x);
        root = merge(merge(less, element), greater);
        table[x]= element;
    }
    T find_min(){
        upd_min(root -> minim);
        return root -> minim -> key;
    }

};
#endif //UNTITLED_MYSET_H
