#!/usr/bin/python3

import sys
import json
import os
import stat

dbPath = "~/Inflatiometer/Database"

def CreateSubDirs(arrSubDirs):
  for dir in arrSubDirs:
    if len(dir["childs"]) == 0:
      
    if not os.path.isdir(dir["i"]):
      os.mkdir(dir["i"])
    

def CreateDbStructure():
  with open("Data.json", "r") as dataFile:
    data = json.load(dataFile)

  if not os.path.isdir(dbPath):
    os.mkdir(dbPath)

  os.chdir(dbPath)
   


if sys.argc < 2:
  exit(1)

if sys.argv[1] == "create":
  CreateDbStructure() 