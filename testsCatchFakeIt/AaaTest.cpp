#include <string>

#include <catch.hpp>


class Accumulator
{
public:
    Accumulator(int total) : m_total(total)
    {}

    void accumulate(int value)
    {
        m_total += value;
    }

    int total() const
    {
        return m_total;
    }

private:
    int m_total = 0;
};

TEST_CASE("Accumulator initialized with a non-zero value adds a new value to total")
{
    // Arrange
    Accumulator accumulator(100);

    // Act
    accumulator.accumulate(42);

    // Assert
    REQUIRE(accumulator.total() == 142);
}
