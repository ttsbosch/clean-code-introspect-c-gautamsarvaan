#include <gtest/gtest.h>
#include <gmock/gmock.h>
#include <cstring>
#include "CSVToXMLConverter.h"

// Mocking the functions GetFromString and TryConvertToDouble
bool GetFromString(const char* str, int* out) {
    try {
        *out = std::stoi(str);
        return true;
    } catch (...) {
        return false;
    }
}

bool TryConvertToDouble(const char* str, double* out) {
    try {
        *out = std::stod(str);
        return true;
    } catch (...) {
        return false;
    }
}

class CSVToXMLConverterTest : public ::testing::Test {
protected:
    virtual void SetUp() {
        // Set up initializations here if needed
    }

    virtual void TearDown() {
        // Clean up here if needed
    }

    void TestCSVInput(const char* input, const char* expectedOutput) {
        // Prepare the input stream
        std::istringstream inputStream(input);

        // Redirect stdout to a string
        testing::internal::CaptureStdout();
        CovertTradeRecordFromCSVToXML(inputStream);
        std::string output = testing::internal::GetCapturedStdout();

        // Check if the output matches the expected output
        ASSERT_EQ(output, expectedOutput);
    }
};

TEST_F(CSVToXMLConverterTest, ValidInput) {
    const char* input = "USDJPY,100,110.25\nEURUSD,200,1.2\n";
    const char* expectedOutput = 
        "INFO: 2 trades processed\n";

    TestCSVInput(input, expectedOutput);
}

TEST_F(CSVToXMLConverterTest, MalformedLine) {
    const char* input = "USDJPY,100\n";
    const char* expectedOutput = 
        "WARN: Line 1 malformed. Only 2 field(s) found.\nINFO: 0 trades processed\n";

    TestCSVInput(input, expectedOutput);
}

TEST_F(CSVToXMLConverterTest, InvalidTradeAmount) {
    const char* input = "USDJPY,abc,110.25\n";
    const char* expectedOutput = 
        "WARN: Trade amount on line 1 not a valid integer: 'abc'\nINFO: 0 trades processed\n";

    TestCSVInput(input, expectedOutput);
}

TEST_F(CSVToXMLConverterTest, InvalidTradePrice) {
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
