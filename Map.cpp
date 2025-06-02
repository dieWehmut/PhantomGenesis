#include "Map.h"
#include <QGraphicsPixmapItem>
#include <QDebug>
#include <QDir>

Map::Map(QGraphicsScene* scene) : gameScene(scene) {
    for (int i = 0; i < GRID_ROW; i++) {
        for (int j = 0; j < GRID_COL; j++) {
            mapItems[i][j] = nullptr;
            currentMapData[i][j] = 0;
        }
    }
    allMapLayouts.resize(2); 
    allMapLayouts[0] = {
       //1, 2, 3, 4, 5, 6, 7, 8, 9,10,11,12,13,14,15,16,17,18,19,20,21,22,23,24,25  
        {2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 1 },//1
        {2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 1 },//2
        {2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 1 },//3
        {2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 1 },//4
        {2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 1 },//5
        {2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 1 },//6
        {2, 2, 2, 2, 2, 2, 2, 2,-1,-1,-1,-1,-1,-1,-2,-1,-1, 2, 2, 2, 2, 2, 2, 2, 1 },//7
        {2, 2, 2, 2, 2, 2, 2, 2,-1, 2, 3, 2, 2, 4, 2, 2,-1, 2, 2, 2, 2, 2, 2, 2, 1 },//8
        {2, 2, 2, 2, 2, 2, 2, 2,-2, 2, 2, 2, 2, 2, 2, 3,-1, 2, 2, 2, 2, 2, 2, 2, 1 },//9
        {2, 2, 2, 2, 2, 2, 2, 2,-1, 4, 2, 2, 2, 2, 2, 2,-1, 2, 2, 2, 2, 2, 2, 2, 1 },//10
        {2, 2, 2, 2, 2, 2, 2, 2,-1, 2, 2, 2, 0, 2, 2, 2,-1, 2, 2, 2, 2, 2, 2, 2, 1 },//11
        {2, 2, 2, 2, 2, 2, 2, 2,-1, 2, 2, 2, 2, 2, 2, 4,-1, 2, 2, 2, 2, 2, 2, 2, 1 },//12
        {2, 2, 2, 2, 2, 2, 2, 2,-1, 3, 2, 2, 2, 2, 2, 2,-2, 2, 2, 2, 2, 2, 2, 2, 1 },//13
        {2, 2, 2, 2, 2, 2, 2, 2,-1, 2, 2, 4, 2, 2, 3, 2,-1, 2, 2, 2, 2, 2, 2, 2, 1 },//14
        {2, 2, 2, 2, 2, 2, 2, 2,-1,-1,-2,-1,-1,-1,-1,-1,-1, 2, 2, 2, 2, 2, 2, 2, 1 },//15
        {2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 1 },//16
        {2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 1 },//17
        {2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 1 },//18
        {2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 1 },//19
        {2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 1 },//20
        {2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 1 }//21
    };
    allMapLayouts[1] = {
       //1, 2, 3, 4, 5, 6, 7, 8, 9,10,11,12,13,14,15,16,17,18,19,20,21,22,23,24,25  
        {2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 1 },//1
        {2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 1 },//2
        {2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 1 },//3
        {2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 1 },//4
        {2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 1 },//5
        {2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 1 },//6
        {2, 2, 2, 2, 2, 2, 2, 2,-1,-1,-1,-1,-1,-1,-1,-1,-1, 2, 2, 2, 2, 2, 2, 2, 1 },//7
        {2, 2, 2, 2, 2, 2, 2, 2,-1, 2, 2, 2, 2, 2, 2, 2,-1, 2, 2, 2, 2, 2, 2, 2, 1 },//8
        {2, 2, 2, 2, 2, 2, 2, 2,-1, 2, 2, 2, 2, 2, 2, 2,-1, 2, 2, 2, 2, 2, 2, 2, 1 },//9
        {2, 2, 2, 2, 2, 2, 2, 2,-1, 2, 2, 2, 2, 2, 2, 2,-1, 2, 2, 2, 2, 2, 2, 2, 1 },//10
        {2, 2, 2, 2, 2, 2, 2, 2,-1, 2, 2, 2, 0, 2, 2, 2,-1, 2, 2, 2, 2, 2, 2, 2, 1 },//11
        {2, 2, 2, 2, 2, 2, 2, 2,-1, 2, 2, 2, 2, 2, 2, 2,-1, 2, 2, 2, 2, 2, 2, 2, 1 },//12
        {2, 2, 2, 2, 2, 2, 2, 2,-1, 2, 2, 2, 2, 2, 2, 2,-1, 2, 2, 2, 2, 2, 2, 2, 1 },//13
        {2, 2, 2, 2, 2, 2, 2, 2,-1, 2, 2, 2, 2, 2, 2, 2,-1, 2, 2, 2, 2, 2, 2, 2, 1 },//14
        {2, 2, 2, 2, 2, 2, 2, 2,-1,-1,-1,-1,-1,-1,-1,-1,-1, 2, 2, 2, 2, 2, 2, 2, 1 },//15
        {2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 1 },//16
        {2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 1 },//17
        {2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 1 },//18
        {2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 1 },//19
        {2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 1 },//20
        {2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 1 }//21
    };
}
Map::~Map() {//清理地图
    for (int i = 0; i < GRID_ROW; i++) {
        for (int j = 0; j < GRID_COL; j++) {
            if (mapItems[i][j]) {
                mapItems[i][j] = nullptr;
            }
        }
    }
}
QRectF Map::getMapBounds() const {  
    return QRectF(0, 0, GRID_COL * GRID_SIZE, GRID_ROW * GRID_SIZE);
}
int Map::getGridSize()const{
    return GRID_SIZE;
}
int Map::getGridRow()const{
    return GRID_ROW;
}
int Map::getGridCol()const{
    return GRID_COL;
}
void Map::loadMapTile(int row, int col, int tileType) {
    QString imagePath;
    switch(tileType) {
        case -3: imagePath = "Resource/mapExit.png"; break;// 地图出口
        case -2: imagePath = "Resource/mazeExit.png"; break;// 迷宫出口
        case -1: imagePath = "Resource/portal.png"; break;// 传送门
        case 1: imagePath = "Resource/flame.png"; break;// 火焰
        case 2: imagePath = "Resource/ruins.png"; break;// 废墟
        case 3: imagePath = "Resource/flamePhantomBase.png"; break;//flamePhantom生成点
        case 4: imagePath = "Resource/lurkPhantomBase.png"; break;//lurkPhantom生成点
        default: imagePath = "Resource/ruins.png"; break; 
    }
    QGraphicsPixmapItem* item = new QGraphicsPixmapItem(QPixmap(imagePath));
    item->setPos(col * GRID_SIZE, row * GRID_SIZE);
    item->setZValue(-1);
    gameScene->addItem(item);
    mapItems[row][col] = item;
}

