#ifndef QHIMAGE_H
#define QHIMAGE_H

#include <QWidget>
#include <QFile>

namespace Ui {
class qhImage;
}

class qhImage : public QWidget
{
    Q_OBJECT

    Q_PROPERTY(QString PicPath READ PicPath WRITE SetPicPath)
    Q_PROPERTY(E_ShowType ShowType READ ShowType WRITE SetShowType)

public:
    explicit qhImage(QWidget *parent = nullptr);
    ~qhImage();

    enum E_ShowType {Preferred, Expanding, Repeat};//自适应,拉伸,平铺
    Q_ENUM(E_ShowType)

    QString PicPath() const;
    void SetPicPath(const QString &pic);

    E_ShowType ShowType() const;
    void SetShowType(E_ShowType st);

private:
    Ui::qhImage *ui;

    QPixmap mPixmap;
    QString mPicPath;
    E_ShowType mShowType;

    void UpdatePic();
};

#endif // QHIMAGE_H
