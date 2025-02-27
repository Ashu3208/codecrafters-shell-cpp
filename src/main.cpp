#include <iostream>
#include <sstream>
#include<vector>
using namespace std;

int main()
{
  // Flush after every std::cout / std:cerr
  cout << unitbuf;
  cerr << unitbuf;

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
    }
    else{
      std::cout << input << ": command not found" << std::endl;
    }

  }
}
