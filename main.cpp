//Important to notice that the library VL53L0X is causing the initial sentence error "Invalid new address"
//Despite my best efforts, I could not figure out where or what should be changed in order to get rid of this small error.
//From what I understood, the sensor is getting the same address as the one set as default as per code bellow:
/*
if (new_addr != VL53L0X_DEFAULT_ADDRESS) {
            status = set_device_address(new_addr);
            if (status) {
                printf("Failed to change I2C address!\n\r");
                return status;
            }
        } else {
            printf("Invalid new address!\n\r");
            return VL53L0X_ERROR_INVALID_PARAMS;
*/

/* Includes */
#include "mbed.h"
#include "HTS221Sensor.h"
#include "LPS22HBSensor.h"
#include "LSM6DSLSensor.h"
#include "lis3mdl_class.h"
#include "VL53L0X.h"

// objects for various sensors
static DevI2C devI2c(PB_11,PB_10);
static LPS22HBSensor press_temp(&devI2c);
static HTS221Sensor hum_temp(&devI2c);
static LSM6DSLSensor acc_gyro(&devI2c,0xD4,D4,D5); // high address
static LIS3MDL magnetometer(&devI2c, 0x3C);
static DigitalOut shutdown_pin(PC_6);
static VL53L0X range(&devI2c, &shutdown_pin, PC_7, 0x52);
static UnbufferedSerial pc(USBTX, USBRX);

char inp_char=0;


// functions to print sensor data
void print_t_rh(){
    float value1, value2;
    hum_temp.get_temperature(&value1);
    hum_temp.get_humidity(&value2);

    value1=value2=0;    
    press_temp.get_temperature(&value1);
    press_temp.get_pressure(&value2);
    printf("LPS22HB: [temp] %.2f C\r\n", value1);
}

void print_p_rh(){
    float value1, value2;
    hum_temp.get_temperature(&value1);
    hum_temp.get_humidity(&value2);

    value1=value2=0;    
    press_temp.get_temperature(&value1);
    press_temp.get_pressure(&value2);
    printf("LPS22HB: [press] %.2f mbar\r\n", value2);
}

void print_mag(){
    int32_t axes[3];
    magnetometer.get_m_axes(axes);
    printf("LIS3MDL [mag/mgauss]:    %6ld, %6ld, %6ld\r\n", axes[0], axes[1], axes[2]);

}

void print_accel(){
    int32_t axes[3];
    acc_gyro.get_x_axes(axes);
    printf("LSM6DSL [acc/mg]:        %6ld, %6ld, %6ld\r\n", axes[0], axes[1], axes[2]);
}

void print_gyro(){
    int32_t axes[3];
    acc_gyro.get_g_axes(axes);
    printf("LSM6DSL [gyro/mdps]:     %6ld, %6ld, %6ld\r\n", axes[0], axes[1], axes[2]);
}

void print_distance(){
    uint32_t distance;
    int status = range.get_distance(&distance);
    if (status == VL53L0X_ERROR_NONE) {
        printf("VL53L0X [mm]:            %6ld\r\n", distance);
    } else {
        printf("VL53L0X [mm]:                --\r\n");
    }
}


void pc_interrupt(){
    pc.read(&inp_char, 1);
}

/* Simple main function */
int main() {
    uint8_t id;
    float value1, value2;

    int32_t axes[3];

    hum_temp.init(NULL);

    press_temp.init(NULL);
    magnetometer.init(NULL);
    acc_gyro.init(NULL);

    range.init_sensor(0x52);

    hum_temp.enable();
    press_temp.enable();

    acc_gyro.enable_x();
    acc_gyro.enable_g();
  
    //printf("\033[2J\033[20A");
    printf ("\r\n--- Starting new run ---\r\n\r\n");

    hum_temp.read_id(&id);
    printf("HTS221  humidity & temperature    = 0x%X\r\n", id);

    press_temp.read_id(&id);
    printf("LPS22HB pressure & temperature    = 0x%X\r\n", id);
    magnetometer.read_id(&id);
    printf("LIS3MDL magnetometer              = 0x%X\r\n", id);
    acc_gyro.read_id(&id);
    printf("LSM6DSL accelerometer & gyroscope = 0x%X\r\n", id);
    
    printf("\n\r--- Reading sensor values ---\n\r"); ;
    print_t_rh();
    print_p_rh();
    print_mag();
    print_accel();
    print_gyro();
    print_distance();
    printf("\r\n");

    printf("Press\n -'t' for Temperature readings\n -'p' for Pressure readings\n -'m' for Magnetometer readings\n -'a' for Accelerometer readings\n -'g' for Gyroscope readings\n -'d' for Distance readings.\r\n\n");
    
    pc.attach(&pc_interrupt);

    while(1) {
        switch(inp_char){
            case 't':
                print_t_rh();
                inp_char = 0;
                printf("\r\n");
                printf("Press\n -'t' for Temperature readings\n -'p' for Pressure readings\n -'m' for Magnetometer readings\n -'a' for Accelerometer readings\n -'g' for Gyroscope readings\n -'d' for Distance readings.\r\n\n");
                break;

            case 'p':
                print_p_rh();
                inp_char = 0;
                printf("\r\n");
                printf("Press\n -'t' for Temperature readings\n -'p' for Pressure readings\n -'m' for Magnetometer readings\n -'a' for Accelerometer readings\n -'g' for Gyroscope readings\n -'d' for Distance readings.\r\n\n");
                break;

            case 'm':
                print_mag();
                inp_char = 0;
                printf("\r\n");
                printf("Press\n -'t' for Temperature readings\n -'p' for Pressure readings\n -'m' for Magnetometer readings\n -'a' for Accelerometer readings\n -'g' for Gyroscope readings\n -'d' for Distance readings.\r\n\n");
                break;

            case 'a':
                print_accel();
                inp_char = 0;
                printf("\r\n");
                printf("Press\n -'t' for Temperature readings\n -'p' for Pressure readings\n -'m' for Magnetometer readings\n -'a' for Accelerometer readings\n -'g' for Gyroscope readings\n -'d' for Distance readings.\r\n\n");
                break;

            case 'g':
                print_gyro();
                inp_char = 0;
                printf("\r\n");
                printf("Press\n -'t' for Temperature readings\n -'p' for Pressure readings\n -'m' for Magnetometer readings\n -'a' for Accelerometer readings\n -'g' for Gyroscope readings\n -'d' for Distance readings.\r\n\n");
                break;

            case 'd':
                print_distance();
                inp_char = 0;
                printf("\r\n");
                printf("Press\n -'t' for Temperature readings\n -'p' for Pressure readings\n -'m' for Magnetometer readings\n -'a' for Accelerometer readings\n -'g' for Gyroscope readings\n -'d' for Distance readings.\r\n\n");
                break;
            default:
                if (inp_char != 0) {
                    printf("Invalid input.\r\n");
                    printf("Press\n -'t' for Temperature readings\n -'p' for Pressure readings\n -'m' for Magnetometer readings\n -'a' for Accelerometer readings\n -'g' for Gyroscope readings\n -'d' for Distance readings.\r\n\n");
                    inp_char = 0;
                }
                break;
        }
        wait_us(500000);
    }
}