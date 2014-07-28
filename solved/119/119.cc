#include <iostream>
#include <map>
#include <vector>

int main() {
  bool first_group = true;
  int num_peoples;
  while (std::cin >> num_peoples) {
    std::vector<std::string> names;
    std::map<std::string, int> balances;
    for (int i = 0; i < num_peoples; i++) {
      std::string name;
      std::cin >> name;
      names.push_back(name);
      balances[name] = 0;
    }

    for (int i = 0; i < num_peoples; i++) {
      std::string giver;
      int giving, total, num_givings;
      std::cin >> giver >> total >> num_givings;
      if (num_givings == 0) {
        continue;
      }
      giving = total / num_givings;
      balances[giver] -= giving * num_givings;
      for (int j = 0; j < num_givings; j++) {
        std::string receiver;
        std::cin >> receiver;
        balances[receiver] += giving;
      }
    }

    if (!first_group) {
      std::cout << std::endl;
    }
    first_group = false;

    for (const auto& name : names) {
      std::cout << name << ' ' << balances[name] << std::endl;
    }
  }
  return 0;
}
