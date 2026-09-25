#pragma once

#include "sra/research/IWebSearchClient.h"
#include <trantor/net/EventLoopThread.h>
#include <drogon/HttpClient.h>

namespace sra::research {

class DuckDuckGoSearchClient : public IWebSearchClient {
public:
    DuckDuckGoSearchClient();
    ~DuckDuckGoSearchClient() override = default;

    std::vector<domain::Evidence> search(const std::string& query, int maxResults = 5) const override;

private:
    mutable trantor::EventLoopThread loopThread_;
    mutable drogon::HttpClientPtr client_;
};

} // namespace sra::research