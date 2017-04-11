#ifndef _ALLOCATIONCONTROLLER_H_
#define _ALLOCATIONCONTROLLER_H_

#include <QObject>

class RomListModel;

class AllocationController : public QObject
{
    Q_OBJECT

    Q_PROPERTY(QObject * romsModel READ romsModel NOTIFY romsModelChanged)

    public:
        AllocationController(QObject *parent = nullptr);
        virtual ~AllocationController();

        QObject *romsModel() const;

    signals:
        void romsModelChanged();

    private:
        RomListModel *m_romsModel;
};

#endif // _ALLOCATIONCONTROLLER_H_
