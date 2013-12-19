#include <algorithm>
#include <cstring>
#include <iostream>
#include <sstream>
#include <vector>

#ifdef ONLINE_JUDGE
#define NDEBUG
#endif
#include <assert.h>

enum {
  MAX_TABLE_LENGTH = 25
};

class BlockWorld {
 private:
  typedef std::vector<int>::const_iterator const_iterator;
  typedef std::vector<int>::iterator iterator;

 public:
  explicit BlockWorld(int num_blocks) : num_blocks_(num_blocks) {
    for (int i = 0; i < num_blocks_; i++) {
      table[i].push_back(i);
    }
  }

  void Execute(const std::string& command_line) {
    std::string command, preposition;
    int block_a, block_b;

    std::istringstream command_stream(command_line);
    command_stream >> command >> block_a >> preposition >> block_b;

    if (block_a == block_b) {
      return;  // Illegal command.
    }

    int stack_a = Find(block_a);
    assert(stack_a != -1);

    int stack_b = Find(block_b);
    assert(stack_b != -1);

    if (stack_a == stack_b) {
      return;  // Illegal command.
    }

    if (command == "move" && preposition == "onto") {
      MoveOnto(block_a, block_b, stack_a, stack_b);
    } else if (command == "move" && preposition == "over") {
      MoveOver(block_a, block_b, stack_a, stack_b);
    } else if (command == "pile" && preposition == "onto") {
      PileOnto(block_a, block_b, stack_a, stack_b);
    } else if (command == "pile" && preposition == "over") {
      PileOver(block_a, block_b, stack_a, stack_b);
    } else {
      assert(0);  // Syntax error.
    }
  }

  int Find(int block) {
    for (int pos = 0; pos < num_blocks_; pos++) {
      const std::vector<int>& stack = table[pos];
      if (stack.end() != find(stack.begin(), stack.end(), block)) {
        return pos;
      }
    }
    return -1;
  }

  void MoveOnto(int block_a, int block_b, int stack_a, int stack_b) {
    ReturnBlocksAbove(stack_a, block_a);
    ReturnBlocksAbove(stack_b, block_b);
    MoveTopBlock(stack_a, stack_b);
  }

  void MoveOver(int block_a, int, int stack_a, int stack_b) {
    ReturnBlocksAbove(stack_a, block_a);
    MoveTopBlock(stack_a, stack_b);
  }

  void PileOnto(int block_a, int block_b, int stack_a, int stack_b) {
    ReturnBlocksAbove(stack_b, block_b);
    MoveBlocks(stack_a, stack_b, block_a);
  }

  void PileOver(int block_a, int, int stack_a, int stack_b) {
    MoveBlocks(stack_a, stack_b, block_a);
  }

  void ReturnBlocksAbove(int stack, int block) {
    int b;
    while ((b = table[stack].back()) != block) {
      table[b].push_back(b);
      table[stack].pop_back();
    }
  }

  void MoveBlocks(int stack_from, int stack_to, int block) {
    iterator end = table[stack_from].end();
    iterator pos = find(table[stack_from].begin(), end, block);
    assert(pos != end);
    for (; pos != end; ++pos) {
      table[stack_to].push_back(*pos);
    }
    // Remove blocks above and itself.
    int b;
    while ((b = table[stack_from].back()) != block) {
      table[stack_from].pop_back();
    }
    table[stack_from].pop_back();
  }

  void MoveTopBlock(int stack_from, int stack_to) {
    int block = table[stack_from].back();
    table[stack_to].push_back(block);
    table[stack_from].pop_back();
  }

  void Output(std::ostream& out) const {
    for (int i = 0; i < num_blocks_; i++) {
      out << i << ':';
      const std::vector<int>& stack = table[i];
      for (const_iterator b = stack.begin(), e = stack.end(); b != e; ++b) {
        out << ' ' << *b;
      }
      out << std::endl;
    }
  }

 private:
  std::vector<int> table[MAX_TABLE_LENGTH];
  int num_blocks_;
};

int main() {
  std::string command_line;

  int num_blocks;
  std::getline(std::cin, command_line);
  assert(std::cin.good());
  std::istringstream(command_line) >> num_blocks;

  BlockWorld world(num_blocks);

  while (std::getline(std::cin, command_line)) {
    if (!strncmp(command_line.c_str(), "quit", 4)) {
      break;
    }
    world.Execute(command_line);
  }

  world.Output(std::cout);

  return 0;
}
