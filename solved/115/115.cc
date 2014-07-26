#include <cassert>
#include <cstdlib>
#include <iostream>
#include <map>
#include <vector>

enum {
#ifdef ONLINE_JUDGE
  DEBUG = 0,
#else
  DEBUG = 1,
#endif  // ONLINE_JUDGE
};

class Person {
 public:
  typedef std::vector<Person*>::const_iterator const_iterator;

  Person();
  explicit Person(std::string name);

  const std::string& name() const;

  void SetParent(Person* parent);
  const Person* GetParent() const;

  void AddChild(Person* child);
  const_iterator cbegin() const;
  const_iterator cend() const;

 private:
  std::string name_;
  Person* parent_;
  std::vector<Person*> children_;
};

typedef std::map<std::string, Person> Persons;

void PrintFamilyTree(const Person* person, int level, std::ostream& out);

int ComputeAncestorNumber(const Person* ancestor, const Person* descedent);
bool IsAncestorOf(const Person* ancestor, const Person* descedent);

const Person* FindCommonAncestor(const Person* p, const Person* q);

int main() {
  Persons persons;
  std::vector<Person*> roots;
  std::string name1, name2;

  while (std::cin >> name1 >> name2 && name1 != "no.child" &&
         name2 != "no.parent") {
    Persons::iterator child =
        persons.insert(std::make_pair(name1, Person(name1))).first;
    Persons::iterator parent =
        persons.insert(std::make_pair(name2, Person(name2))).first;
    parent->second.AddChild(&child->second);
    child->second.SetParent(&parent->second);
  }
  for (auto& pair : persons) {
    Person& person = pair.second;
    if (!person.GetParent()) {
      roots.push_back(&person);
    }
  }
  if (DEBUG) {
    for (const Person* person : roots) {
      PrintFamilyTree(person, 0, std::cerr);
      std::cerr << "---" << std::endl;
    }
  }

  while (std::cin >> name1 >> name2) {
    const Person* p =
        &persons.insert(std::make_pair(name1, Person(name1))).first->second;
    const Person* q =
        &persons.insert(std::make_pair(name2, Person(name2))).first->second;
    if (p == q) {
      std::cout << "no relation" << std::endl;
      continue;
    }

    const Person* ancestor = FindCommonAncestor(p, q);
    if (DEBUG) {
      std::cerr << "  p=" << p->name() << std::endl;
      std::cerr << "  q=" << q->name() << std::endl;
      if (ancestor) {
        std::cerr << "  ancestor=" << ancestor->name() << std::endl;
      }
    }

    if (!ancestor) {
      std::cout << "no relation" << std::endl;
      continue;
    }

    if (p == ancestor || q == ancestor) {
      const Person* descendent = p == ancestor ? q : p;
      const char* const relation = p == ancestor ? "parent" : "child";
      int k = ComputeAncestorNumber(ancestor, descendent);
      if (DEBUG) {
        std::cerr << "  k=" << k << std::endl;
      }
      while (k-- >= 2) {
        std::cout << "great ";
      }
      if (k >= 0) {
        std::cout << "grand ";
      }
      std::cout << relation << std::endl;
      continue;
    }

    int m = ComputeAncestorNumber(ancestor, p);
    int n = ComputeAncestorNumber(ancestor, q);
    if (DEBUG) {
      std::cerr << "  m=" << m << std::endl;
      std::cerr << "  n=" << n << std::endl;
    }
    if (m == 0 && n == 0) {
      std::cout << "sibling" << std::endl;
    } else {
      std::cout << std::min(n, m) << " cousin";
      if (abs(n - m)) {
        std::cout << " removed " << abs(n - m);
      }
      std::cout << std::endl;
    }
  }
  return 0;
}

void PrintFamilyTree(const Person* person, int level, std::ostream& out) {
  for (int i = 0; i < level; i++) {
    out << "  ";
  }
  out << person->name() << std::endl;
  for (Person::const_iterator i = person->cbegin(); i != person->cend(); ++i) {
    PrintFamilyTree(*i, level + 1, out);
  }
}

int ComputeAncestorNumber(const Person* ancestor, const Person* descedent) {
  assert(descedent);
  for (int k = 0; (descedent = descedent->GetParent()) != nullptr; k++) {
    if (descedent == ancestor) {
      return k;
    }
  }
  return -1;
}

bool IsAncestorOf(const Person* ancestor, const Person* descedent) {
  return ComputeAncestorNumber(ancestor, descedent) >= 0;
}

const Person* FindCommonAncestor(const Person* p, const Person* q) {
  while (p) {
    if (p == q || IsAncestorOf(p, q)) {
      return p;
    }
    p = p->GetParent();
  }
  return nullptr;
}

Person::Person() : parent_(nullptr) {}
Person::Person(std::string name) : name_(std::move(name)), parent_(nullptr) {}

const std::string& Person::name() const { return name_; }

void Person::SetParent(Person* parent) { parent_ = parent; }
const Person* Person::GetParent() const { return parent_; }

void Person::AddChild(Person* child) { children_.push_back(child); }
Person::const_iterator Person::cbegin() const { return children_.cbegin(); }
Person::const_iterator Person::cend() const { return children_.cend(); }
