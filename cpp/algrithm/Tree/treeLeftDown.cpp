struct TreeNode
{
    TreeNode* left;
    TreeNode* right;
    int val;
    TreeNode():val(0),left(nullptr),right(nullptr){};
    TreeNode(int x):val(x),left(nullptr),right(nullptr){};
    TreeNode(int x,TreeNode* left,TreeNode* right):val(x),left(left),right(right);
};

class Solution{
public:
    int deep=0;
    int value;
    int findBottomLeftValue(TreeNode* root){
        dfs(root,0);
        return value;
    }
    void dfs(TreeNode* root,int depth){
        if(root==nullptr){
            return;
        }
        if(deep==depth){
            value=root->val;
            deep+=1;
        }
        dfs(root->left,depth+1);
        dfs(root->right,depth+1);
    }
};
