#ifndef QBind_H
#define QBind_H

#include "attribute\attribute.h"

class QAttribute;

class EXPOSURE_RENDER_DLL QBinder : public QWidget
{
    Q_OBJECT

public:
    QBinder(QAttribute* Attribute, QWidget* Parent = 0);
    virtual ~QBinder();

protected:
	QHBoxLayout*	Layout;

private:
	QAttribute*		Attribute;
	QLabel*			Label;
};

#endif
