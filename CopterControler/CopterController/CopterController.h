#ifndef COPTERCONTROLLER_H
#define COPTERCONTROLLER_H

#include <ui_CopterController.h>
#include <QtWidgets/QWidget>
#include <QDebug>
#include "rs232.h"



enum COPTER_COMMANDS
{

    ENGINES_ARM,

    ENGINES_STOP,

    COPTER_RAISE,

    COPTER_DROP

};



class CopterController
    :
        public QWidget
{

    Q_OBJECT

    public:

                                    CopterController()
                                        :
                                            comPortFrequency(9600)
        {

            ui.setupUi(this);

            QObject::connect(ui.COMPortConnect,SIGNAL(clicked()),this,SLOT(comPortConnect()));
            QObject::connect(ui.copterAcelerate,SIGNAL(clicked()),this,SLOT(copterAcelerate()));
            QObject::connect(ui.copterSlow,SIGNAL(clicked()),this,SLOT(copterSlow()));
            QObject::connect(ui.copterStop,SIGNAL(clicked()),this,SLOT(copterStop()));
            QObject::connect(ui.copterArm,SIGNAL(clicked()),this,SLOT(copterArm()));

        }

                                    ~CopterController()
        {

            CloseComport(this->comPortId);

        }

    public slots:

        void                        comPortConnect()
        {

            ui.COMPort->setEnabled(false);
            this->comPortId = ui.COMPortNumber->value()-1;

            //  If COM port opened successfully
            if (OpenComport(this->comPortId,comPortFrequency) == 0)
            {

                ui.copterAcelerator->setEnabled(true);

            }

            else
            {

               ui.COMPort->setEnabled(true);

            }

        }

        void                        copterAcelerate()
        {

            SendByte(this->comPortId,COPTER_RAISE);

        }

        void                        copterSlow()
        {

            SendByte(this->comPortId,COPTER_DROP);

        }

        void                        copterStop()
        {

            SendByte(this->comPortId,ENGINES_STOP);

        }

        void                        copterArm()
        {

            SendByte(this->comPortId,ENGINES_ARM);

        }

    private:

        int                         comPortFrequency;

        int                         comPortId;

        Ui::CopterControllerUi      ui;

};



#endif // COPTERCONTROLLER_H
