import esphome.codegen as cg
import esphome.config_validation as cv
from esphome.components import binary_sensor, i2c
from esphome.const import (
    CONF_THRESHOLD,
    CONF_ABOVE,
	DEVICE_CLASS_MOTION,
)

DEPENDENCIES = ["i2c"]
CODEOWNERS = ["@bdemeny"]

distance_binary_ns = cg.esphome_ns.namespace("distance_binary")

DistanceBinaryComponent = distance_binary_ns.class_(
    "DistanceBinaryComponent", binary_sensor.BinarySensor, cg.PollingComponent, i2c.I2CDevice
)

CONFIG_SCHEMA = (
    binary_sensor.binary_sensor_schema(
        DistanceBinaryComponent, device_class=""
    )
    .extend(
        {
            cv.Optional(CONF_THRESHOLD, default=20): cv.float_with_unit(
                "threshold", "(cm)", True
            ),
            cv.Optional(CONF_ABOVE, default=False): cv.boolean,
        }
    )
    .extend(cv.polling_component_schema("1s"))
    .extend(i2c.i2c_device_schema(0x57))
)


async def to_code(config):
    var = await binary_sensor.new_binary_sensor(config)
    await cg.register_component(var, config)
    await i2c.register_i2c_device(var, config)
    cg.add(var.set_threshold(config[CONF_THRESHOLD]))
    cg.add(var.set_above_threshold(config[CONF_ABOVE]))