void Map::updateSingleTile(int row, int col, int tileType) {
    if (mapItems[row][col]) {
        gameScene->removeItem(mapItems[row][col]);
        delete mapItems[row][col];
        mapItems[row][col] = nullptr;
    }
    loadMapTile(row, col, tileType);
    currentMapData[row][col] = tileType;
}
int Map::getTileType(int row, int col) const {//获取当前地图数据
    if (row >= 0 && row < GRID_ROW && col >= 0 && col < GRID_COL) {
        return currentMapData[row][col];
    }
    return 0;
}
int Map::getOriginalTileType(int row, int col, int mapId) const {//获取原始地图数据
    if (mapId >= 0 && mapId < allMapLayouts.size() &&
        row >= 0 && row < GRID_ROW &&
        col >= 0 && col < GRID_COL) {
        return allMapLayouts[mapId][row][col];
    }
    return 0;
}
void Map::initMap(int mapId) {
    // 清理地图
    flamePhantomBases.clear();
    lurkPhantomBases.clear();
    for (int i = 0; i < GRID_ROW; i++) {
        for (int j = 0; j < GRID_COL; j++) {
            if (mapItems[i][j]) {
                gameScene->removeItem(mapItems[i][j]);
                delete mapItems[i][j];
                mapItems[i][j] = nullptr;
                currentMapData[i][j] = 0; 
            }
        }
    }
    if (mapId < 0 || mapId >= allMapLayouts.size()) {
        mapId = 0;
    }
    const auto& srcMap = allMapLayouts[mapId];
    for(int i=0; i<GRID_ROW; i++) {//加载地图
        for(int j=0; j<GRID_COL; j++) {
            currentMapData[i][j] = srcMap[i][j];
            loadMapTile(i, j, currentMapData[i][j]);
            if (currentMapData[i][j] == 3) {//添加flamePhantom生成点
                flamePhantomBases.append(QPoint(j, i));
            }
            if (currentMapData[i][j] == 4) {//添加lurkPhantom生成点
                lurkPhantomBases.append(QPoint(j, i));
            }
        }
    }
}

void Map::updateMap() {//地图整体状态更新

}
