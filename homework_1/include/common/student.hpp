#ifndef STUDENT_HPP_
#define STUDENT_HPP_

#include <map>
#include <string>
#include <vector>

enum class Grade { Unsatisfactorily = 2, Satisfactorily, Good, Excellent };

class Student {
 public:
  // Discipline: ((name, semester), grade)
  using Discipline = std::pair<std::pair<std::string, int>, Grade>;
  using Disciplines = std::vector<Discipline>;

  Student(std::string name, std::string department, int semester, Disciplines disciplines)
      : name_(name), department_(department), semester_(semester), disciplines_(disciplines) {}

  Student() noexcept = default;
  Student(const Student& other) noexcept = default;
  Student(Student&&) noexcept = default;
  Student& operator=(const Student&) noexcept = default;
  Student& operator=(Student&&) noexcept = default;

  std::string GetName() const noexcept { return name_; }
  int GetSemester() const noexcept { return semester_; }
  std::string GetDepartment() const noexcept { return department_; }
  Disciplines GetDisciplines() const noexcept { return disciplines_; }

  void AddDiscipline(const Discipline& d) noexcept { disciplines_.push_back(d); }

  void SetName(const std::string& name) { name_ = name; }
  void SetSemester(int semester) { semester_ = semester; }
  void SetDepartment(const std::string& department) { department_ = department; }

  Grade DGrade(const Discipline& discipline) const noexcept { return discipline.second; }
  std::string DName(const Discipline& discipline) const noexcept { return discipline.first.first; }
  int DSemester(const Discipline& discipline) const noexcept { return discipline.first.second; }

  std::string stringify() const noexcept {
    std::string str = Red(GetName()) + ' ' + Green(GetDepartment()) + ' ';
    str += Blue(std::to_string(GetSemester()));
    return str;
  }

 private:
  std::string name_;
  std::string department_;
  int semester_ = 1;
  Disciplines disciplines_;
};

#endif  // STUDENT_HPP_
