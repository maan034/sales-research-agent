#include "sra/research/DemoResearchProvider.h"

namespace sra::research {

sra::domain::ResearchResult DemoResearchProvider::research(const std::string& companyName) const {
    sra::domain::ResearchResult result;
    result.company.name = companyName;
    result.company.industry = "Technology";
    result.company.website = "https://example.com";
    result.company.description = "Automated sales intelligence platform";

    result.technologySignals.push_back({"Cloud", "AWS Multi-region deployment", "DNS", 0.95});
    result.businessSignals.push_back({"Hiring", "Expanding sales team", "Job Board", 0.85});
    
    result.icpScore = 85;
    result.recommendation = "High Priority Outreach";

    return result;
}

} // namespace sra::research