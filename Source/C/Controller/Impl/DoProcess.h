#pragma once

struct TProcessesToRun {
  const char *parser;
  const char *dataHandler;
};

int DoProcess(struct TProcessesToRun *);
