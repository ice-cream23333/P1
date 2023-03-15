#include "filesystem"
#include "./external/porter2_stemmer.h"
#include "bits/stdc++.h"
using namespace std;
namespace fs=std::filesystem;
std::map<string,int> dict;


void file_parser(ifstream& file,std::map<string,int>&dict)
{
    string temp,word;
    bool inword=0;
    while(getline(file,temp)){
        for(int i=0;temp[i];i++)
        {
            if(std::isalnum(temp[i]))
            {
                if(inword==0) inword=1;
                word.push_back(temp[i]);
            }
            else if(inword)
            {
                Porter2Stemmer::trim(word);
                Porter2Stemmer::stem(word);
                dict[word]++;
                inword=0,word.clear();
            }

        }
    }
}

void load_dict(string& dict_base,std::map<string,int>&dict,string& filename)
{
    for(auto &p:dict)
    {
        string word=p.first;
        fstream f(dict_base+"/"+word+".txt",ios::out|ios::app);
        f<<filename<<"/"<<p.second<<endl;
        f.close();
    }
    return;
}

int main()
{
    fs::path base("../sample");
    fs::path dict_base("../dict");
    if(!fs::is_directory(dict_base))
        fs::create_directory(dict_base);
    for(auto &v:fs::directory_iterator(base))
    {
        std::string filename=v.path().filename().string();
        cout<<"directory:"<<filename<<endl;
        for(auto &tv:fs::directory_iterator(base/filename))
        {
            string full_path=tv.path().string();
            ifstream file(full_path,ios::in);
            file_parser(file,dict);
            string article_name=filename+" "+tv.path().filename().string();
            load_dict(dict_base.string(),dict,article_name);
            dict.clear();
        }
    }
    return 0;

}