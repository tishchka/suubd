#include <base.hpp>
#include <extra.hpp>
#include <iomanip>
#include <prompts.hpp>

size_t Base::Peek() noexcept { return PromptForStudents(students_) - 1; }

bool Base::Seek(const std::string& name) const {
  for (const auto& s : students_) {
    if (s.GetName() == name) return true;
  }
  return false;
}

void Base::ListStudent(const Student& s) const noexcept {
  std::cout << s.stringify() << std::endl << "| ";
  for (const auto& d : s.GetDisciplines()) {
    std::cout << d.first.first << '-' << d.first.second << ": " << static_cast<int>(d.second) << " | ";
  }
  std::cout << std::endl;
}

void Base::ListStudents() const noexcept {
  for (const auto& s : students_) ListStudent(s);
}

void Base::SortByName() noexcept {
  std::sort(students_.begin(), students_.end(), [](const auto& a, const auto& b) { return a.GetName() < b.GetName(); });
}

void Base::SortBySemester() noexcept {
  std::sort(students_.begin(), students_.end(),
            [](const auto& a, const auto& b) { return a.GetSemester() > b.GetSemester(); });
}

std::vector<Student> Base::GroupByDepartment(const std::string& dep) const {
  std::vector<Student> group;
  for (const auto& s : students_) {
    if (s.GetDepartment() == dep) group.push_back(s);
  }
  return group;
}

std::vector<Student> Base::GroupByDiscipline(const std::string& dis) const {
  std::vector<Student> group;
  for (const auto& s : students_) {
    for (const auto& d : s.GetDisciplines()) {
      if (d.first.first == dis) {
        group.push_back(s);
        break;
      }
    }
  }
  return group;
}
