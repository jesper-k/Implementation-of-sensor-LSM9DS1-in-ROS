/*****************************************************************/
// The SFE_LSM9DS1 library requires both Wire and SPI be
// included BEFORE including the 9DS1 library.
#include <Wire.h>
#include <SPI.h>
#include <SparkFunLSM9DS1.h>
#include <ros.h>
#include <std_msgs/Float32MultiArray.h>

//Set up the ros node and publisher
std_msgs::Float32MultiArray imu_msg;
ros::Publisher pub_imu("IMUdata", &imu_msg);
ros::NodeHandle nh;


//////////////////////////
// LSM9DS1 Library Init //
//////////////////////////
// Use the LSM9DS1 class to create an object. [imu] can be
// named anything, we'll refer to that throught the sketch.
LSM9DS1 imu;

///////////////////////
// Example I2C Setup //
///////////////////////
// SDO_XM and SDO_G are both pulled high, so our addresses are:
#define LSM9DS1_M  0x1E // Would be 0x1C if SDO_M is LOW
#define LSM9DS1_AG  0x6B // Would be 0x6A if SDO_AG is LOW

////////////////////////////
// Sketch Output Settings //
////////////////////////////


// Earth's magnetic field varies by location. Add or subtract 
// a declination to get a more accurate heading. Calculate 
// your's here:
// http://www.ngdc.noaa.gov/geomag-web/#declination
//#define DECLINATION -8.58 // Declination (degrees) in Boulder, CO.

//Function definitions

void setup() 
{

   nh.initNode();

   imu_msg.data_length = 9;

   nh.advertise(pub_imu);
  
   Serial.begin(115200);
  
  // Before initializing the IMU, there are a few settings
  // we may need to adjust. Use the settings struct to set
  // the device's communication mode and addresses:
  imu.settings.device.commInterface = IMU_MODE_I2C;
  imu.settings.device.mAddress = LSM9DS1_M;
  imu.settings.device.agAddress = LSM9DS1_AG;
  // The above lines will only take effect AFTER calling
  // imu.begin(), which verifies communication with the IMU
  // and turns it on.
  if (!imu.begin())
  {
    Serial.println(F("Failed to communicate with LSM9DS1."));
    Serial.println(F("Double-check wiring."));
    Serial.println(F("Default settings in this sketch will " \
                  "work for an out of the box LSM9DS1 " \
                  "Breakout, but may need to be modified " \
                  "if the board jumpers are."));
    while (1)
      ;
  }
}

void loop()
{
  // Update the sensor values whenever new data is available
  if ( imu.gyroAvailable() )
  {
    // To read from the gyroscope,  first call the
    // readGyro() function. When it exits, it'll update the
    // gx, gy, and gz variables with the most current data.
    imu.readGyro();
  }
  if ( imu.accelAvailable() )
  {
    // To read from the accelerometer, first call the
    // readAccel() function. When it exits, it'll update the
    // ax, ay, and az variables with the most current data.
    imu.readAccel();
  }
  if ( imu.magAvailable() )
  {
    // To read from the magnetometer, first call the
    // readMag() function. When it exits, it'll update the
    // mx, my, and mz variables with the most current data.
    imu.readMag();
  }
  float data_arr [imu_msg.data_length];
  //imu_msg.data = imu.calcGyro(imu.gx);
  data_arr[0] = imu.calcGyro(imu.gx);
  data_arr[1] = imu.calcGyro(imu.gy);
  data_arr[2] = imu.calcGyro(imu.gz);
  data_arr[3] = imu.calcAccel(imu.ax);
  data_arr[4] = imu.calcAccel(imu.ay);
  data_arr[5] = imu.calcAccel(imu.az);
  data_arr[6] = imu.calcMag(imu.mx);
  data_arr[7] = imu.calcMag(imu.my);
  data_arr[8] = imu.calcMag(imu.mz);
  imu_msg.data = data_arr;
  pub_imu.publish(&imu_msg);
  nh.spinOnce();
  delay(1000);
}
