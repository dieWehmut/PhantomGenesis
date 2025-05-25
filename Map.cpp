#include "Map.h"
#include <QGraphicsPixmapItem>
#include <QDebug>
#include <QDir>

Map::Map(QGraphicsScene* scene) : gameScene(scene) {
    for (int i = 0; i < GRID_ROW; i++) {
        for (int j = 0; j < GRID_COL; j++) {
            mapItems[i][j] = nullptr;
        }
    }
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
void Map::loadMapTile(int row, int col, int tileType) {
    QString imagePath;
    switch(tileType) {
        case 0:
            imagePath = "Resource/map0.png";
            break;
        case 1:
            imagePath = "Resource/map1.png";
            break;
        case 2:
            imagePath = "Resource/map2.png";
            break;
    }
    QGraphicsPixmapItem* item = new QGraphicsPixmapItem(QPixmap(imagePath));
    item->setPos(col * GRID_SIZE, row * GRID_SIZE);
    gameScene->addItem(item);
    mapItems[row][col] = item;
}
void Map::initMap(int mapId) {
    // 清理地图
    for (int i = 0; i < GRID_ROW; i++) {
        for (int j = 0; j < GRID_COL; j++) {
            if (mapItems[i][j]) {
                gameScene->removeItem(mapItems[i][j]);
                delete mapItems[i][j];
                mapItems[i][j] = nullptr;
            }
        }
    }
    switch (mapId) {
        case 1: {
            int mapLayout[GRID_ROW][GRID_COL] = {
                {1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1}, 
                {1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1},
                {1,0,2,2,2,0,0,0,0,0,0,0,0,0,0,0,0,1},
                {1,0,2,0,2,0,0,0,0,0,0,0,0,0,0,0,0,1},
                {1,0,2,2,2,0,0,0,0,0,0,0,0,0,0,0,0,1},
                {1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1},
                {1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1},
                {1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1},
                {1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1},
                {1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1},
                {1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1},
                {1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1}
            };
            
            for (int row = 0; row < GRID_ROW; row++) {
                for (int col = 0; col < GRID_COL; col++) {
                    loadMapTile(row, col, mapLayout[row][col]);
                }
            }
            break;
        } 
        case 2: {
            int mapLayout[GRID_ROW][GRID_COL] = {
                {1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1}, 
                {1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1},
                {1,0,2,2,2,0,0,0,0,0,0,0,0,0,0,0,0,1},
                {1,0,2,0,2,0,0,0,0,0,0,0,0,0,0,0,0,1},
                {1,0,2,2,2,0,0,0,0,0,0,0,0,0,0,0,0,1},
                {1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1},
                {1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1},
                {1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1},
                {1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1},
                {1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1},
                {1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1},
                {1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1}
            };
            
            for (int row = 0; row < GRID_ROW; row++) {
                for (int col = 0; col < GRID_COL; col++) {
                    loadMapTile(row, col, mapLayout[row][col]);
                }
            }
            break;
        }
    }
}

void Map::updateMap() {

}
