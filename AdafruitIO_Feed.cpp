//
// Copyright (c) 2015-2016 Adafruit Industries
// Authors: Tony DiCola, Todd Treece
// Licensed under the MIT license.
//
// All text above must be included in any redistribution.
//
#include "AdafruitIO_Feed.h"
#include "AdafruitIO.h"

AdafruitIO_Feed::AdafruitIO_Feed(AdafruitIO *io, const char *n)
{
  _io = io;
  name = n;
  _sub = 0;
  _pub = 0;

  _init();
}

AdafruitIO_Feed::AdafruitIO_Feed(AdafruitIO *io, const __FlashStringHelper *n)
{
  _io = io;
  name = (const char*)n;
  _sub = 0;
  _pub = 0;

  _init();
}

AdafruitIO_Feed::~AdafruitIO_Feed()
{
  if(_sub)
    delete _sub;

  if(_pub)
    delete _pub;

  if(_data)
    delete _data;

  if(_topic)
    free(_topic);
}

void AdafruitIO_Feed::onMessage(AdafruitIODataCallbackType cb)
{
  _dataCallback = cb;
  _io->_mqtt->subscribe(_sub);

  _sub->setCallback(this, &AdafruitIO_Feed::subCallback);
}

bool AdafruitIO_Feed::save(char *value, double lat, double lon, double ele)
{
  _data->setValue(value, lat, lon, ele);
  return _pub->publish(_data->toCSV());
}

bool AdafruitIO_Feed::save(bool value, double lat, double lon, double ele)
{
  _data->setValue(value, lat, lon, ele);
  return _pub->publish(_data->toCSV());
}

bool AdafruitIO_Feed::save(String value, double lat, double lon, double ele)
{
  _data->setValue(value, lat, lon, ele);
  return _pub->publish(_data->toCSV());
}

bool AdafruitIO_Feed::save(int value, double lat, double lon, double ele)
{
  _data->setValue(value, lat, lon, ele);
  return _pub->publish(_data->toCSV());
}

bool AdafruitIO_Feed::save(unsigned int value, double lat, double lon, double ele)
{
  _data->setValue(value, lat, lon, ele);
  return _pub->publish(_data->toCSV());
}

bool AdafruitIO_Feed::save(long value, double lat, double lon, double ele)
{
  _data->setValue(value, lat, lon, ele);
  return _pub->publish(_data->toCSV());
}

bool AdafruitIO_Feed::save(unsigned long value, double lat, double lon, double ele)
{
  _data->setValue(value, lat, lon, ele);
  return _pub->publish(_data->toCSV());
}

bool AdafruitIO_Feed::save(float value, double lat, double lon, double ele, int precision)
{
  _data->setValue(value, lat, lon, ele, precision);
  return _pub->publish(_data->toCSV());
}

bool AdafruitIO_Feed::save(double value, double lat, double lon, double ele, int precision)
{
  _data->setValue(value, lat, lon, ele, precision);
  return _pub->publish(_data->toCSV());
}

void AdafruitIO_Feed::setLocation(double lat, double lon, double ele)
{
  _data->setLocation(lat, lon, ele);
}

void AdafruitIO_Feed::subCallback(char *val, uint16_t len)
{
  _data->setCSV(val);
  _dataCallback(_data);
}

void AdafruitIO_Feed::_init()
{

  // dynamically allocate memory for topic
  _topic = (char *) malloc(sizeof(char) * (strlen(_io->_username) + strlen(name) + 8)); // 8 extra chars for /f/, /csv & null termination

  // init feed data
  _data = new AdafruitIO_Data(this);

  if(_topic) {

    // build topic string
    strcpy(_topic, _io->_username);
    strcat(_topic, "/f/");
    strcat(_topic, name);
    strcat(_topic, "/csv");

    // setup subscription
    _sub = new Adafruit_MQTT_Subscribe(_io->_mqtt, _topic);
    _pub = new Adafruit_MQTT_Publish(_io->_mqtt, _topic);

  } else {

    // malloc failed
    _topic = 0;
    _sub = 0;
    _pub = 0;
    _data = 0;

  }

}
