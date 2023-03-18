//
// Created by andres on 2023-03-15.
//
#include "bits/stdc++.h"
#include "filesystem"
#include "./external/porter2_stemmer.h"
struct unit{
    std::string name;
    int times;
    unit(const std::string& n,const int i){times=i;name=n;}
};
struct search_result{
    int match_words=0;
    int score=0;
};
std::map<std::string,std::vector<unit>> dict;
std::map<std::string,search_result> final;
std::vector<std::pair<std::string ,search_result>> sort_final;

void load_dict(std::string& dict_base,std::map<std::string,std::vector<unit>>&dict)
{
    std::fstream f(dict_base+"/dict.txt",std::ios::in);
    std::string word,trash;
    while(f>>word) {
        int size;
        f>>size;
        std::getline(f,trash);
        int num;std::string arc;
        for(int i=0;i<size;i++) {
            std::getline(f,arc);
            f>>num;
            dict[word].emplace_back(arc,num);
            std::getline(f,trash);
        }
    }

}

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
    std::string dict_base("../dict");
    int threshold=10;
    std::string words;
    std::vector<std::string> tokens;
    load_dict(dict_base,dict);
    std::cout<<"Enter the word you need to query(split them by space):"<<std::endl;
    std::getline(std::cin,words);
    string_split(words,' ',tokens);
    for(auto& word:tokens)
    {
        Porter2Stemmer::trim(word);
        Porter2Stemmer::stem(word);
        for(auto &arc:dict[word])
            final[arc.name].score+=arc.times,final[arc.name].match_words++;
    }
    for(auto &p:final)
        sort_final.push_back(std::pair<std::string ,search_result>(p.first,p.second));
    std::sort(sort_final.begin(),sort_final.end(),
              [](auto a,auto b){
        return a.second.match_words==b.second.match_words?
                a.second.score>b.second.score:
                a.second.match_words>b.second.match_words;
    });
    int i=0;


    for(auto &res:sort_final)
    {
        std::cout<<res.first<<std::endl;
        if(++i==threshold) break;
    }
    return 0;
}