# Библиотека для записи сообщений в журнал с разными уровнями важности.

Этот проект представляет собой библиотеку для записи сообщений в журнал с разными уровнями важности и приложение,
демонстрирующее работу библиотеки. 

## Структура проекта

-   `/include/Library/Library.h`: Заголовочный файл библиотеки.
-   `/include/ThreadClasses/ThreadClasses.h`: Заголовочный файл реализации потокобезопасной очереди и функций обработки запросов.
-   `/include/Program/Program.h`: Заголовочный файл приложения.
-   `src/`: Файлы реализации двух версий библиотеки (с использованием сокетов и без).
-   `ThreadClasses/ThreadClasses.cpp`: Реализация потокобезопасной очереди и функций обработки запросов.
-   `Program/Program.cpp`: Реализация приложения.
-   `Makefile`: Makefile с целями: all, install, uninstall, clean, tests, dvi, dist.
-   `main.cpp`: Запуск программы.

## Установка

1.  Клонируйте репозиторий:
    ```bash
    git clone https://github.com/TarasovaAlina/infotecs_task_C_.git
    ```
2.  Установите необходимые библиотеки:
    ```bash
    sudo apt install cpp g++ gcc build-essential cmake
    ```

## Сборка проекта

Скопировать код из репозитория
```bash
git clone  https://github.com/TarasovaAlina/infotecs_task_C_.git
```

Чтобы установить программу, необходимо ввести команду:
```bash
make install
```

И запустить:
```bash
./build/LogLibrary
```

Сборка статической библиотеки:
```bash
make static_library
```

Сборка динамической библиотеки:
```bash
make dynamic_library
```

Если необходимо удалить программу, то
```bash
make uninstall
```

## Использование


