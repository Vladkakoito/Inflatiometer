
import requests
import json
import sys
import re
import time
from datetime import datetime

with open(sys.argv[1], "r", encoding="utf-8") as f:
    rawCurl = f.read()
curl = json.loads(rawCurl)
headers = curl['headers']
step = 50
payload = {
  "sort" : {
    "order" : "desc",
    "type" : "price"
  },
  "pagination" : {
    "limit" : step,
    "offset" : 0
  },
  "includeAdultGoods" : True,
  "categories" : [0],
  "catalogType" : "1",
  "storeType" : "6",
  "storeCode" : "991004" #Ptz, Kazarmenskaya
}

def PrintResponse(response):
  print('response status code:', response.status_code, '\n')
  print('response headers:')
  for key, value in response.headers.items():
    print(f"{key}: {value}")
  try:
    print("\nresponse content:\n", json.dumps(response.json(), ensure_ascii=False, indent=4), '\n')
    print ('count:\n', len(response.json()['items']))
  except:
    try:
      print("\nresponse content:\n", response.content, '\n')
    except:
      print("\nresponse content:\n", response.text, '\n')


listOfProducts = {}
errors = {}

def GetProductsInPage(offset):
  url = 'https://magnit.ru/webgate/v2/goods/search'
  payload["pagination"]["offset"] = offset
  response = requests.post(url = url, headers = headers, json = payload)

  if response.status_code != 200:
    errors.setdefault(url, {})
    errors[url].setdefault({response.text : [0, {}]});
    errors[url][response.text][0] += 1
    errors[url][response.text][1]["category"] = payload["categories"][0]
    errors[url][response.text][1]["page"] = offset / step
    return 0
  
  cnt = len(response.json()["items"])

  for item in response.json()["items"]:
    listOfProducts[item["name"]] = item["price"]

  return cnt

def GetProductsInCatalog(id):
  payload["categories"][0] = int(id)
  offset = 0
  while GetProductsInPage(offset) != 0:
    time.sleep(0.1)
    offset += step
  
  
url = 'https://magnit.ru/catalog/'
response = requests.post(url = url, headers = headers)
if response.status_code != 200:
   PrintResponse(response)
   print('\n', response.status_code, '\n')
   exit(0)

pattern = r'"([0-9]+)",\s*"/catalog/\1[^,\n]*&shopType=6"'
matchs = re.findall(pattern, response.text)
print("categories found: ", len(matchs))
cnt = 0
for id in matchs:
   time.sleep(0.1)
   prev = len(listOfProducts)
   print("category ", id, " started.", datetime.now().time())
   GetProductsInCatalog(id)
   cnt += 1
   print("category ", id, " finished (", cnt, "/", len(matchs), "). ", 
        len(listOfProducts) - prev, " products added. (total ", len(listOfProducts), ")")


print("total products found: ", len(listOfProducts))
print(json.dumps(errors, ensure_ascii=False, indent=4))
