#include <gtest/gtest.h>
#include "dht22.h"

TEST(DewpointTest, HandlesInputs) {
    EXPECT_NEAR(10.5, dewPoint(30, 30), 0.1);
    EXPECT_NEAR(29.1, dewPoint(30, 95), 0.1);
    EXPECT_NEAR(-6, dewPoint(10,30), 0.8);
    EXPECT_NEAR(9.2, dewPoint(10, 95), 0.1);
}
// TEST_F(...)

#if defined(ARDUINO)
#include <Arduino.h>

void setup()
{
    // should be the same value as for the `test_speed` option in "platformio.ini"
    // default value is test_speed=115200
    Serial.begin(115200);

    ::testing::InitGoogleTest();
    // if you plan to use GMock, replace the line above with
    // ::testing::InitGoogleMock();
}

void loop()
{
  // Run tests
  if (RUN_ALL_TESTS())
  ;

  // sleep for 1 sec
  delay(1000);
}

#else
int main(int argc, char **argv)
{
    ::testing::InitGoogleTest(&argc, argv);
    // if you plan to use GMock, replace the line above with
    // ::testing::InitGoogleMock(&argc, argv);

    if (RUN_ALL_TESTS())
    ;

    // Always return zero-code and allow PlatformIO to parse results
    return 0;
}
#endif