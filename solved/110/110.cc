#include <algorithm>
#include <iostream>

const int MAX_NUM_VARIABLES = 8;

char GetVarName(int var_index) { return "abcdefgh"[var_index]; }

void PrintVars(const int vars[], int num_vars) {
  for (int i = 0; i < num_vars; i++) {
    if (i > 0)
      std::cout << ',';
    std::cout << GetVarName(vars[i]);
  }
}

void PrintIndent(int level) {
  while (level--)
    std::cout << "  ";
}

void Generate(int depth, int num_vars, int vars[]) {
  if (depth == num_vars) {
    PrintIndent(depth);
    std::cout << "writeln(";
    PrintVars(vars, num_vars);
    std::cout << ")" << std::endl;
    return;
  }

  int child_vars[MAX_NUM_VARIABLES];
  std::copy(vars, vars + num_vars, child_vars);
  for (int i = depth - 1; i >= 0; i--) {
    PrintIndent(depth);
    if (i != depth - 1)
      std::cout << "else ";
    std::cout << "if " << GetVarName(child_vars[i])
              << " < " << GetVarName(child_vars[i + 1])
              << " then" << std::endl;
    Generate(depth + 1, num_vars, child_vars);
    std::swap(child_vars[i], child_vars[i + 1]);
  }

  PrintIndent(depth);
  std::cout << "else" << std::endl;
  Generate(depth + 1, num_vars, child_vars);
}

int main() {
  int num_programs;
  std::cin >> num_programs;
  while (num_programs--) {
    int num_variables, variables[MAX_NUM_VARIABLES];
    std::cin >> num_variables;
    for (int i = 0; i < num_variables; i++)
      variables[i] = i;
    std::cout << "program sort(input,output);" << std::endl
              << "var" << std::endl;
    PrintVars(variables, num_variables);
    std::cout << " : integer;" << std::endl
              << "begin" << std::endl
              << "  readln(";
    PrintVars(variables, num_variables);
    std::cout << ");" << std::endl;
    if (num_variables == 1)
      std::cout << "  writeln(a)" << std::endl;
    else
      Generate(1, num_variables, variables);
    std::cout << "end." << std::endl;
    if (num_programs > 0)
      std::cout << std::endl;
  }
  return 0;
}
