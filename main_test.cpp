#include <gtest/gtest.h>
#include "driver.hpp"

int main(int argc, char **argv) {
  ::testing::InitGoogleTest(&argc, argv);
  return RUN_ALL_TESTS();
}

class CalculatorTest : public ::testing::Test
{
    protected:
        Calculator::Driver m_driver;
};

TEST_F(CalculatorTest, simple)
{
    m_driver.parse("1+1");
    ASSERT_EQ(m_driver.getResult(), 2);
	m_driver.parse("2+2");
    ASSERT_EQ(m_driver.getResult(), 4);
}

TEST_F(CalculatorTest, precedence)
{
    m_driver.parse("1+2*3");
    ASSERT_EQ(m_driver.getResult(), 7);
}

TEST_F(CalculatorTest, simpleBrackets)
{
	m_driver.parse("(1+2)*3");
    ASSERT_EQ(m_driver.getResult(), 9);
}

TEST_F(CalculatorTest, power)
{
    m_driver.parse("2^2^3");
    ASSERT_EQ(m_driver.getResult(), 256);
	m_driver.parse("(2^2)^3");
    ASSERT_EQ(m_driver.getResult(), 64);
}

TEST_F(CalculatorTest, minus)
{
	m_driver.parse("2+-1");
    ASSERT_EQ(m_driver.getResult(), 1);
}


TEST_F(CalculatorTest, powerAndMultiplyOrdering)
{
    m_driver.parse("2*3^2");
    ASSERT_EQ(m_driver.getResult(), 18);
}

TEST_F(CalculatorTest, syntaxError)
{
    m_driver.parse("2^^");
	ASSERT_TRUE(m_driver.isError());
	ASSERT_EQ(m_driver.getError(), "syntax error");
}
