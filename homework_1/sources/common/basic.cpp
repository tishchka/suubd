#include <basic.hpp>
#include <iostream>
#include <student.hpp>

size_t BasicPlan::CountExcellents() const {
  size_t excellents = 0;
  for (const auto& s : students_) {
    for (const Student::Discipline& d : s.GetDisciplines()) {
      if (s.DSemester(d) == s.GetSemester() && s.DGrade(d) != Grade::Excellent) continue;
      ++excellents;
    }
  }
  return excellents;
}
