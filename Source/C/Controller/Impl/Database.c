#include <Defines.h>

#include <errno.h>
#include <signal.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

#include <Common/Logger/Logger.h>
#include <Common/Settings.h>
#include <sys/wait.h>

#include <Controller/Impl/Database.h>

#include "Constants.h"

static pid_t g_dbManagerPid = -1;

// Дескриптор для команд. работает в обе стороны - с него же и получаем ответ.
// Это только для Linux вроде работает. Но я на другие платформы и не рассчитываю
static int g_fd = -1;

static const char *kCheckCommand = "check";

// запуск менеджера БД. он будет принимать команды
int RunDBManager(const struct TSettingsDatabase *settings) {
  DBG(2, "Запускается менеджер БД");

  int fds[2];
  if (pipe(fds) < 0)
    RETURN_LOG(-10, "Ошибка создания PIPEs при запуске менеджера БД");
  DBG(9, "PIPEs для команд менеджеру БД создан");
  g_fd = fds[1];

  LOG_FLUSH(); // что бы буфер не попал и в родителя и в потомка

  int pid = fork();
  if (pid < 0)
    RETURN_LOG(-14, "Ошибка вызова fork в RunDBManager: %s", strerror(errno));

  // родительский процесс
  if (pid > 0) {
    close(fds[0]);
    return 0;
  }

  close(fds[1]);

  if (dup2(fds[0], STDIN_FILENO) != STDIN_FILENO)
    EXIT_LOG(-17, "Не удалось назначить стандартный ввод для канала для менеджера БД");
  if (dup2(fds[0], STDOUT_FILENO) != STDOUT_FILENO)
    EXIT_LOG(-17, "Не удалось назначить стандартный вывод для канала для менеджера БД");

  if (execlp(settings->manager, settings->manager, nullptr) >= 0)
    exit(0);

  EXIT_LOG(-20, "Ошибка запуска менеджера БД (execlp): %s", strerror(errno));
}

// можно было бы сделать команду "стоп" для менеджера. я просто решил попрактиковаться с сигналами.
// Да и так получится более общий способ остановки дочерних процессов внутри проекта
void StopDbManager() {
  close(g_fd);

  DBG(1, "Остановка менеджера БД");
  if (g_dbManagerPid == -1) {
    DBG(3, "Менеджер БД не был инициализирован.");
    return;
  }

  DBG(5, "Отправка сигнала SIGTERM менеджеру БД");
  if (kill(g_dbManagerPid, SIGTERM) == 0) {
    DBG(7, "Сигнал отправлен, ждём ответа");
    int status;
    if (waitpid(g_dbManagerPid, &status, 0) != g_dbManagerPid)
      DBG(1, "Не удалось дождаться дождаться процесса менеджера БД");
    else if (WIFSTOPPED(status))
      DBG(1, WSTOPSIG(status) == getpid() ? "Менеджер остановлен без сюрпризов"
                                          : "Менеджер сообщил, что его остановили не мы");
    else
      DBG(1, "Менеджер завершился, но причина остановки не совпадает с ожидаемой");
  }
}


int DBCheck() {
  if (write(g_fd, kCheckCommand, strlen(kCheckCommand) + 1) < 0)
    RETURN_LOG(-18, "Не удалось отправить команду \"check\" в менеджер БД: %s", strerror(errno));

  char answer[MAX_ANSWER_LENGTH];
  memset(answer, 0, MAX_ANSWER_LENGTH);
  if (read(g_fd, answer, MAX_ANSWER_LENGTH) < 0)
    RETURN_LOG(-19, "Не прочитан ответ от менеджера при выолнении команды check: %s",
               strerror(errno));

  LOG("Выполнена команда \"check\" менеджером БД. Ответ: %s", answer);
  return 0;
}
