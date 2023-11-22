
#include "zio_ultrasonic.h"

#include "esphome/core/log.h"

namespace esphome {
namespace hcsr04_i2c {

static const uint8_t HCSR04_COMMAND_START = 0b00000001;

void HCSR04_I2CComponent::dump_config() {
  LOG_SENSOR("  ", "Ultrasonic sensor:", this);
  LOG_I2C_DEVICE(this);
  if (this->is_failed()) {
    ESP_LOGE(TAG, "Communication with HC-SR04 over I2C failed!");
  }
  LOG_UPDATE_INTERVAL(this);
}

void HCSR04_I2CComponent::setup() {
  ESP_LOGCONFIG(TAG, "HC-SR04 Sensor (I2C)'%s'...", this->name_.c_str());
  uint8_t start_cmd = HCSR04_COMMAND_START;
  if (this->write(&start_cmd,1) != i2c::ERROR_OK) {
    this->mark_failed();
    return;
  }
}

void HCSR04_I2CComponent::update() {
  uint8_t start_cmd = HCSR04_COMMAND_START;
  if(this->write(&start_cmd,1) != i2c::ERROR_OK){          //1 = cmd to start meansurement
    ESP_LOGE(TAG, "Error writing start command to HC-SR04 over I2C bus");
    this->publish_state(NAN);
  }  
  else{
    delay(120);             //1 cycle approx. 100mS.
    uint16_t distance;
    uint8_t ds[3] = {0,0,0};
    unsigned long distance; 
    if (this->read(ds, &distance) != i2c::ERROR_OK) {
      ESP_LOGE(TAG, "Error reading data from HC-SR04 over I2C bus");
      this->status_set_error();
      this->publish_state(NAN);
    } else {
      this->status_clear_error();
      distance = (unsigned long)(ds[0]) * 65536;
      distance = distance + (unsigned long)(ds[1]) * 256;
      distance = (distance + (unsigned long)(ds[2])) / 10000;
      //distance=(unsigned long)((ds[0]*65536+ds[1]*256+ds[2])/10000);
      if ((1<=distance)&&(900>=distance)){    //measured value between 1cm to 9meters
        this->publish_state(distance);
        this->status_clear_warning();
      }else{
        this->publish_state(0); //Distance beyond limits
        this->status_set_warning();
      }
    }
  }
}

}  // namespace hcsr04_i2c
}  // namespace esphome
