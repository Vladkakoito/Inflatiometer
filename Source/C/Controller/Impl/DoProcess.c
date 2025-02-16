#include <Defines.h>

#include <errno.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

#include <Common/Functions/Functions.h>
#include <Common/Logger/Logger.h>
#include <sys/stat.h>
#include <sys/wait.h>

#include <Controller/Impl/DoProcess.h>

static int RunParser(int fd, const char *command) {
  DBG(2, "Запускается парсер %s", command);

  LOG_FLUSH(); // иначе потом в созданный процесс буфер скопируется (я так понимаю)

  int pid = fork();
  if (pid < 0)
    RETURN_LOG(-12, "Ошибка вызова fork в RunParser: %s", strerror(errno));

  // родительский процесс
  if (pid > 0) {
    close(fd);
    return pid;
  }

  if (dup2(fd, STDOUT_FILENO) != STDOUT_FILENO)
    EXIT_LOG(-17, "Не удалось назначить стандартный вывод для канала для парсера");

  if (execlp(command, GetFileName(command), nullptr) >= 0)
    exit(0);

  if (errno == EACCES) {
    LOG("Ошибка запуска команды парсера (execlp): %s. "
        "Попытка установить право на выполнение.",
        strerror(errno));

    struct stat statbuf;
    if (stat(command, &statbuf) < 0 || chmod(command, statbuf.st_mode | S_IXUSR) < 0)
      EXIT_LOG(-12, "Не удалось изменить права файла %s. Ошибка: %s", command, strerror(errno));

    LOG("Права на выполнение успешно установлены для файла %s. Повторная попытка запуска.",
        command);
    if (execlp(command, GetFileName(command), nullptr) >= 0)
      exit(0);
  }

  EXIT_LOG(-12, "Ошибка запуска команды парсера (execlp): %s", strerror(errno));
}

static int RunDataHandler(int fd, const char *command) {
  return 0;
}

int DoProcess(struct TProcessesToRun *processes) {
  DBG(2, "Запуск процессов парсеров и обработчиков данных");

  int fds[2];
  if (pipe(fds) < 0)
    RETURN_LOG(-10, "Ошибка создания PIPE при запуске парсера");
  DBG(9, "PIPE для парсера и обработчика создан");

  // вернёт pid дочернего процесса (парсера).
  // ему отдаём один fd для ввода, себе оставляем другой
  int parserPid = RunParser(fds[1], processes->parser);
  if (parserPid < 0)
    RETURN_LOG(parserPid, "Ошибка запуска парсера: %d", parserPid);

  DBG(2, "Главный процесс вернулся в DoProcess после запуска парсеров");

  close(fds[1]); // нам (родителю) он уже не нужен

  // запуск обработчика данных. желательно, что бы парсер их отдавал порционно.
  // что бы размазать нагрузку по времени
  int handlerPid = RunDataHandler(fds[0], processes->dataHandler);
  if (handlerPid < 0)
    RETURN_LOG(handlerPid, "Ошибка запуска обработчика данных: %d", handlerPid);

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
