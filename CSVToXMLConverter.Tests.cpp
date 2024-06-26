#include <gtest/gtest.h>
#include <gmock/gmock.h>
#include <cstdio>
#include <cstring>
#include <fstream>
#include <sstream>
#include "CSVToXMLConverter.h"

using ::testing::_;
using ::testing::Return;
using ::testing::SetArgPointee;
using ::testing::DoAll;

// Mock class for the functions
class MockConverter {
public:
    MOCK_METHOD(bool, GetFromString, (const char*, int*));
    MOCK_METHOD(bool, TryConvertToDouble, (const char*, double*));
};

MockConverter* g_mockConverter = nullptr;

// Redirect the calls to the mock methods
bool GetFromString(const char* str, int* out) {
    return g_mockConverter->GetFromString(str, out);
}

bool TryConvertToDouble(const char* str, double* out) {
    return g_mockConverter->TryConvertToDouble(str, out);
}

class CSVToXMLConverterTest : public ::testing::Test {
protected:
    virtual void SetUp() {
        g_mockConverter = &mockConverter;
    }

    virtual void TearDown() {
        g_mockConverter = nullptr;
    }

    void TestCSVInput(const char* input, const char* expectedOutput) {
        // Create a temporary file
        FILE* tempFile = tmpfile();
        fprintf(tempFile, "%s", input);
        rewind(tempFile);

        // Redirect stdout to a string
        testing::internal::CaptureStdout();
        CovertTradeRecordFromCSVToXML(tempFile);
        std::string output = testing::internal::GetCapturedStdout();

        // Close the temporary file
        fclose(tempFile);

        // Read the output XML file
        std::ifstream outFile("output.xml");
        std::stringstream buffer;
        buffer << outFile.rdbuf();
        outFile.close();
        
        // Check if the output matches the expected output
        ASSERT_EQ(output, "INFO: 2 trades processed\n");

        // Check if the output XML matches the expected XML
        ASSERT_EQ(buffer.str(), expectedOutput);
    }

    MockConverter mockConverter;
};

TEST_F(CSVToXMLConverterTest, ValidInput) {
    EXPECT_CALL(mockConverter, GetFromString(_, _))
        .WillRepeatedly(DoAll(SetArgPointee<1>(100), Return(true)));
    EXPECT_CALL(mockConverter, TryConvertToDouble(_, _))
        .WillRepeatedly(DoAll(SetArgPointee<1>(110.25), Return(true)));

    const char* input = "USDJPY,100,110.25\nEURUSD,200,1.2\n";
    const char* expectedOutput = 
        "<TradeRecords>\n"
        "\t<TradeRecord>\n"
        "\t\t<SourceCurrency>USD</SourceCurrency>\n"
        "\t\t<DestinationCurrency>JPY</DestinationCurrency>\n"
        "\t\t<Lots>33</Lots>\n"
        "\t\t<Price>110.250000</Price>\n"
        "\t</TradeRecord>\n"
        "\t<TradeRecord>\n"
        "\t\t<SourceCurrency>EUR</SourceCurrency>\n"
        "\t\t<DestinationCurrency>USD</DestinationCurrency>\n"
        "\t\t<Lots>66</Lots>\n"
        "\t\t<Price>1.200000</Price>\n"
        "\t</TradeRecord>\n"
        "</TradeRecords>\n";

    TestCSVInput(input, expectedOutput);
}

TEST_F(CSVToXMLConverterTest, MalformedLine) {
    const char* input = "USDJPY,100\n";
    const char* expectedOutput = 
        "WARN: Line 1 malformed. Only 2 field(s) found.\nINFO: 0 trades processed\n";

    TestCSVInput(input, expectedOutput);
}

TEST_F(CSVToXMLConverterTest, InvalidTradeAmount) {
    EXPECT_CALL(mockConverter, GetFromString(_, _))
        .WillRepeatedly(Return(false));

    const char* input = "USDJPY,abc,110.25\n";
    const char* expectedOutput = 
        "WARN: Trade amount on line 1 not a valid integer: 'abc'\nINFO: 0 trades processed\n";

    TestCSVInput(input, expectedOutput);
}

TEST_F(CSVToXMLConverterTest, InvalidTradePrice) {
    EXPECT_CALL(mockConverter, TryConvertToDouble(_, _))
        .WillRepeatedly(Return(false));

    const char* input = "USDJPY,100,abc\n";
    const char* expectedOutput = 
        "WARN: Trade price on line 1 not a valid decimal: 'abc'\nINFO: 0 trades processed\n";

    TestCSVInput(input, expectedOutput);
}

TEST_F(CSVToXMLConverterTest, MalformedTradeCurrencies) {
    const char* input = "USD,100,110.25\n";
    const char* expectedOutput = 
        "WARN: Trade currencies on line 1 malformed: 'USD'\nINFO: 0 trades processed\n";

    TestCSVInput(input, expectedOutput);
}

int main(int argc, char **argv) {
    ::testing::InitGoogleTest(&argc, argv);
    return RUN_ALL_TESTS();
}
