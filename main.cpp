#include "filesystem"
#include "./external/porter2_stemmer.h"
#include "bits/stdc++.h"
using namespace std;
namespace fs=std::filesystem;
struct unit{
    string name;
    int times;
    unit(const string& n){times=1;name=n;}
};
std::map<string,vector<unit>> dict;

void file_parser(ifstream& file,std::map<string,vector<unit>>&dict,string articleName)
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
                if(word.size()==0) continue;
                if(dict.count(word)==0||articleName!=(dict[word].end()-1)->name)
                    dict[word].push_back(unit(articleName));
                else
                    (dict[word].end()-1)->times++;
                inword=0,word.clear();
            }

        }
    }
}

void load_dict(string& dict_base,std::map<string,vector<unit>>&dict)
{
    fstream f(dict_base+"/dict.txt",ios::out);
    for(auto &p:dict)
    {
        string word=p.first;
        f<<p.first<<endl<<p.second.size()<<endl;
        for(auto &arc:p.second)
            f<<arc.name<<endl<<arc.times<<endl;
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
            string article_name=filename+" "+tv.path().filename().string();
            file_parser(file,dict,article_name);
        }
    }
    load_dict(dict_base.string(),dict);
    return 0;

}