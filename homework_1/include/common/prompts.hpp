#ifndef PROMPTS_HPP_
#define PROMPTS_HPP_

#include <iostream>
#include <student.hpp>

inline size_t PromptForStudents(const std::vector<Student>& students) {
  for (size_t i = 0; i < students.size(); ++i) {
    std::cout << std::setw(30);
    std::cout << Bold(std::to_string(i + 1)) << " - ";
    std::cout << students[i].stringify();
    if (i % 3 == 2 || i == students.size() - 1) std::cout << std::endl;
  }
  size_t index = 0;
  std::cout << "Choose a student: ";
  std::cin >> index;
  return index;
}

inline Student PromptForStudent() {
  Student student;
  
  std::cout << "name: ";
  std::string s;
  std::cin >> s;
  student.SetName(s);
  std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');

  std::cout << "department: ";
  std::cin >> s;
  student.SetDepartment(s);
  std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');

  std::cout << "semester: ";
  size_t semester = 1;
  std::cin >> semester;
  student.SetSemester(semester);
  std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');

  while (true) {
    std::cout << "discipline (blank to finish): ";
    std::getline(std::cin, s);
    if (s.empty()) {
      break;
    }

    Student::Discipline d;
    d.first.first = s;

    std::cout << "discipline's semester: ";
    semester = 1;
    std::cin >> semester;
    std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
    d.first.second = semester;

    std::cout << "grade: ";
    size_t grade = 1;
    std::cin >> grade;
    std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
    d.second = static_cast<Grade>(grade);

    student.AddDiscipline(d);
  }

  return student;
}

#endif  // PROMPTS_HPP_
