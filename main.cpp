#include "filesystem" // include the header file for the C++17 Filesystem library
#include "./external/porter2_stemmer.h" // include the header file for the Porter2 stemmer library
#include "bits/stdc++.h" // include a standard C++ header file that includes most other standard header files
using namespace std; // use the standard namespace
namespace fs=std::filesystem; // create an alias for the Filesystem namespace

struct unit{ // define a struct to store word frequency information
    string name; // a string to store the name of the article the word is in
    int times; // an integer to store the number of times the word appears in the article
    unit(const string& n){times=1;name=n;} // a constructor that sets the article name and initializes the word count to 1
};

std::map<string,vector<unit>> dict; // create a map to store the word frequency information

void file_parser(ifstream& file,std::map<string,vector<unit>>&dict,string articleName) // define a function to parse a file
{
    string temp,word; // create string variables to store temporary information and individual words
    bool inword=0; // create a boolean flag to check if currently parsing a word
    while(getline(file,temp)){ // read the file line by line
        for(int i=0;temp[i];i++) // iterate over each character in the line
        {
            if(std::isalnum(temp[i])) // check if the character is alphanumeric
            {
                if(inword==0) inword=1; // if not currently parsing a word, set flag to true
                word.push_back(temp[i]); // add the character to the word string
            }
            else if(inword) // if not alphanumeric and currently parsing a word
            {
                Porter2Stemmer::trim(word); // use Porter2 stemmer to trim the word
                Porter2Stemmer::stem(word); // use Porter2 stemmer to stem the word
                if(word.size()==0) continue; // if the word has no length, skip to the next iteration
                if(dict.count(word)==0||articleName!=(dict[word].end()-1)->name) // if the word does not already exist in the map or if the last article in the vector is not the current article
                    dict[word].push_back(unit(articleName)); // add the word and article information to the map
                else
                    (dict[word].end()-1)->times++; // if the word already exists in the map and the last article in the vector is the current article, increment the word count
                inword=0,word.clear(); // set the flag to false and clear the word string
            }

        }
    }
}

void load_dict(string& dict_base,std::map<string,vector<unit>>&dict) // define a function to load the map into a file
{
    fstream f(dict_base+"/dict.txt",ios::out); // open the output file stream
    for(auto &p:dict) // iterate over each key-value pair in the map
    {
        string word=p.first; // get the word string
        f<<p.first<<endl<<p.second.size()<<endl; // write the word and the number of articles to the file
        for(auto &arc:p.second) // iterate over each article in the vector
            f<<arc.name<<endl<<arc.times<<endl; // write the article name and word count to the file
    }
    return; //
}


int main()
{
    // Define the base path where the sample files are stored
    fs::path base("../sample");

    // Define the base path where the inverted index dictionary will be stored
    fs::path dict_base("../dict");

    // Check if the dictionary directory exists, and if not, create it
    if(!fs::is_directory(dict_base))
        fs::create_directory(dict_base);

    // Loop through each subdirectory in the base directory
    for(auto &v:fs::directory_iterator(base))
    {
        // Get the name of the subdirectory
        std::string filename=v.path().filename().string();

        // Print out the name of the directory
        cout<<"directory:"<<filename<<endl;

        // Loop through each file in the subdirectory
        for(auto &tv:fs::directory_iterator(base/filename))
        {
            // Get the full path of the file
            string full_path=tv.path().string();

            // Open the file for reading
            ifstream file(full_path,ios::in);

            // Create a string representing the article name (directory name + file name)
            string article_name=filename+" "+tv.path().filename().string();

            // Parse the file and add its words to the inverted index dictionary
            file_parser(file,dict,article_name);
        }
    }

    // Load the inverted index dictionary from disk
    load_dict(dict_base.string(),dict);

    // End the program
    return 0;
}
