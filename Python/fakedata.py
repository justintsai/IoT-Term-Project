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
         "dataChnId":"Vib2_Display",
         "values":{
            "value":"0"
         }
      }
   ]
}

string_value = "2757010" #json_extractor[0]['data'].decode("hex")
print(float(string_value[0:3])/10)
print(float(string_value[3:6])/10)
print(string_value[6:7])
print("")
mcs_data_format['datapoints'][0]['values']['value'] = float(string_value[0:3])/10
mcs_data_format['datapoints'][1]['values']['value'] = float(string_value[3:6])/10
mcs_data_format['datapoints'][2]['values']['value'] = string_value[6:7]
print(mcs_data_format)
req = urllib2.Request('http://api.mediatek.com/mcs/v2/devices/DIK4dY0L/datapoints')
req.add_header('deviceKey', 'oOQPFC7Of5Hn0b89')
req.add_header('Content-Type', 'application/json')

response = urllib2.urlopen(req, json.dumps(mcs_data_format))
