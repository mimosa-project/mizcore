#pragma once

#include <memory>
#include <vector>

#include "error_object.hpp"

namespace mizcore {

class ErrorTable
{
  public:
    // ctor, dtor
    ErrorTable() = default;
    virtual ~ErrorTable() = default;
    ErrorTable(ErrorTable const&) = delete;
    ErrorTable(ErrorTable&&) = delete;
    ErrorTable& operator=(ErrorTable const&) = delete;
    ErrorTable& operator=(ErrorTable&&) = delete;

    // attributes
    void AddError(ErrorObject* error);

    // operation
    void LogErrors();

    // implementation
  private:
    void SortErrors();

  private:
    std::vector<std::unique_ptr<ErrorObject>> errors_;
};

} // namespace mizcore
