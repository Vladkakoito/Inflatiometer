#include <Controller/Impl/DoProcess.h>
#include <Common/Logger/Logger.h>
#include <Defines.h>

#include <errno.h>
#include <string.h>
#include <sys/stat.h>
#include <sys/wait.h>
#include <unistd.h>
#include <stdlib.h>

static const char* GetFileName (const char *path) {
  const char *filename = strrchr(path, '/');
  return filename ? filename + 1 : path;
}

static int RunParser(int fd, const char *command) {
  DBG(2, "Запускается парсер %s", command);

  LOG_FLUSH(); // иначе потом в созданный процесс буфер скопируется (я так понимаю)

  int pid = fork();
  if (pid < 0) {
    LOG("Ошибка вызова fork в RunParser: %s", strerror(errno));
    return pid;
  }

  // родительский процесс
  if (pid > 0) {
    close(fd);
    return pid;
  }

  if (execlp(command, GetFileName(command), (char*)0) >= 0)
    exit(0);

  if (errno == EACCES) {
    LOG("Ошибка запуска команды парсера (execlp): %s. " \
        "Попытка установить право на выполнение.", strerror(errno));

    struct stat statbuf;
    if (stat(command, &statbuf) < 0 || chmod(command, statbuf.st_mode | S_IXUSR) < 0){
      LOG("Не удалось изменить права файла %s. Ошибка: %s", command, strerror(errno));
      exit(-12);
    }
    LOG("Права на выполнение успешно установлены для файла %s. Повторная попытка запуска...", command);
    if (execlp(command, GetFileName(command), (char*)0) >=  0)
      exit(0); 
  }
  LOG("Ошибка запуска команды парсера (execlp): %s", strerror(errno));
  exit(-12);
}

static int RunDataHandler(int fd, const char *command) {

  return 0;
}

int DoProcess(struct TProcessesToRun *processes) {
  DBG(2, "Запуск процессов парсеров и обработчиков данных");
  int fds[2];
  
  if (pipe(fds) < 0) {
    LOG("Ошибка создания PIPE при запуске парсера");
    return -10;  
  }

  // вернёт pid дочернего процесса (парсера). 
  // ему отдаём один fd для ввода, себе оставляем другой 
  int parserPid = RunParser(fds[1], processes->parser);
  if (parserPid < 0) {
    LOG("Ошибка запуска парсера: %d", parserPid);
    return parserPid;
  }

  DBG(2, "Главный процесс вернулся в DoProcess после запуска парсеров");

  close(fds[1]); // нам (родителю) он уже не нужен

  // запуск обработчика данных. желательно, что бы парсер их отдавал порционно.
  // что бы размазать нагрузку по времени
  int handlerPid = RunDataHandler(fds[0], processes->dataHandler);
  if (handlerPid < 0) {
    LOG("Ошибка запуска обработчика данных: %d", handlerPid);
    return handlerPid;
  }

  DBG(2, "Главный процесс вернулся в DoProcess после запуска обработчиков данных");

  // ждём первого освободившегося. дай бог, парсер
  bool ok = true;
  for (int i = 0; i < 2; ++i) {
    DBG(5, "Старт ожидания первого из парсеров и обработчиков");
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