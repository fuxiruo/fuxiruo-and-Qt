#include "myqtpropertybrowser.h"
#include "ui_myqtpropertybrowser.h"
#include <QMetaObject>
#include <QMetaProperty>
#include <QDebug>
#include "qtvariantproperty.h"

MyQtPropertyBrowser::MyQtPropertyBrowser(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::MyQtPropertyBrowser)
{
    ui->setupUi(this);
    mbWidgetSetting = false;
    mWidget = nullptr;
    mIgnoreProperty<<"x"<<"y"<<"pos"<<"size"<<"minimumWidth"<<"minimumHeight"<<"maximumWidth"<<"maximumHeight";

    /*!
         如果有些内容想让编辑，有些内容不想让用户编辑，可以设置两个manager，
        一个设置Factory另外一个不设置，那么设置了Factory的manager生成的属性就可以编辑，没设置的就不可用编辑了
    */
    mVarManager = new QtVariantPropertyManager(ui->treePropertyBrowser);
    mReadOnlyVarManager = new QtVariantPropertyManager(ui->treePropertyBrowser);
    connect(mVarManager, &QtVariantPropertyManager::valueChanged, this, &MyQtPropertyBrowser::OnValueChanged);

    mVarFactory = new QtVariantEditorFactory(ui->treePropertyBrowser);
    ui->treePropertyBrowser->setFactoryForManager(mVarManager, mVarFactory);
}

MyQtPropertyBrowser::~MyQtPropertyBrowser()
{
    delete ui;
}

void MyQtPropertyBrowser::SetWidgte(QWidget *widget)
{
    mbWidgetSetting = true;
    mWidget = widget;

    ui->treePropertyBrowser->setResizeMode(QtTreePropertyBrowser::ResizeToContents);
    QtProperty *groupProperty;
    const QMetaObject* metaObject = mWidget->metaObject();
    QList<const QMetaObject *> superList;
    superList.append(metaObject);
    const QMetaObject *super = metaObject->superClass();
    while(super){
        superList.append(super);
        super = super->superClass();
    }

    QtVariantProperty *item;
    while(superList.size() > 0){
        super = superList.last();
        groupProperty = mVarManager->addProperty(QtVariantPropertyManager::groupTypeId(), super->className());

        for(int i = super->propertyOffset(); i < super->propertyCount(); ++i){
            if(super->property(i).isWritable()){
                if(mIgnoreProperty.contains(super->property(i).name())){//有些重复的属性不用重复添加
                    continue;
                }

                int propertyType = super->property(i).type();
                if(super->property(i).isFlagType()){
                    propertyType = QVariant::String;
                    item = mReadOnlyVarManager->addProperty(propertyType, super->property(i).name());
                }else if(super->property(i).isEnumType()){
                    propertyType = QtVariantPropertyManager::enumTypeId();
                    item = mVarManager->addProperty(propertyType, super->property(i).name());
                }else {
                    item = mVarManager->addProperty(propertyType, super->property(i).name());
                }

                if(item){
                    QVariant qv = mWidget->property(super->property(i).name());

                    if(super->property(i).isFlagType()){//FlagType需要特殊处理
                        //TODO:FlagType勾选对应flagProperty后，flagProperty的值没有反映到item上
                        QMetaEnum qme = super->property(i).enumerator();
                        QByteArray qmeText;
                        if(!qv.isNull()){
                            qmeText = qme.valueToKeys(qv.toInt());
                        }
                        for(int j = 0; j < qme.keyCount(); j++){
                            QtVariantProperty *flagProperty = mVarManager->addProperty(QVariant::Bool, qme.key(j));
                            if(qmeText.contains(qme.key(j))){
                                flagProperty->setValue(true);
                            }
                            item->addSubProperty(flagProperty);
                            item->setValue(qmeText);
                        }
                    }else{
                        if(!qv.isNull()){
                            item->setValue(qv);
                        }
                    }
                    groupProperty->addSubProperty(item);
                }
            }
        }
        AddGroupProperty(groupProperty, QColor("#ffffbf"), QColor("#a0a0a0"));
        superList.pop_back();
    }

    mbWidgetSetting = false;
    ui->treePropertyBrowser->setResizeMode(QtTreePropertyBrowser::Interactive);
}

void MyQtPropertyBrowser::OnValueChanged(QtProperty *property, const QVariant &val)
{
    if(!mbWidgetSetting && mWidget){
        mWidget->setProperty(property->propertyName().toUtf8().data(), val);
    }
}

void MyQtPropertyBrowser::AddGroupProperty(QtProperty *property, const QColor &childColor, const QColor &parentColor)
{
    QtBrowserItem *qBrowserItem = ui->treePropertyBrowser->addProperty(property);
    ui->treePropertyBrowser->setBackgroundColor(qBrowserItem, parentColor);
    foreach (QtBrowserItem *item, qBrowserItem->children()) {
        ui->treePropertyBrowser->setBackgroundColor(item, childColor);
    }
}

