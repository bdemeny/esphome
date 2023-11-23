#include "distance_binary.h"

#include "esphome/core/log.h"
#include "esphome/core/hal.h"

namespace esphome {
namespace distance_binary {

static const uint8_t HCSR04_COMMAND_START = 0b00000001;

void DistanceBinaryComponent::dump_config() {
  LOG_BINARY_SENSOR("  ", "Distance binary sensor (based on HC-SR04 over I2C):", this);
  LOG_I2C_DEVICE(this);
  if (this->is_failed()) {
    ESP_LOGE(TAG, "Communication with HC-SR04 over I2C failed!");
  }
  LOG_UPDATE_INTERVAL(this);
}

void DistanceBinaryComponent::setup() {
  ESP_LOGCONFIG(TAG, "Distance binary - HC-SR04 Sensor (I2C)'%s'...", this->name_.c_str());
  ESP_LOGI(TAG, "Sensor initialized.");
}

void DistanceBinaryComponent::update() {
  uint8_t start_cmd = HCSR04_COMMAND_START;
  if(this->write(&start_cmd,1) != i2c::ERROR_OK){          //1 = cmd to start meansurement
    ESP_LOGE(TAG, "Error writing start command to HC-SR04 over I2C bus");
    this->publish_state(false);
    this->mark_failed();
  }  
  else{
    delay(150);             //1 cycle approx. 100mS.
    uint8_t ds[3] = {0,0,0};
    unsigned long distance; 
    if (this->read(ds, 3) != i2c::ERROR_OK) {
      ESP_LOGE(TAG, "Error reading data from HC-SR04 over I2C bus");
      this->publish_state(false);
      this->status_set_error();
    } else {
      distance = (unsigned long)(ds[0]) * 65536;
      distance = distance + (unsigned long)(ds[1]) * 256;
      distance = (distance + (unsigned long)(ds[2])) / 10000;
      if ((2<=distance)&&(400>=distance)){    //measured value between 1cm to 4meters
        this->status_clear_error();
        this->status_clear_warning();
        bool state = false;
        if(above_threshold_){
          state = (distance >= threshold_);
        }
        else{
          state = (distance <= threshold_);
        }
        this->publish_state(state);
      }else{
        this->publish_state(false); //Distance beyond limits
        this->status_set_warning();
      }
    }
    delay(120);
  }
}

}  // namespace distance_binary
}  // namespace esphome
