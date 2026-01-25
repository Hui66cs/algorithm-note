#include <iostream>
#include <vector>
#include <climits>
using namespace std;
struct TreeNode {
    int val;
    TreeNode *left;
    TreeNode *right;
    long long record;
    //vector<int> arr;
    TreeNode() : val(0), left(nullptr), right(nullptr) {}
    TreeNode(int x) : val(x), left(nullptr), right(nullptr) {}
    TreeNode(int x, TreeNode *left, TreeNode *right) : val(x), left(left), right(right),record(LONG_MIN) {}
/*    void Travasal(TreeNode* root){
        if(root!=nullptr){
            Travasal(root->left);
            arr.push_back(root->val);
            Travasal(root->right);
        }
    }
*/
    bool isValidBST(TreeNode* root) {
        if(root!=nullptr){
            if(!isValidBST(root->left)){
                return false;
            }
            if(root->val<=record){
                return false;
            }
            record=root->val;
            return isValidBST(root->right);
        }else{
            return true;
        }
    }
};
/* bool isValidBST(TreeNode* root) {
        vector<int> arr;
        if(root!=nullptr){
            if(isValidBST(root->left)&&root->val>record){
                record=root->val;
                if(isValidBST(root->right)&&root->value){
                    return true;
                }else{
                    return false;
                }
            }else{
                return false;
            }
        }else{
            return true;
        }
    }
*/