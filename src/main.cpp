#include <iostream>
#include <sstream>
#include<vector>
#include <unordered_set>
#include <cstdlib>
#include <filesystem>
using namespace std;

string find_executable(string exe_path, vector<string>exec_dirs){
  for(auto dir: exec_dirs){
    string path = dir + "/" + exe_path;
    if(std::filesystem::exists(path)){
      return path;
    }
  }
  return "";
}

int main()
{
  // Flush after every std::cout / std:cerr
  std::cout << std::unitbuf;
  std::cerr << std::unitbuf;

  string env_variable = "PATH";
  char* path= getenv(env_variable.c_str());
  vector<string>exec_dirs;
  stringstream ss(path);
  string token;
  while(getline(ss,token,':')){
    exec_dirs.push_back(token);
  }

  bool end_program = false;
  while (!end_program)
  {
    // Taking the user input
    cout << "$ ";
    string input;
    getline(cin, input);

    stringstream s(input);
    vector<string> arguments;
    string argument;
    while (s >> argument)
    {
      arguments.push_back(argument);
    }

    if(arguments[0]=="exit"){
      if(arguments.size() <=1) cerr<<"exit code not provided"<<endl;
      if (arguments[1] == "0") return 0;
    }
    else if(arguments[0]=="echo"){
      for(int i=1;i<arguments.size();i++) cout<<arguments[i]<<" ";
      cout<<endl;
    }else if(arguments[0]=="type"){
      string exe_path = find_executable(arguments[1],exec_dirs);
      if(exe_path != ""){
        cout<<arguments[1]<<" is "<<exe_path<<endl;
      }else{
        cout<<arguments[1]<<": not found"<<endl;
      }
    }
    else{
      cout<<input<<": command not found"<<endl;
    }

  }
}
