# Run on Python 2.7
# Just for upload fake data to MCS
import urllib2
import json

mcs_data_format = {
   "datapoints":[
      {
         "dataChnId":"Temp_Display",
         "values":{
            "value":"00.0"
         }
      },
      {
         "dataChnId":"Hum_Display",
         "values":{
            "value":"00.0"
         }
      },
      {
         "dataChnId":"Vib_Display",
         "values":{
            "value":"0"
         }
      },
      {
         "dataChnId":"Bat_Display",
         "values":{
            "value":"0"
         }
      }
   ]
}

string_value = "26968111"
print(float(string_value[0:3])/10)
print(float(string_value[3:6])/10)
print(string_value[6:7])
battery = string_value[7:8]
if battery == '0':
    battery = 0
elif battery == '1':
    battery = 33
elif battery == '2':
    battery = 66
elif battery == '3':
    battery = 100
print(battery)
print("")
mcs_data_format['datapoints'][0]['values']['value'] = float(string_value[0:3])/10
mcs_data_format['datapoints'][1]['values']['value'] = float(string_value[3:6])/10
mcs_data_format['datapoints'][2]['values']['value'] = string_value[6:7]
mcs_data_format['datapoints'][3]['values']['value'] = battery
print(mcs_data_format)
req = urllib2.Request('http://api.mediatek.com/mcs/v2/devices/DKV8iNT6/datapoints')
req.add_header('deviceKey', '0dmE68ETeSrq9Y8K')
req.add_header('Content-Type', 'application/json')

response = urllib2.urlopen(req, json.dumps(mcs_data_format))
