#include "Map.h"
#include <QGraphicsPixmapItem>
#include <QDebug>
#include <QDir>

Map::Map(QGraphicsScene* scene) : gameScene(scene) {
    for (int i = 0; i < GRID_ROW; i++) {
        for (int j = 0; j < GRID_COL; j++) {
            mapItems[i][j] = nullptr;
            currentMapData[i][j] = 0; // 开始时是黑暗视野
        }
    }
     // 初始化所有地图布局
    allMapLayouts.resize(3); 
    allMapLayouts[0].resize(GRID_ROW);
    for (int i = 0; i < GRID_ROW; ++i) {
        allMapLayouts[0][i].resize(GRID_COL);
        for (int j = 0; j < GRID_COL; ++j) {
            allMapLayouts[0][i][j] = 0;
        }
    }
    allMapLayouts[1] = {
        {2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2},
        {2,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,2},
        {2,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,2},
        {2,0,0,0,0,0,0,0,0,0,0,2,0,2,0,0,0,0,0,0,0,0,0,2},
        {2,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,2},
        {2,0,0,0,0,0,0,0,0,2,0,2,0,0,0,0,0,0,0,0,0,0,0,2},
        {2,0,0,0,0,0,0,0,0,0,0,0,0,0,2,0,2,0,0,0,0,0,0,2},
        {2,0,0,0,0,0,0,0,0,2,1,0,1,2,0,0,0,0,0,0,0,0,0,2},
        {2,0,0,0,0,0,0,0,0,0,0,2,1,0,0,0,1,0,0,0,0,0,0,2},
        {2,0,0,0,0,0,0,2,0,0,0,0,0,0,2,0,0,0,0,0,0,0,0,2},
        {2,0,0,0,0,0,0,0,0,2,0,2,0,0,0,0,2,0,0,0,0,0,0,2},
        {2,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,2},
        {2,0,0,0,0,0,0,2,0,0,0,2,0,0,2,0,0,0,0,0,0,0,0,2},
        {2,0,0,0,0,0,0,0,0,0,1,0,1,0,0,0,0,0,0,0,0,0,0,2},
        {2,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,2},
        {2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2}
    };
    allMapLayouts[2] = {
        {2,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,2},
        {2,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,2},
        {2,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,2},
        {2,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,2},
        {2,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,2},
        {2,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,2},
        {2,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,2},
        {2,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,2},
        {2,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,2},
        {2,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,2},
        {2,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,2},
        {2,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,2},
        {2,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,2},
        {2,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,2},
        {2,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,2},
        {2,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,2}
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
        case 0://纯黑视野专用
            imagePath = "Resource/black.png";
            break;
        case 1:
            imagePath = "Resource/flame.png";
            break;
        case 2:
            imagePath = "Resource/portal.png";
            break;
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
    currentMapData[row][col] = tileType; // 更新当前地图数据
}
int Map::getTileType(int row, int col) const {//获取当前地图数据
    if (row >= 0 && row < GRID_ROW && col >= 0 && col < GRID_COL) {
        return currentMapData[row][col];
    }
    return 0; // 默认返回纯黑
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
        }
    }
}

void Map::updateMap() {//地图整体状态更新

}
