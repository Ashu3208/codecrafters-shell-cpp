#include <iostream>

int main()
{
  // Flush after every std::cout / std:cerr
  std::cout << std::unitbuf;
  std::cerr << std::unitbuf;

  // Uncomment this block to pass the first stage
  bool end_program = false;
  while (!end_program)
  {
    std::cout << "$ ";
    std::string input;
    std::getline(std::cin, input);
    if(input == "exit")return 0;
    std::cout << input << ": command not found" << std::endl;
  }
}
