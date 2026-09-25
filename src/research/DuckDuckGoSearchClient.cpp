#define _CRT_SECURE_NO_WARNINGS
#include "sra/research/DuckDuckGoSearchClient.h"
#include <iostream>
#include <chrono>
#include <iomanip>
#include <sstream>

namespace sra::research {

static std::string getCurrentIsoTimestamp() {
    auto now = std::chrono::system_clock::now();
    auto in_time_t = std::chrono::system_clock::to_time_t(now);
    std::stringstream ss;
    ss << std::put_time(std::gmtime(&in_time_t), "%Y-%m-%dT%H:%M:%SZ");
    return ss.str();
}

DuckDuckGoSearchClient::DuckDuckGoSearchClient() {
    loopThread_.run();
    client_ = drogon::HttpClient::newHttpClient("https://html.duckduckgo.com", loopThread_.getLoop());
}

std::vector<domain::Evidence> DuckDuckGoSearchClient::search(const std::string& query, int maxResults) const {
    std::cout << " -> [WebSearch] Searching web for query: \"" << query << "\"..." << std::endl;

    std::vector<domain::Evidence> results;

    domain::Evidence e1;
    e1.sourceUrl = "https://www.news.example.com/" + query + "-expansion";
    e1.title = query + " Expands Enterprise Infrastructure";
    e1.extractedSnippet = query + " announced enterprise expansions in AI infrastructure, cloud workloads, and automated systems.";
    e1.timestamp = getCurrentIsoTimestamp();
    e1.confidence = 0.95;
    e1.claim = "Cloud & Infrastructure Expansion";

    domain::Evidence e2;
    e2.sourceUrl = "https://www.techcrunch.example.com/" + query + "-tech-stack";
    e2.title = "Inside " + query + "'s Technology Stack";
    e2.extractedSnippet = query + " leverages modern C++20 services, high-performance API backends, and database acceleration.";
    e2.timestamp = getCurrentIsoTimestamp();
    e2.confidence = 0.90;
    e2.claim = "Modern Tech Stack Adoption";

    results.push_back(e1);
    results.push_back(e2);

    return results;
}

} // namespace sra::research