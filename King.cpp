#include "King.h"
#include "Wave.h"
#include <QGraphicsScene>
#include <QPixmap>
#include <QTimer>
#include "KingWave.h"
#include <QPointer>
#include "Player.h"
King::King(QObject* parent) : PhantomBase()
{
    setMaxHp(30000);
    hp = maxHp;
    setAtk(2000);
    setAtkRange(800);
    setSpeed(6.0f);
    setSightRange(2000);
    setStaticPixmap(QPixmap(":/Resource/king.png").scaled(128, 128));
    createRangeIndicator(QColor(255, 215, 0, 60), 3);

    rangeTimer = new QTimer(this);
    connect(rangeTimer, &QTimer::timeout, this, [this]() {
        setRangeIndicatorVisible(true);
        QPointer<King> self(this);
        QTimer::singleShot(800, [self]() {
            if (self) self->setRangeIndicatorVisible(false);
        });
    });
    rangeTimer->start(2000);

    waveTimer = new QTimer(this);
    connect(waveTimer, &QTimer::timeout, this, &King::updateStatus); 
    waveTimer->start(60);
}

King::~King()
{
    if (rangeTimer) rangeTimer->stop();
    if (waveTimer) waveTimer->stop();
}

void King::updateStatus()
{
    if (!scene()) return;
    if (!player) {
        for (QGraphicsItem* item : scene()->items()) {
            player = dynamic_cast<Player*>(item);
            if (player) break;
        }
    }
    if (!player) return;

    QPointF kingCenter = pos() + boundingRect().center();
    QPointF playerCenter = player->pos() + QPointF(player->boundingRect().width()/2, player->boundingRect().height()/2);
    qreal distance = QLineF(kingCenter, playerCenter).length();
    if (distance <= getSightRange()) {
        QPointF dir = playerCenter - kingCenter;
        qreal len = std::hypot(dir.x(), dir.y());
        if (len > 1e-2) {
            dir /= len;
            setPos(pos() + dir * getSpeed());
        }
        static int attackCD = 1500;
        static QElapsedTimer atkTimer;
        if (!atkTimer.isValid()) atkTimer.start();
        if (distance <= getAtkRange() && atkTimer.elapsed() >= attackCD) {
            shootAllDirections();
            atkTimer.restart();
        }
    }
}

void King::shootAllDirections()
{
    static const QVector<QPointF> dirs = {
        {1,0}, {0,1}, {-1,0}, {0,-1}, {1,1}, {-1,1}, {-1,-1}, {1,-1}
    };
    for (const QPointF& dir : dirs) {
        shootWave(dir);
    }
}

void King::shootWave(const QPointF& direction)
{
    if (!scene()) return;
    KingWave* wave = new KingWave(direction, getAtk(), this);
    wave->setIgnoreItem(this);
    QPointF kingCenter = pos() + boundingRect().center();
    QPointF waveTopLeft = kingCenter - wave->boundingRect().center();
    wave->setPos(waveTopLeft);
    scene()->addItem(wave);
}
void King::stopTimers() {
    if (rangeTimer) rangeTimer->stop();
    if (waveTimer) waveTimer->stop();

}
void King::startTimers() {
    if (rangeTimer && !rangeTimer->isActive()) rangeTimer->start(2000);
    if (waveTimer && !waveTimer->isActive()) waveTimer->start(60);
}
