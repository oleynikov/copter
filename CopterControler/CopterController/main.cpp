#include <QApplication>
#include <CopterController.h>

int main(int argc, char* argv[])
{

    QApplication application(argc,argv);
    CopterController controller;
    controller.show();

    return application.exec();

}
