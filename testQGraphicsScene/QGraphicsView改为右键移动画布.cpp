QGraphicsView的源代码写的是鼠标左键触发移动画布的

class MyGraphicsView : public QGraphicsView
{
    Q_OBJECT
public:
    explicit MyGraphicsView(QWidget *parent = nullptr);

protected:
    void mousePressEvent(QMouseEvent *event) Q_DECL_OVERRIDE;
    void mouseReleaseEvent(QMouseEvent *event) Q_DECL_OVERRIDE;
};

MyGraphicsView::MyGraphicsView(QWidget *parent)
    : QGraphicsView(parent)
{
    this->setMouseTracking(true);
    this->setVerticalScrollBarPolicy(Qt::ScrollBarAlwaysOn);
    this->setHorizontalScrollBarPolicy(Qt::ScrollBarAlwaysOn);
}

void MyGraphicsView::mousePressEvent(QMouseEvent *event)
{
   if (event->button() == Qt::RightButton){
       event->accept();
       QMouseEvent fakeEvent(event->type(), event->pos(), Qt::LeftButton, Qt::NoButton, event->modifiers());
       this->setDragMode(ScrollHandDrag);
       QGraphicsView::mousePressEvent(&fakeEvent);
   }else{
       QGraphicsView::mousePressEvent(event);
   }
}

void MyGraphicsView::mouseReleaseEvent(QMouseEvent *event)
{
    this->setDragMode(NoDrag);

    QGraphicsView::mouseReleaseEvent(event);
}