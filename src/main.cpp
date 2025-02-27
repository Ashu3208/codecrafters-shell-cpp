#include <iostream>
#include <sstream>
#include<vector>
#include<unordered_set>
using namespace std;

int main()
{
  // Flush after every std::cout / std:cerr
  std::cout << std::unitbuf;
  std::cerr << std::unitbuf;

  bool end_program = false;
  unordered_set<string> builtins={"echo","cd","pwd", "exit"};
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
      if(builtins.find(arguments[1]) != builtins.end()){
        cout<<arguments[1]<<" is a shell builtin"<<endl;
      }else{
        cout<<arguments[1]<<": not found"<<endl;
      }
    }
    else{
      std::cout << input << ": command not found" << std::endl;
    }

  }
}
