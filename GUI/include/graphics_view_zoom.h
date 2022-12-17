#ifndef GRAPHICVIEWZOOM_H
#define GRAPHICVIEWZOOM_H
#include <QMainWindow>
#include <QGraphicsPixmapItem>
#include <QFileSystemModel>
#include <QGraphicsView>


// зуммирование окна отображения в GUI
class Graphics_view_zoom : public QObject {
Q_OBJECT
public:
    Graphics_view_zoom(QGraphicsView* view); 
    void gentle_zoom(double factor); // зум экрана
    void set_modifiers(Qt::KeyboardModifiers modifiers); 
    void set_zoom_factor_base(double value);

private:
    QGraphicsView* _view;
    Qt::KeyboardModifiers _modifiers;
    double _zoom_factor_base;
    QPointF target_scene_pos, target_viewport_pos;
    bool eventFilter(QObject* object, QEvent* event);

    bool pressed = false;

    QPointF prev_position;

signals:
    void zoomed();
    void moved();
};


#endif
