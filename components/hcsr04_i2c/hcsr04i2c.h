#pragma once

#include "esphome/core/component.h"
#include "esphome/components/sensor/sensor.h"
#include "esphome/components/i2c/i2c.h"

namespace esphome {
namespace hcsr04i2c {

static const char *const TAG = "HC-SR04 (I2C)";

class HCSR04I2CComponent : public i2c::I2CDevice, public PollingComponent, public sensor::Sensor {
 public:
  float get_setup_priority() const override { return setup_priority::DATA; }

  void dump_config() override;

  void update() override;

  void setup() override;
};

}  // namespace hcsr04i2c
}  // namespace esphome
