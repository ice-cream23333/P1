//
// Created by andres on 2023-03-15.
//
#include "bits/stdc++.h"
#include "filesystem"
#include "./external/porter2_stemmer.h"
std::map<std::string,int> final;
std::vector<std::pair<std::string ,int>> sort_final;
void string_split(std::string str,char split,std::vector<std::string>& tokens)
{
    std::istringstream iss(str);
    std::string token;
    while(std::getline(iss,token,split))
        tokens.push_back(token);
    return ;
}
int main()
{
    int threshold=60;
    std::string words;
    std::vector<std::string> tokens;
    std::string dict_base="../dict";
    std::cout<<"Enter the word you need to query(split them by space):"<<std::endl;
    std::getline(std::cin,words);
    string_split(words,' ',tokens);
    for(auto& word:tokens)
    {
        Porter2Stemmer::trim(word);
        Porter2Stemmer::stem(word);
        std::fstream f(dict_base+"/"+word+".txt",std::ios::in);
        if(f.is_open())
        {
            std::string line;
            std::vector<std::string> search_result;
            while(std::getline(f,line))
            {
                string_split(line,'/',search_result);
                final[search_result[0]]+=atoi(search_result[1].c_str())+100;
                search_result.clear();
            }
        }
        f.close();
    }
    for(auto &p:final)
        sort_final.push_back(std::pair<std::string ,int>(p.first,p.second));
    std::sort(sort_final.begin(),sort_final.end(),
              [](std::pair<std::string,int> a,std::pair<std::string,int> b){return a.second>b.second;});
    int i=0;


    for(auto &res:sort_final)
    {
        std::cout<<res.first<<std::endl;
        if(++i==threshold) break;
    }
    return 0;
}