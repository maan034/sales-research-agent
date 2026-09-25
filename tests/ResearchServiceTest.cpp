#include <gtest/gtest.h>
#include "sra/research/ResearchService.h"
#include "sra/research/ResearchProvider.h"
#include "sra/domain/ResearchResult.h"
#include <memory>

class FakeResearchProvider : public sra::research::ResearchProvider {
public:
    sra::domain::ResearchResult research(const std::string& companyName) const override {
        sra::domain::ResearchResult result;
        result.company.name = companyName;
        result.icpScore = 90;
        return result;
    }
};

TEST(ResearchServiceTest, ResearchCompanyReturnsValidResult) {
    auto provider = std::make_unique<FakeResearchProvider>();
    sra::research::ResearchService service(std::move(provider));

    auto result = service.researchCompany("TestCorp");
    EXPECT_EQ(result.company.name, "TestCorp");
    EXPECT_EQ(result.icpScore, 90);
}