#ifndef IMAGE_GALLERY_CONTROLLER_H
#define IMAGE_GALLERY_CONTROLLER_H

#include <QObject>

/**
 * @brief The ImageGalleryController class
 * 
 * Analogous to the video and audio controllers for photo galleries.
 */
class ImageGalleryController : public QObject
{
    Q_OBJECT
public:
    explicit ImageGalleryController(QObject *parent = nullptr);

signals:

public slots:
};

#endif // IMAGE_GALLERY_CONTROLLER_H
