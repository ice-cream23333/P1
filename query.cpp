#include "bits/stdc++.h"
#include "filesystem"
#include "./external/porter2_stemmer.h"

// A unit consists of a string name and an integer times.
// It is used to represent the frequency of a word in a document.
struct unit{
    std::string name;
    int times;
    unit(const std::string& n,const int i){times=i;name=n;}
};

// A search_result consists of two integers: match_words and score.
// match_words represents the number of query words matched in the document,
// while score represents the total frequency of all matched words.
struct search_result{
    int match_words=0;
    int score=0;
};

// A dictionary maps each word to a vector of units.
// Each unit contains the name of a document and the frequency of the word in that document.
std::map<std::string,std::vector<unit>> dict;

// A final map maps each document name to its corresponding search_result.
std::map<std::string,search_result> final;

// A vector of pairs of document name and search_result.
std::vector<std::pair<std::string ,search_result>> sort_final;

// This function reads the dictionary from the file system and fills the dict map.
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

// This function splits a string into tokens using the given delimiter character,
// and stores the tokens in a vector.
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

    // Ask the user to input query words.
    std::cout<<"Enter the word you need to query(split them by space):"<<std::endl;
    std::getline(std::cin,words);
    string_split(words,' ',tokens);

    // For each query word, stem it, and then update the search_result for each document
    // in which the query word appears.
    for(auto& word:tokens)
    {
        Porter2Stemmer::trim(word);
        Porter2Stemmer::stem(word);
        for(auto &arc:dict[word])
            final[arc.name].score+=arc.times,final[arc.name].match_words++;
    }

    // Copy the final map to the sort_final vector, and then sort the vector
    // based on the match_words and score fields in search_result.
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
