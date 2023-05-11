// imen_chanel.cpp : Этот файл содержит функцию "main". Здесь начинается и заканчивается выполнение программы.
//
#include <windows.h>
#include <iostream>
#include <string.h>
#include "ServerOmn.h"
#include <tchar.h>

using namespace std;
int main()
{
    HANDLE hNamedPipe; 
    SECURITY_ATTRIBUTES sa; // атрибуты безопасности. 
    SECURITY_DESCRIPTOR sd; // дескриптор безопасности 
    DWORD dwBytesRead; // для количества прочитанных байтов 
    DWORD dwBytesWrite; // для количества записанных байтов 
    char pchMessage[80]; // для сообщения 
    int nMessageLength; // длина сообщения 
    // инициализация атрибутов безопасности 
    sa.nLength = sizeof(sa);
    sa.bInheritHandle = FALSE; // дескриптор канала ненаследуемый 
    // инициализируем дескриптор безопасности
    InitializeSecurityDescriptor(&sd, SECURITY_DESCRIPTOR_REVISION); // разрешаем доступ к именованному каналу всем пользователям 
    SetSecurityDescriptorDacl(&sd, TRUE, NULL, FALSE); 
    sa.lpSecurityDescriptor = &sd;
    // создаем именованный канал для чтения и записи
    hNamedPipe = CreateNamedPipe(
        L"\\\\.\\pipe\\demo_pipe", // имя канала 
        PIPE_ACCESS_DUPLEX, // читаем из канала и пишем в канал 
        PIPE_TYPE_MESSAGE | PIPE_WAIT, // синхронная передача сообщений 
        1, // максимальное количество экземпляров канала
        0, // размер выходного буфера по умолчанию
        0, // размер входного буфера по умолчанию
        INFINITE, // клиент ждет связь бесконечно долго
        &sa // доступ для всех пользователей 
    );

    // проверяем на успешное создание 
    if (hNamedPipe == INVALID_HANDLE_VALUE)
    {
        cerr << "--Failed to create named -- ('Ne udalos sozdat imenovannoi kanal')" << endl
            << "--Last error code-- ('Poslednii kod oshibki'): " << GetLastError() << endl;
        cout << "--Press any key to exit-- ('Najmite lubyu klavishu dlya vihoda'): "; 
        cin.get();
        return 0;
    }

    //ждем пока клиент свяжется с каналом
    cout << "--The server is waiting for a connection from the client-- ('Server ojidaet soedenenie s klientom')"<< endl;
    if (!ConnectNamedPipe(
        hNamedPipe,//дескриптор канала
        NULL //связь синхронная
    ))
    {
        cerr << "--Failed to connect named pipe-- ('Ne udalos podkluchit imenovannoi kanal')" << endl
            << "--Last error code-- ('Poslednii kod oshibki'): " << GetLastError() << endl;
        CloseHandle(hNamedPipe);
        cout << "--Press any key to exit-- ('Najmite lubyu klavishu dlya vihoda'):";
        cin.get();
        return 0;
    }

    //читаем сообщение клиента
    if (!ReadFile(
        hNamedPipe,
        pchMessage, //фдрес буфера
        sizeof(pchMessage),
        &dwBytesRead, //колич прочит байтов
        NULL))
    {
        cerr << "--Error reading data from named pipe-- ('Oshibka chteniya dannykh iz imenovannogo kanala')" << endl
            << "--Last error code-- ('Poslednii kod oshibki'):  " << GetLastError() << endl;
        CloseHandle(hNamedPipe);
        cout << "--Press any key to exit-- ('Najmite lubyu klavishu dlya vihoda'):";
        cin.get();
        return 0;
    }

    // выводим полученное от клиента соо на консоль
    cout << "--The server received a message from the client-- ('Server poluchil soobshcheniye ot kliyenta') "
        << endl << '\t' << pchMessage << endl;
    //вводим строку
    cout << "--Input to string-- ('Vvedite stroku'): ";
    cin.getline(pchMessage, 80);
    //определяем длину строки
    nMessageLength = strlen(pchMessage) + 1;

    //отвечаем клиенту
    if (!WriteFile(
        hNamedPipe,
        pchMessage,
        nMessageLength,
        &dwBytesWrite,
        NULL
    ))
    {
        cerr << "--Write file failed-- ('Ne udalos zapisat fail')" << endl
            << "--Last error code-- ('Poslednii kod oshibki'): " << GetLastError() << endl;
        CloseHandle(hNamedPipe);
        cout << "--Press any key to exit-- ('Najmite lubyu klavishu dlya vihoda'):";
        cin.get();
        return 0;
    }
    //выводим клиентку соо
    cout << "--The server sent the message to a client-- ('Server otpravil soobshcheniye kliyentu'): "
        << endl << '\t' << pchMessage << endl;
    //завершаем процесс
    CloseHandle(hNamedPipe);
    cout << "--Press any key to exit-- ('Najmite lubyu klavishu dlya vihoda'):";
    cin.get();
    return 0;
}

// Запуск программы: CTRL+F5 или меню "Отладка" > "Запуск без отладки"
// Отладка программы: F5 или меню "Отладка" > "Запустить отладку"

// Советы по началу работы 
//   1. В окне обозревателя решений можно добавлять файлы и управлять ими.
//   2. В окне Team Explorer можно подключиться к системе управления версиями.
//   3. В окне "Выходные данные" можно просматривать выходные данные сборки и другие сообщения.
//   4. В окне "Список ошибок" можно просматривать ошибки.
//   5. Последовательно выберите пункты меню "Проект" > "Добавить новый элемент", чтобы создать файлы кода, или "Проект" > "Добавить существующий элемент", чтобы добавить в проект существующие файлы кода.
//   6. Чтобы снова открыть этот проект позже, выберите пункты меню "Файл" > "Открыть" > "Проект" и выберите SLN-файл.
