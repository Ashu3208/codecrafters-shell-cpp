#include <iostream>
#include <sstream>
#include <vector>
#include <unordered_set>
#include <cstdlib>
#include <filesystem>
#include <unistd.h>
#include <sys/wait.h>
using namespace std;

unordered_set<string> builtins{"echo", "exit", "type", "pwd", "cd"};
vector<string> exec_dirs;

vector<string> parse_arguments(const string &input)
{
  vector<string> arguments;
  string argument;
  bool in_quotes = false, in_double_quotes = false;

  for (size_t i = 0; i < input.length(); i++)
  {
    char c = input[i];
    if (c == '"' and !in_quotes)
    {
      in_double_quotes = !in_double_quotes;
      continue;
    }
    if (c == '\'' and !in_double_quotes)
    {
      in_quotes = !in_quotes; // Toggle quote state
      continue;               // Don't include the quote itself
    }

    if (!in_quotes && !in_double_quotes && c == ' ')
    {
      if (!argument.empty())
      {
        arguments.push_back(argument);
        argument.clear();
      }
    }
    else
    {
      argument += c;
    }
  }

  if (!argument.empty())
  {
    arguments.push_back(argument); // Push last token
  }

  return arguments;
}

string find_executable(string exe_path, vector<string> exec_dirs)
{
  for (auto dir : exec_dirs)
  {
    string path = dir + "/" + exe_path;
    if (std::filesystem::exists(path))
    {
      return path;
    }
  }
  return "";
}

void run_builtin_commands(string command, vector<string> &args)
{
  if (command == "pwd")
  {
    cout << getcwd(nullptr, 0) << endl;
  }
  else if (command == "type")
  {
    if (builtins.count(args[0]))
    {
      cout << args[0] << " is a shell builtin" << endl;
      return;
    }
    string exe_path = find_executable(args[0], exec_dirs);
    if (exe_path != "")
    {
      cout << args[0] << " is " << exe_path << endl;
    }
    else
    {
      cout << args[0] << ": not found" << endl;
    }
  }
  else if (command == "exit")
  {
    if (args.size() == 0)
      cerr << "exit code not provided" << endl;
    else
      exit(stoi(args[0]));
  }
  else if (command == "echo")
  {
    for (auto i : args)
      cout << i << " ";
    cout << endl;
  }
  else if (command == "cd")
  {
    if (args.size() == 0)
      cerr << "cd: path not provided" << endl;
    else if (args.size() > 1)
      cerr << "cd: too many arguments" << endl;
    else if (args[0] == "~")
    {
      chdir(getenv("HOME"));
    }
    else if (chdir(args[0].c_str()) != 0)
      cerr << "cd: " << args[0] << ": No such file or directory" << endl;
  }
}

int main()
{
  // Flush after every std::cout / std:cerr
  std::cout << std::unitbuf;
  std::cerr << std::unitbuf;

  string env_variable = "PATH";
  char *path = getenv(env_variable.c_str());

  stringstream ss(path);
  string token;
  while (getline(ss, token, ':'))
  {
    exec_dirs.push_back(token);
  }

  bool end_program = false;
  while (!end_program)
  {
    // Taking the user input
    cout << "$ ";
    string input;
    getline(cin, input);

    vector<string> arguments = parse_arguments(input);

    if (builtins.count(arguments[0]))
    {
      vector<string> args(arguments.begin() + 1, arguments.end());
      run_builtin_commands(arguments[0], args);
    }
    else
    {

      string exe_path = find_executable(arguments[0], exec_dirs);
      int pid = fork();
      if (exe_path != "" && pid == 0)
      {
        // Run the executable
        char *args[arguments.size() + 1];
        for (size_t i = 0; i < arguments.size(); i++)
        {
          args[i] = const_cast<char *>(arguments[i].c_str());
        }
        args[arguments.size()] = nullptr;
        execvp(exe_path.c_str(), args);
        // If execvp returns, it means there was an error
        perror("execvp failed");
        exit(1);
      }
      if (pid == 0)
        return 0;
      wait(nullptr);
      if (exe_path == "")
      {
        cout << input << ": command not found" << endl;
      }
    }
  }
}
