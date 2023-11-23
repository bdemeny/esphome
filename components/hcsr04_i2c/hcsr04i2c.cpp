#include "hcsr04i2c.h"

#include "esphome/core/log.h"
#include "esphome/core/hal.h"

namespace esphome {
namespace hcsr04i2c {

static const uint8_t HCSR04_COMMAND_START = 0b00000001;

void HCSR04I2CComponent::dump_config() {
  LOG_SENSOR("  ", "Ultrasonic sensor:", this);
  LOG_I2C_DEVICE(this);
  if (this->is_failed()) {
    ESP_LOGE(TAG, "Communication with HC-SR04 over I2C failed!");
  }
  LOG_UPDATE_INTERVAL(this);
}

void HCSR04I2CComponent::setup() {
  ESP_LOGCONFIG(TAG, "HC-SR04 Sensor (I2C)'%s'...", this->name_.c_str());
  ESP_LOGI(TAG, "Sensor initialized.");
}

void HCSR04I2CComponent::update() {
  uint8_t start_cmd = HCSR04_COMMAND_START;
  if(this->write(&start_cmd,1) != i2c::ERROR_OK){          //1 = cmd to start meansurement
    ESP_LOGE(TAG, "Error writing start command to HC-SR04 over I2C bus");
    this->publish_state(NAN);
    this->mark_failed();
  }  
  else{
    delay(150);             //1 cycle approx. 100mS.
    uint8_t ds[3] = {0,0,0};
    unsigned long distance; 
    if (this->read(ds, 3) != i2c::ERROR_OK) {
      ESP_LOGE(TAG, "Error reading data from HC-SR04 over I2C bus");
      this->publish_state(NAN);
    } else {
      distance = (unsigned long)(ds[0]) * 65536;
      distance = distance + (unsigned long)(ds[1]) * 256;
      distance = (distance + (unsigned long)(ds[2])) / 10000;
      //distance=(unsigned long)((ds[0]*65536+ds[1]*256+ds[2])/10000);
      if ((2<=distance)&&(400>=distance)){    //measured value between 1cm to 4meters
        this->publish_state(distance);
      }else{
        this->publish_state(0); //Distance beyond limits
      }
    }
    delay(120);
  }
}

}  // namespace hcsr04_i2c
}  // namespace esphome
