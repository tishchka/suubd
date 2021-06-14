#include <individual.hpp>
#include <iostream>

size_t IndividualPlan::CountExcellents() const {
  size_t excellents = 0;
  for (const auto& s : students_) {
    for (const Student::Discipline& d : s.GetDisciplines()) {
      if (s.DGrade(d) != Grade::Excellent) continue;
      ++excellents;
    }
  }
  return excellents;
}
