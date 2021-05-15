#include <fmt/core.h>
#include <cassert>

using fmt::print;

template<typename T>
class RedBlackTree
{
public:
        struct Node
        {
                T key;
                Node* left = nullptr;
                Node* right = nullptr;
                Node* parent = nullptr;
                char color = 'r';
        };

public:
        RedBlackTree() = default;

        ~RedBlackTree()
        {
                free_nodes(root);
        }

        Node* find(const T& key) const
        {
                Node* node = root;

                while(node != nullptr)
                {
                        if(node->key == key)
                        {
                                return node;
                        }

                        if(key < node->key)
                        {
                                node = node->left;
                        }
                        else
                        {
                                node = node->right;
                        }
                }

                return nullptr;
        }

        void print_inorder(FILE* f) const
        {
                print_inorder(f, root);
        }

        std::vector<T> get_inorder_vector() const
        {
                std::vector<T> vec;
                vec.reserve(sz);

                get_inorder_vector(vec, root);

                return vec;
        }

        void insert(const T& key)
        {
                ++sz;

                Node* z = new Node{key};

                Node* y = nullptr;
                Node* x = root;

                while(x != nullptr)
                {
                        y = x;

                        if(key < x->key)
                        {
                                x = x->left;
                        }
                        else
                        {
                                x = x->right;
                        }
                }

                z->parent = y;
                if(y == nullptr)
                {
                        root = z;
                }
                else if(key < y->key)
                {
                        y->left = z;
                }
                else
                {
                        y->right = z;
                }

                fix_after_insert(z);
        }

        void left_rotate(Node* x)
        {
                Node* y = x->right;
                x->right = y->left;

                if(y->left != nullptr)
                {
                        y->left->parent = x;
                }

                y->parent = x->parent;
                if(x->parent == nullptr)
                {
                        root = y;
                }
                else if(x == x->parent->left)
                {
                        x->parent->left = y;
                }
                else
                {
                        x->parent->right = y;
                }

                y->left = x;
                x->parent = y;
        }

        void right_rotate(Node* y)
        {
                Node* x = y->left;
                y->left = x->right;

                if(x->right != nullptr)
                {
                        x->right->parent = y;
                }

                x->parent = y->parent;
                if(y->parent == nullptr)
                {
                        root = x;
                }
                else if(y == y->parent->left)
                {
                        y->parent->left = x;
                }
                else
                {
                        y->parent->right = x;
                }

                x->right = y;
                y->parent = x;
        }

private:
        void print_inorder(FILE* f, const Node* node) const
        {
                if(node != nullptr)
                {
                        print_inorder(f, node->left);
                        fmt::print(f, "{}\n", node->key);
                        print_inorder(f, node->right);
                }
        }

        void get_inorder_vector(std::vector<T>& vec, const Node* node) const
        {
                if(node != nullptr)
                {
                        get_inorder_vector(vec, node->left);
                        vec.push_back(node->key);
                        get_inorder_vector(vec, node->right);
                }
        }

        void fix_after_insert(Node* z)
        {
                while(z->parent != nullptr && z->parent->color == 'r')
                {
                        if(is_lchild(z->parent))
                        {
                                Node* y = z->parent->parent->right;
                                if(has_color(y, 'r'))
                                {
                                        z->parent->color = 'b';
                                        y->color = 'b';
                                        z->parent->parent->color = 'r';
                                        z = z->parent->parent;
                                }
                                else
                                {
                                        if(is_rchild(z))
                                        {
                                                z = z->parent;
                                                left_rotate(z);
                                        }
                                        z->parent->color = 'b';
                                        z->parent->parent->color = 'r';
                                        right_rotate(z->parent->parent);
                                }
                        }
                        else
                        {
                                Node* y = z->parent->parent->left;
                                if(has_color(y, 'r'))
                                {
                                        z->parent->color = 'b';
                                        y->color = 'b';
                                        z->parent->parent->color = 'r';
                                        z = z->parent->parent;
                                }
                                else
                                {
                                        if(is_lchild(z))
                                        {
                                                z = z->parent;
                                                right_rotate(z);
                                        }
                                        z->parent->color = 'b';
                                        z->parent->parent->color = 'r';
                                        left_rotate(z->parent->parent);
                                }
                        }
                }
                root->color = 'b';
        }

        void free_nodes(Node* node)
        {
                if(node != nullptr)
                {
                        free_nodes(node->left);
                        free_nodes(node->right);
                        delete node;
                }
        }

        static bool is_lchild(const Node* node)
        {
                assert(node != nullptr);
                return node->parent != nullptr && node == node->parent->left;
        }

        static bool is_rchild(const Node* node)
        {
                assert(node != nullptr);
                return node->parent != nullptr && node == node->parent->right;
        }

        static bool has_color(const Node* node, const char color)
        {
                if(node != nullptr)
                {
                        return node->color == color;
                }
                return color == 'b';
        }

private:
        Node* root = nullptr;
        unsigned sz = 0;
};
