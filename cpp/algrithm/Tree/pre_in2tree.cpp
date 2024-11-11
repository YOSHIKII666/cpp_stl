#include<vector>
#include<unordered_map>
struct TreeNode{
    TreeNode* left;
    TreeNode* right;
    int val;
    TreeNode():val(0),left(nullptr),right(nullptr){};
    TreeNode(int x):val(x),left(nullptr),right(nullptr){};
    TreeNode(int x,TreeNode* left,TreeNode* right):val(x),left(left),right(right){};
};

class solution{
    public:
        std::unordered_map<int,int> map;
        int id=0;
        std::vector<int> preorder;
        TreeNode* build(std::vector<int>& preorder,std::vector<int>& inorder){
                this->preorder=preorder;
                for(int i=0;i<inorder.size();i++){
                    map.emplace(inorder[i],i);
                }
                TreeNode* root=dfs(0,inorder.size()-1);
                return root;
        }
        TreeNode* dfs(int left,int right){
            if(left<=right){
                TreeNode* root=new TreeNode(preorder[id]);
                int index=map[root->val];
                id++;
                root->left=dfs(left,index-1);
                root->right=dfs(index+1,right);
                return root;
            }else{
                return nullptr;
            }
        }
};