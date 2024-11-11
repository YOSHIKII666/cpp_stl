#include<vector>
class solution{
    public:
        int rob(std::vector<int>& nums){
            std::vector<int> dp(nums.size());
            if(nums.size()==1) return nums[0];
            dp[0]=nums[0];
            dp[1]=std::max(nums[0],nums[1]);
            if(nums.size()==2) return dp[1];
            for(int i=2;i<nums.size();i++){
                dp[i]=std::max(dp[i-1],dp[i-2]+nums[i]);
            }
            return dp[nums.size()-1];
        }
};