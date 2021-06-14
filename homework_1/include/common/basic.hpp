#ifndef BASIC_HPP_
#define BASIC_HPP_

#include <base.hpp>
#include <student.hpp>
#include <vector>

class BasicPlan : public Base {
 public:
  BasicPlan() noexcept = default;
  BasicPlan(const std::vector<Student>& students) noexcept : Base(students) {}
  ~BasicPlan() noexcept = default;

  virtual size_t CountExcellents() const override;  // over current semester
};

#endif  // BASIC_HPP_
