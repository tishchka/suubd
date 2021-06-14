#ifndef INDIVIDUAL_HPP_
#define INDIVIDUAL_HPP_

#include <base.hpp>
#include <map>
#include <vector>

class IndividualPlan : public Base {
 public:
  IndividualPlan() noexcept = default;
  IndividualPlan(const std::vector<Student>& students) noexcept : Base(students) {}
  ~IndividualPlan() noexcept = default;

  virtual size_t CountExcellents() const override;  // over all disciplines
};

#endif  // INDIVIDUAL_HPP_