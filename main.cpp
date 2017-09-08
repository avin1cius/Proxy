#include "main.h"

bool server_on;

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);

    Data sharedData;

    std::thread tProducer;

    std::thread *tReader = new std::thread [5];

    Semaphore *semaphore = new Semaphore();

    Producer producer( sharedData, *semaphore );

    Reader reader( sharedData, *semaphore );

    MainWindow w( producer, reader, sharedData, tReader, tProducer );

    w.show();

    return a.exec();
    //while(1);
}
