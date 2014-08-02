#include <algorithm>
#include <iostream>

void Generate(int num_elements, int* sequence_index, std::string* sequence);
bool IsHardSequenceBackward(const std::string& sequence);
void PrintSequence(const std::string& sequence);

int main() {
  for (int sequence_index, num_elements;
       std::cin >> sequence_index >> num_elements;) {
    if (sequence_index == 0 || num_elements == 0) {
      break;
    }
    std::string sequence;
    sequence.reserve(80);
    Generate(num_elements, &sequence_index, &sequence);
  }
  return 0;
}

void Generate(int num_elements, int* sequence_index, std::string* sequence) {
  if (*sequence_index == 0) {
    return;
  }

  sequence->push_back('\0');
  for (int i = 0; i < num_elements; i++) {
    sequence->back() = 'A' + i;
    if (!IsHardSequenceBackward(*sequence)) {
      continue;
    }

    (*sequence_index)--;
    if (*sequence_index == 0) {
      PrintSequence(*sequence);
      break;
    }

    Generate(num_elements, sequence_index, sequence);
    if (*sequence_index == 0) {
      break;
    }
  }
  sequence->pop_back();
}

bool IsHardSequenceBackward(const std::string& sequence) {
  std::string p, q;
  int size = static_cast<int>(sequence.size());
  for (int step = 1; step * 2 <= size; step++) {
    auto i = sequence.cend(), j = i - step;
    if (j - step < sequence.cbegin()) {
      continue;
    }
    if (p.assign(j - step, j) == q.assign(i - step, i)) {
      return false;
    }
  }
  return true;
}

void PrintSequence(const std::string& sequence) {
  int count = 0;
  std::string chunk;
  for (auto i = sequence.cbegin(); i < sequence.cend(); i += 4) {
    if (count > 0 && count % 16 == 0) {
      std::cout << std::endl;
    } else if (count % 16 != 0) {
      std::cout << ' ';
    }
    std::cout << chunk.assign(i, std::min(i + 4, sequence.cend()));
    count++;
  }
  std::cout << std::endl << sequence.size() << std::endl;
}
