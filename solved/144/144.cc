#include <cstring>
#include <iomanip>
#include <iostream>

struct Student {
  int id, amount;
};

enum { MAX_NUM_STUDENTS = 25, INITIAL_AMOUNT = 40, };

int main() {
  for (int num_students, limit;
       std::cin >> num_students >> limit && num_students && limit;) {
    Student students[MAX_NUM_STUDENTS + 1];
    for (int i = 0; i < num_students; i++) {
      students[i].id = i + 1;
      students[i].amount = INITIAL_AMOUNT;
    }

    int next_output = 1;
    int output_store = 0;
    while (num_students > 0) {
      if (output_store == 0) {
        output_store = next_output;
        next_output++;
        if (next_output > limit) {
          next_output = 1;
        }
      }

      int amount = std::min(students[0].amount, output_store);
      students[0].amount -= amount;
      output_store -= amount;
      if (students[0].amount == 0) {
        std::cout << std::setw(3) << students[0].id;
        num_students--;
      } else {
        students[num_students] = students[0];
      }
      ::memmove(students, students + 1, sizeof(Student) * num_students);
    }

    std::cout << std::endl;
  }
  return 0;
}
