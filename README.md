# Тестовое задание Yadro 

Программа представляет из себя прототип системы,
которая следит за работой компьютерного клуба
обрабатывает события и подсчитывает выручку 
за день и время занятости каждого стола.

### Build
```shell
git clone https://github.com/sddwbbs/yadro_test.git
cd yadro_test
mkdir build && cd build
cmake ..
make
```

### Tests
Для запуска тестов в папке `build` выполнить команду:
```shell
./unit_tests
```

### Run
Для запуска потребуется указать файл с входными данными первым аргументом программы

**Пример:** 
```shell
./yadro_test input.txt
```

### Дополнительно
Программа учитывает пробелы в конце строки во входном файле 