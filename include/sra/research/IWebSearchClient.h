#pragma once

#include "sra/domain/Evidence.h"
#include <string>
#include <vector>

namespace sra::research {

class IWebSearchClient {
public:
    virtual ~IWebSearchClient() = default;
    virtual std::vector<domain::Evidence> search(const std::string& query, int maxResults = 5) const = 0;
};

} // namespace sra::research