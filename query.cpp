//
// Created by andres on 2023-03-15.
//
#include "bits/stdc++.h"
#include "filesystem"
#include "./external/porter2_stemmer.h"

// Define a struct named "unit" to store article names and the times the word appears in each article
struct unit{
    std::string name;
    int times;
    unit(const std::string& n,const int i){times=i;name=n;}
};

// Define a struct named "search_result" to store the search result of each article
struct search_result{
    int match_words=0;
    int score=0;
};

// Declare global variables
std::map<std::string,std::vector<unit>> dict;  // to store the dictionary read from the file
std::map<std::string,search_result> final;     // to store the search result
std::vector<std::pair<std::string ,search_result>> sort_final;   // to store the sorted search result

// Function to read the dictionary file
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

// Function to split a string into multiple tokens based on a given delimiter
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
// Initialize variables
std::string dict_base("../dict"); // Base directory for the dictionary files
int threshold=10; // Maximum number of search results to display
std::string words; // Input words to search for
std::vectorstd::string tokens; // Vector to store the individual words
load_dict(dict_base,dict); // Load the dictionary files into a map
std::cout<<"Enter the word you need to query(split them by space):"<<std::endl;
std::getline(std::cin,words); // Read in the input words
string_split(words,' ',tokens); // Split the input words by spaces and store in the tokens vector
// Loop through each input word
for(auto& word:tokens)
{
    // Apply Porter2 stemmer algorithm to reduce words to their base form
    Porter2Stemmer::trim(word);
    Porter2Stemmer::stem(word);
    // Loop through each unit (i.e. arc) in the dictionary corresponding to the current word
    for(auto &arc:dict[word])
        // Update the corresponding search result with the number of matches and the total score
        final[arc.name].score+=arc.times,final[arc.name].match_words++;
}

// Copy the search results into a vector for sorting
for(auto &p:final)
    sort_final.push_back(std::pair<std::string ,search_result>(p.first,p.second));

// Sort the search results by number of matches and then by score
std::sort(sort_final.begin(),sort_final.end(),
          [](auto a,auto b){
    return a.second.match_words==b.second.match_words?
            a.second.score>b.second.score:
            a.second.match_words>b.second.match_words;
});

// Display the top search results
int i=0;
for(auto &res:sort_final)
{
    std::cout<<res.first<<std::endl;
    if(++i==threshold) break;
}
return 0;
}
