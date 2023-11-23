#pragma once

#include "esphome/core/component.h"
#include "esphome/components/binary_sensor/binary_sensor.h"
#include "esphome/components/i2c/i2c.h"

namespace esphome {
namespace distance_binary {

static const char *const TAG = "Binary Distance (HC-SR04 over I2C))";

class DistanceBinaryComponent : public binary_sensor::BinarySensor, public i2c::I2CDevice, public PollingComponent {
 public:
 
  void set_threshold(float threshold) { threshold_ = ceilf(threshold); }
  void set_above_threshold(bool above_threshold) { above_threshold = above_threshold_; }
  float get_setup_priority() const override { return setup_priority::DATA; }

  void dump_config() override;

  void update() override;

  void setup() override;
  
 protected:
  float threshold_;
  bool above_threshold_;
};

}  // namespace distance_binary
}  // namespace esphome
