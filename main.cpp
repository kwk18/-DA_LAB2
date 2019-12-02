//
//  main.cpp
//  AVLTree
//
//  Created by kosha on 23/11/2019.
//  Copyright © 2019 kosha. All rights reserved.
//


#include <iostream>
#include <fstream>
#include <algorithm>

using namespace std;
using Tll = long double;
const int S = -1;
class TAVLTree {
protected:
    struct Node {
        Node (const string& key, const Tll& value, int height):
            key(key), value(value), height(height)
        {}
        string key;
        Tll value;
        int height;
        Node *left, *right;
    };
    
    Node *root = NULL;
    int Max(int a, int b) {
        return (a > b) ? a : b;
    }
    
    int Height(Node *N) {
        if (N == NULL) {
            return 0;
        }
        return N->height;
    }
    
    Node* NewNode(string key, Tll value) {
        Node* node = new Node(key, value, 1);
        node->left = NULL;
        node->right = NULL;
        return node;
    }
    
    Node *RightRotate(Node *y) {
        Node *x = y->left;
        Node *T2 = x->right;
        
        x->right = y;
        y->left = T2;
        
        y->height = max(this->Height(y->left), this->Height(y->right)) + 1;
        x->height = max(this->Height(x->left), this->Height(x->right)) + 1;
        
        return x;
    }
    
    Node *LeftRotate(Node *x) {
        Node *y = x->right;
        Node *T2 = y->left;
        y->left = x;
        x->right = T2;
        x->height = max(this->Height(x->left), this->Height(x->right)) + 1;
        y->height = max(this->Height(y->left), this->Height(y->right)) + 1;
        return y;
    }
    
    int GetBalance(Node *N) {
        if (N == nullptr)
            return 0;
        return Height(N->left) - Height(N->right);
    }
    
    Node* Insert(Node* node, string key, Tll value) {
        if (node == nullptr)
            return(this->NewNode(key, value));
        
        if (key.compare(node->key)<0)
            node->left = this->Insert(node->left, key, value);
        else if (key.compare(node->key)>0)
            node->right = this->Insert(node->right, key, value);
        else
            return node;
        
        node->height = 1 + max(this->Height(node->left), this->Height(node->right));
        
        int balance = GetBalance(node);
        
        if (balance > 1 && key.compare(node->left->key) < 0)
            return this->RightRotate(node);
        
        if (balance < -1 && key.compare(node->right->key) > 0)
            return this->LeftRotate(node);
        
        if (balance > 1 && key.compare(node->left->key) > 0) {
            node->left = LeftRotate(node->left);
            return this->RightRotate(node);
        }
        
        if (balance < -1 && key.compare(node->right->key) < 0) {
            node->right = RightRotate(node->right);
            return this->LeftRotate(node);
        }
        
        return node;
    }
    
    Node * MinValueNode(Node* node) {
        Node* current = node;
        while (current->left != NULL) {
            current = current->left;
        }
        
        return current;
    }
    
    Node* DeleteNode(Node* root, string key) {
        if (root == NULL)
            return root;
        
        if ( key.compare(root->key) < 0) {
            root->left = this->DeleteNode(root->left, key);
        } else if( key.compare(root->key) > 0) {
            root->right = this->DeleteNode(root->right, key);
        } else {
            if( (root->left == NULL) || (root->right == NULL) ) {
                Node *temp = root->left ? root->left : root->right;
                if (temp == NULL) {
                    temp = root;
                    root = NULL;
                }
                else
                    *root = *temp;
                free(temp);
            } else {
                Node* temp = this->MinValueNode(root->right);
                root->key = temp->key;
                root->right = this->DeleteNode(root->right, temp->key);
            }
        }
        
        if (root == nullptr)
            return root;
        
        root->height = 1 + max(this->Height(root->left), this->Height(root->right));
        
        int balance = this->GetBalance(root);
        
        
        if (balance > 1 && this->GetBalance(root->left) >= 0)
            return this->RightRotate(root);
        
        if (balance > 1 && this->GetBalance(root->left) < 0) {
            root->left = this->LeftRotate(root->left);
            return this->RightRotate(root);
        }
        
        if (balance < -1 && this->GetBalance(root->right) <= 0)
            return this->LeftRotate(root);
        
        if (balance < -1 && this->GetBalance(root->right) > 0) {
            root->right = this->RightRotate(root->right);
            return this->LeftRotate(root);
        }
        
        return root;
    }
    
