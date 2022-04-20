#ifndef MYQTPROPERTYBROWSER_H
#define MYQTPROPERTYBROWSER_H

#include <QWidget>
#include <QMap>

namespace Ui {
class MyQtPropertyBrowser;
}

class QtProperty;
class QtVariantPropertyManager;
class QtVariantEditorFactory;

class MyQtPropertyBrowser : public QWidget
{
    Q_OBJECT

public:
    explicit MyQtPropertyBrowser(QWidget *parent = nullptr);
    ~MyQtPropertyBrowser();

    void SetWidgte(QWidget *widget);

protected slots:
    void OnValueChanged(QtProperty *property, const QVariant &val);

private:
    Ui::MyQtPropertyBrowser *ui;

    QWidget *mWidget;
    QStringList mIgnoreProperty;
    bool mbWidgetSetting;

    QtVariantPropertyManager *mVarManager;
    QtVariantPropertyManager *mReadOnlyVarManager;
    QtVariantEditorFactory *mVarFactory;

    void AddGroupProperty(QtProperty *property, const QColor &childColor=QColor(), const QColor &parentColor=QColor());
};

#endif // MYQTPROPERTYBROWSER_H
