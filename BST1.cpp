#include <iostream>
#include <vector>
using namespace std;
struct TreeNode{
    TreeNode* parent;
    TreeNode* left;
    TreeNode* right;
    int val;
    int cnt;
    int size;
    TreeNode(int x){
        parent=nullptr;
        left=nullptr;
        right=nullptr;
        val=x;
        cnt=1;
        size=1;  //the size represent the number of nodes in the subtree whose root is this node
    }
};

void update_size(TreeNode* node){
    if(node!=nullptr){
        node->size=node->cnt;
        if(node->left!=nullptr){
            node->size+=node->left->size;
        }
        if(node->right!=nullptr){
            node->size+=node->right->size;
        }
    }
}

void Insert(TreeNode*& root,TreeNode* p,int x){
    if(root==nullptr){
        root=new TreeNode(x);
        root->parent=p;
        return;
    }
    if(x<root->val){
        Insert(root->left,root,x);
    }else if(x==root->val){
        root->cnt++;
    }else{
        Insert(root->right,root,x);
    }
    update_size(root);
};

TreeNode* Successor(TreeNode* root,int x){
    TreeNode* answer=nullptr;  //record the local optimal solution
    while(root!=nullptr){
        if(root->val>x){  //if current node is greater than x, it will be the best solution up to now
            answer=root;
            root=root->left;  //try to find whether there is a better solution in the left subtree
        }else{
            root=root->right;
        }
    }
    return answer;
}

TreeNode* Predecessor(TreeNode* root,int x){
    TreeNode* answer=nullptr;  //record the local optimal solution
    while(root!=nullptr){
        if(root->val<x){  //if current node is smaller than x, it will be the best solution up to now
            answer=root;
            root=root->right;  //try to find whether there is a better solution in the left subtree
        }else{
            root=root->left;
        }
    }
    return answer;
}

TreeNode* Search(TreeNode* root,int x){
    while(root->val!=x){
        if(root->val>x){
            root=root->left;
        }else{
            root=root->right;
        }
    }
    return root;
}

void Transplant(TreeNode*& root,TreeNode* node1,TreeNode* node2){
    if(node1->parent==nullptr){
        root=node2;
    }else if(node1==node1->parent->left){
        node1->parent->left=node2;
    }else{
        node1->parent->right=node2;
    }
    if(node2!=nullptr){
        node2->parent=node1->parent;
    }
}
 
void Delete(TreeNode*& root,int x){
    TreeNode* node1=Search(root,x);
    if(node1->cnt>1){
        node1->cnt--;
        while(node1!=nullptr){
            update_size(node1);
            node1=node1->parent;
        }
    }else{
        if(node1->left==nullptr){
            TreeNode* node2=node1->right;
            Transplant(root,node1,node2);
            if(node2==nullptr){
                node2=node1->parent;
            }
            while(node2!=nullptr){
            update_size(node2);
            node2=node2->parent;
            }
            delete node1;
        }else if(node1->right==nullptr){
            TreeNode* node2=node1->left;
            Transplant(root,node1,node2);
            if(node2==nullptr){
                node2=node2->parent;
            }
            while(node2!=nullptr){
            update_size(node2);
            node2=node2->parent;
            }
            delete node1;
        }else{
            TreeNode* node2 = Successor(root,x);
            TreeNode* node3= node2;
            if(node2!=node1->right){
                node3= node2->parent;
                Transplant(root,node2,node2->right);
                node2->right=node1->right;
                node2->right->parent=node2;
            }
            Transplant(root,node1,node2);
            node2->left=node1->left;
            node2->left->parent=node2;
            delete node1;
            
            while(node3!=nullptr){
            update_size(node3);
            node3=node3->parent;
            }
        }
    }
}

int smaller(TreeNode* root,int x){
    if(root==nullptr){
        return 0;
    }
    if(x<root->val){
        return smaller(root->left,x);
    }else if(x==root->val){
        if(root->left!=nullptr){
            return root->left->size;
        }else{
            return 0;
        } 
    }else{
        if(root->left!=nullptr){
            return root->left->size+root->cnt+smaller(root->right,x);
        }else{
            return root->cnt+smaller(root->right,x);
        } 
    }
}

TreeNode* rank_at_x(TreeNode* root,int x){
    int temp=0;
    if(root->left==nullptr){
        temp=0;
    }else{
        temp=root->left->size;
    }
    if(x<=temp){
        return rank_at_x(root->left,x);
    }else if(temp<x&&x<=temp+root->cnt){
        return root;
    }else{
        return rank_at_x(root->right,x-(temp+root->cnt));
    }
}

void Inorder(TreeNode* root,vector<int>& arr){
    if(root == nullptr){
        return;
    }
    Inorder(root->left,arr);
    arr.push_back(root->val);
    Inorder(root->right,arr);
}

void Preorder(TreeNode* root,vector<int>& arr){
    if(root == nullptr){
        return;
    }
    arr.push_back(root->val);
    Preorder(root->left,arr);
    Preorder(root->right,arr);
}

void Postorder(TreeNode* root,vector<int>& arr){
    if(root == nullptr){
        return;
    }
    Postorder(root->left,arr);
    Postorder(root->right,arr);
    arr.push_back(root->val);
}

TreeNode* root=nullptr;
int main(){
    int n;
    cin>>n;
    int op;
    int m;
    while(n>=1){
        cin>>op;
        cin>>m;
        if(op==1){
            Insert(root,nullptr,m);
        }else if(op==2){
            Delete(root,m);
        }else if(op==3){
            cout<<smaller(root,m)+1<<endl;
        }else if(op==4){
            cout<<rank_at_x(root,m)->val<<endl;
        }else if(op==5){
            cout<<Predecessor(root,m)->val<<endl;
        }else if(op==6){
            cout<<Successor(root,m)->val<<endl;
        }
        n--;
    }
}