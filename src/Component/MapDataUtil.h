//
// Created by ounol on 2021-10-25.
//

#pragma once


#include <Component/SComponent.h>
#include "DirectionDefine.h"

class MapDataUtil : public CSE::SComponent {
public:
    enum NodeState {
        NONE = 0,
        POINT = 1,
        REV_ATT = 2,
        TELEPORT = 3,
        INBOX_WALL = 8,
        WALL = 9,
    };
public:
    MapDataUtil();

    ~MapDataUtil() override;

    void InitMapData();

    void Init() override;

    void Tick(float elapsedTime) override;

    void Exterminate() override;

    static CSE::ivec2 GetIndexToPosition(const CSE::vec3& position);

    static CSE::vec3 GetPositionToIndex(const CSE::ivec2& index);

    static std::list<Direction> GetTilePath(const CSE::ivec2& index, bool inboxIgnore = false);

    static NodeState GetNode(const CSE::ivec2& index) {
        return m_mapNode[index.x][index.y];
    }

    static std::vector<std::vector<NodeState>> GetMapNode() {
        return m_mapNode;
    }

    static std::list<CSE::ivec2> PathFinding(CSE::ivec2 start, CSE::ivec2 end);

    static CSE::ivec2 GetSize() {
        return m_size;
    }

private:
    /**
     * A*와 Dijkstra 알고리즘과 비슷해보이지만 길 파악 후 최적의 경로를 찾는 것이 아닌
     * 일정 거리를 유지하는 선에서 먼저 찾은 경로를 반환합니다.
     * 이 알고리즘은 기존 맵의 데이터 수정이나 F값은 따로 저장되는 점이 없는 것이 특징입니다.
     */
    static bool SetPathToFind(std::list<CSE::ivec2>& src, std::list<CSE::ivec2>& history, CSE::ivec2 current,
                              CSE::ivec2 start, CSE::ivec2 end);

private:
    static std::vector<std::vector<NodeState>> m_mapNode;
    static CSE::ivec2 m_size;
    static CSE::ivec2 m_center;

    constexpr static float s_space = 1.34f;
};

