#ifndef ABSTRACT_HPP_
#define ABSTRACT_HPP_

#include <algorithm>
#include <cstddef>
#include <extra.hpp>
#include <iostream>
#include <string>
#include <student.hpp>

class Base {
 public:
  Base() noexcept {};
  Base(const std::vector<Student>& students) : students_(students) {}
  virtual ~Base() noexcept {};

  virtual size_t CountExcellents() const = 0;

  size_t Peek() noexcept;
  Student& GetStudent(size_t index) { return students_[index]; }
  bool Seek(const std::string& name) const;

  const std::vector<Student>& Students() const noexcept { return students_; }
  size_t AmountOfStudents() const noexcept { return students_.size(); }
  void AddStudent(const Student& student) noexcept { students_.push_back(student); }
  void RemoveStudent(size_t index) { students_.erase(students_.begin() + index); }

  void ListStudent(const Student& s) const noexcept;
  void ListStudents() const noexcept;

  void SortByName() noexcept;
  void SortBySemester() noexcept;

  std::vector<Student> GroupByDepartment(const std::string& dep) const;
  std::vector<Student> GroupByDiscipline(const std::string& dis) const;

 protected:
  std::vector<Student> students_;
};

#endif  // ABSTRACT_HPP_
