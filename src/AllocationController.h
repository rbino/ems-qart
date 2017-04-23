#ifndef _ALLOCATIONCONTROLLER_H_
#define _ALLOCATIONCONTROLLER_H_

#include "EmsCart.h"

#include <QObject>

class Allocator;
class RomListModel;

class AllocationController : public QObject
{
    Q_OBJECT

    Q_PROPERTY(EmsCart::Bank bank READ bank WRITE setBank)
    Q_PROPERTY(QObject * romsModel READ romsModel NOTIFY romsModelChanged)

    public:
        AllocationController(QObject *parent = nullptr);
        virtual ~AllocationController();

        QObject *romsModel() const;
        EmsCart::Bank bank() const;

        void setBank(EmsCart::Bank bank);

    signals:
        void romsModelChanged();
        void bankChanged(EmsCart::Bank bank);

    private:
        RomListModel *m_romsModel;
        Allocator *m_allocator;

        EmsCart::Bank m_bank;
};

#endif // _ALLOCATIONCONTROLLER_H_
