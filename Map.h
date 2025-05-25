#ifndef MAP_H
#define MAP_H
#include <QGraphicsScene>
#include <QGraphicsPixmapItem>
#include <QString>
class Map{
public:
    Map(QGraphicsScene* scene);
    ~Map();//清理地图
    void initMap(int mapId);//初始化地图
    void updateMap();//更新地图
    QRectF getMapBounds()const;//获取地图边界;
    int getGridSize()const;//获取格子大小
private:
    QGraphicsScene* gameScene;
    static const int GRID_SIZE = 50;//格子大小
    static const int GRID_ROW = 12; // 地图行数
    static const int GRID_COL = 18; // 地图列数
    QGraphicsPixmapItem* mapItems[GRID_ROW][GRID_COL]; // 格子数组
    void loadMapTile(int row, int col, int tileType);//加载地图

};

#endif // MAP_H
