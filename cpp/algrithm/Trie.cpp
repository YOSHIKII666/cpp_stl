#include<iostream>
#include<vector>
#include<string>
class Trie{
    bool isEnd;
    std::vector<Trie*> children;
    Trie():isEnd(false),children(26){};
    void insert(std::string s){
        Trie* node=this;
        for(int i=0;i<s.size();i++){
            if(this->children[s[i]-'a']==nullptr){
                this->children[s[i]-'a']=new Trie();
            }
            node=node->children[s[i]-'a'];
        }
        node->isEnd=true;
    }
    Trie* searchPrefix(std::string s){
        Trie* node=this;
        for(int i=0;i<s.size();i++){
            if(this->children[s[i]-'a']==nullptr){
                return nullptr;
            }
            node=node->children[s[i]-'a'];
        }
        return node;
    }
    bool searchStart(std::string s){
        return searchPrefix(s)!=nullptr; 
    }
    bool search(std::string s){
        Trie* node = searchPrefix(s);
        return node!=nullptr&&node->isEnd;
    }
};

int main()
{
    
    std::cout<<"hello"<<std::endl;
}