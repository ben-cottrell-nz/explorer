#include <QFile>
#include <QCoreApplication>

int main(int argc, char* argv[])
{
    QCoreApplication app(argc, argv);
    QFile::rename("/home/ben/Documents/test/folderA/foo.txt",
                  "/home/ben/Documents/test/folderA/fooa.txt");
    return 0;
}
