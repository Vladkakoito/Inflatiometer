#include <Controller/Impl/DoProcess.h>
#include <Common/Logger/Logger.h>
#include <Defines.h>

#include <sys/wait.h>
#include <unistd.h>


static int RunParser(int fd) {
  return 0;
}

static int RunDataHandler(int fd) {
  return 0;
}

int DoProcess() {
  int fds[2];
  
  if (pipe(fds) < 0) {
    LOG("Ошибка создания PIPE при запуске парсера");
    return -10;  
  }

  // вернёт pid дочернего процесса (парсера). 
  // ему отдаём один fd для ввода, себе оставляем другой 
  int parserPid = RunParser(fds[1]);
  if (parserPid < 0) {
    LOG("Ошибка запуска парсера: %d", parserPid);
    return parserPid;
  }

  close(fds[1]); // нам (родителю) он уже не нужен

  // запуск обработчика данных. желательно, что бы парсер их отдавал порционно.
  // что бы размазать нагрузку по времени
  int handlerPid = RunDataHandler(fds[0]);
  if (handlerPid < 0) {
    LOG("Ошибка запуска обработчика данных: %d", handlerPid);
    return handlerPid;
  }

  // ждём первого освободившегося. дай бог, парсер
  bool ok = true;
  for (int i = 0; i < 2; ++i) {
    int status;
    int pid = wait(&status);
    bool isParser = pid == handlerPid;

    if (WIFEXITED(status)) {
      LOG("%s вернул управление. Нормальное завершение. Собрано примерно %dт. товаров", 
          (isParser ? "Парсер" : "Обработчик"), status & 0xFF);
    } else if (WIFSTOPPED(status)) {
      ok = false;
      LOG("%s вернул управление. Нормальное завершение. Прерван сигналом %d", 
            (isParser ? "Парсер" : "Обработчик"), WSTOPSIG(status));
    } else if (WIFSIGNALED(status)) {
      ok = false;
      LOG("%s вернул управление. Аварийное завершение. Прерван сигналом %d", 
            (isParser ? "Парсер" : "Обработчик"), WSTOPSIG(status));
    }
  }
  if (!ok)
    return -11;

  return 0;
}