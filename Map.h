#ifndef MAP_H
#define MAP_H
#include <QGraphicsScene>
#include <QGraphicsPixmapItem>
#include <QString>
#include <QVector>
class Map{
public:
    Map(QGraphicsScene* scene);
    ~Map();//清理地图
    void initMap(int mapId);//初始化地图
    void updateMap();//地图整体状态更新，不只是格子
    QRectF getMapBounds()const;//获取地图边界;
    int getGridSize()const;//获取格子大小
    int getGridRow()const;//获取行数
    int getGridCol()const;//获取列数
    int getOriginalTileType(int row, int col, int mapId) const;//获取原始格子类型，大概不变
    int getTileType(int row, int col) const;//获取当前的格子类型
    const QVector<QPoint>& getFlamePhantomBases() const { return flamePhantomBases; }//获取flamePhantom生成点
    const QVector<QPoint>& getLurkPhantomBases() const { return lurkPhantomBases; }//获取lurkPhantom生成点
    void updateSingleTile(int row, int col, int tileType);//单格子更新
private:
    QGraphicsScene* gameScene;//game场景
    static const int GRID_SIZE = 512;//格子大小
    static const int GRID_ROW = 21; //行数
    static const int GRID_COL = 25; //列数
    QGraphicsPixmapItem* mapItems[GRID_ROW][GRID_COL];//格子数组
    int currentMapData[GRID_ROW][GRID_COL];//当前地图数据
    QVector<QPoint> flamePhantomBases;//flamePhantom生成点
    QVector<QPoint> lurkPhantomBases;//lurkPhantom生成点
    QVector<QVector<QVector<int>>> allMapLayouts;//所有地图布局
    void loadMapTile(int row, int col, int tileType);//加载地图
};

#endif // MAP_H