    bool IsExists(Node* node, string key) {
        if (node == NULL) {
            return false;
        }
        
        if (key.compare(node->key) < 0) {
            return IsExists(node->left, key);
        } else if (key.compare(node->key) > 0) {
            return IsExists(node->right, key);
        } else {
            return true;
        }
        
        return false;
    }
    
    Tll SearchValue(Node *node, string key) {
        if (node == NULL) {
            return -1;
        }
        
        if (key.compare(node->key) < 0) {
            return this->SearchValue(node->left, key);
        } else if (key.compare(node->key) > 0) {
            return this->SearchValue(node->right, key);
        } else {
            return node->value;
        }
    }
    
public:
    int InsertKey(string key, Tll value) {
        for_each(key.begin(), key.end(), [](char &c) {
            c = ::tolower(c);
        });
        
        if (IsExists(this->root, key)) {
            return -1;
        }
        
        this->root = this->Insert(this->root, key, value);
        return 0;
    }
    
    int DeleteKey(string key) {
        for_each(key.begin(), key.end(), [](char &c) {
            c = ::tolower(c);
        });
        
        if (!IsExists(this->root, key)) {
            return -1;
        }
        
        this->root = this->DeleteNode(this->root, key);
        return 0;
    }
    
    Tll SearchValue(string key) {
        for_each(key.begin(), key.end(), [](char &c) {
            c = ::tolower(c);
        });
        
        return this->SearchValue(this->root, key);
    }
};

class TSerializableAvltree: public TAVLTree {
public:
    void Serialize(const string &fileName) const {
        ofstream os(fileName);
        SerializeImpl(root, os);
    }
    void Deserialize(const string &fileName) {
        ifstream is(fileName);
        root = DeserializeImpl(is);
    }
private:
    void SerializeImpl(const Node* node, ofstream &os) const {
        if (node == nullptr) {
            return;
        }
        os << node->key << '\t';
        os << node->value << '\t';
        os << node->height << '\t';
        
        bool hasLeft = (node->left != nullptr);
        bool hasRight = (node->right != nullptr);
        
        os << hasLeft << '\t';
        os << hasRight << '\n';
        
        if (hasLeft) {
            SerializeImpl(node->left, os);
        }
        
        if (hasRight) {
            SerializeImpl(node->right, os);
        }
    }
    
    Node* DeserializeImpl(ifstream &is) {
        Node* root = nullptr;
        string key;
        if (!(is >> key))
            return root;
        
        Tll value = 0;
        is >> value;
        int height = 0;
        is >> height;
        bool hasRight = false;
        bool hasLeft = false;
        is >> hasLeft;
        is >> hasRight;
        root = new Node(key, value, height);
        if (hasLeft)
            root->left = DeserializeImpl(is);
        if (hasRight)
            root->right = DeserializeImpl(is);
        return root;
    }
};

int main() {
    std::ios::sync_with_stdio(false);
    TSerializableAvltree tree = TSerializableAvltree();
    string cmd;
    
    while (cin >> cmd) {
        try {
            if (cmd == "+") {
                string key;
                Tll value;
                cin >> key >> value;
                
                if (tree.InsertKey(key, value) == S) {
                    cout << "Exist" << endl;
                } else {
                    cout << "OK" << endl;
                }
            } else if (cmd == "-") {
                string key;
                cin >> key;
                
                if (tree.DeleteKey(key) == S) {
                    cout << "NoSuchWord" << endl;
                } else {
                    cout << "OK" << endl;
                }
            } else if (cmd == "!") {
                string additionalCmd, fileName;
                cin >> additionalCmd >> fileName;
                
                try {
                    if (additionalCmd[0] == 'S') {
                        tree.Serialize(fileName);
                    } else {
                        tree.Deserialize(fileName);
                    }
                    
                    cout << "OK" << endl;
                } catch (system_error &error) {
                    cout << "ERROR: " << error.what() << endl;
                }
            } else {
                Tll value = tree.SearchValue(cmd);
    
                if (value != -1) {
                    cout << "OK: " << value << endl;
                } else {
                    cout<< "NoSuchWord"<< endl;
                }
            }
        } catch (runtime_error &error) {
            cout << "ERROR: " << error.what() << endl;
        }
    }
    
    return 0;
}
